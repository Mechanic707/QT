import pyrealsense2 as rs
import numpy as np

# pipeline = rs.pipeline()
# config = rs.config()
# config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)      # 配置depth流
# config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

def depth_filter(aligned_depth_frame):   
    depth_to_disparity = rs.disparity_transform(True)
    disparity_to_depth = rs.disparity_transform(False)

    spatial = rs.spatial_filter()
    temporal = rs.temporal_filter()
    hole_filling = rs.hole_filling_filter()

    frame_filtered = aligned_depth_frame
    frame_filtered = depth_to_disparity.process(frame_filtered)
    frame_filtered = spatial.process(frame_filtered)
    frame_filtered = temporal.process(frame_filtered)
    frame_filtered = disparity_to_depth.process(frame_filtered)
    frame_filtered = hole_filling.process(frame_filtered)
    
    return frame_filtered

def segmentation(aligned_depth_frame, width, height, x0, y0, x1, y1, depth_scale, pixel_dis):
    white_image = np.ones((height, width, 3), dtype=np.uint8) * 255
    x_list = []
    y_list = []
    
    depth_image = np.asanyarray(aligned_depth_frame.get_data())
    for y in range(y0, y1):
        for x in range(x0, x1):
            dis = depth_image[y, x] * depth_scale
            if dis < pixel_dis - 0.017:
                x_list.append(x)
                y_list.append(y)
    
    num = len(x_list)

    for i in range(0, num):
        white_image[y_list[i], x_list[i]] = [0, 0, 0]  # BGR颜色空间中的黑色
    
    return white_image

def get_3d_camera_coordinate(depth_pixel, aligned_depth_frame, depth_intrin, depth_scale, pixel_dis):
    x = depth_pixel[0]
    y = depth_pixel[1]

    depth_image = np.asanyarray(aligned_depth_frame.get_data())

    dis = depth_image[y, x] * depth_scale       # 获取该像素点对应的深度

    camera_coordinate = rs.rs2_deproject_pixel_to_point(depth_intrin, depth_pixel, dis)
    camera_coordinate[2] = pixel_dis - camera_coordinate[2]

    camera_coordinate[0] *= 1000.0
    camera_coordinate[1] *= 1000.0
    camera_coordinate[2] *= 1000.0
    return dis, camera_coordinate

def point_trans(depth_pixel, aligned_depth_frame, depth_intrin, depth_scale, pixel_dis):
    x = depth_pixel[0]
    y = depth_pixel[1]

    depth_image = np.asanyarray(aligned_depth_frame.get_data())

    dis = depth_image[y, x] * depth_scale       # 获取该像素点对应的深度

    camera_coordinate = rs.rs2_deproject_pixel_to_point(depth_intrin, depth_pixel, dis)
    camera_coordinate[2] = pixel_dis - camera_coordinate[2]

    x_fix = 294
    y_fix = 55
    dis_fix = depth_image[y_fix, x_fix] * depth_scale 
    fix_coordinate = rs.rs2_deproject_pixel_to_point(depth_intrin, [y_fix, x_fix], dis_fix)

    x_mid = 320
    y_mid = 240
    dis_mid = depth_image[y_mid, x_mid] * depth_scale 
    mid_coordinate = rs.rs2_deproject_pixel_to_point(depth_intrin, [y_mid, x_mid], dis_mid)

    #cam Trans
    x_offset = mid_coordinate[1]- fix_coordinate[1]
    y_offset = mid_coordinate[0]- fix_coordinate[0]
    print("offset:", x_offset*1000.0, ",", y_offset*1000.0)

    cam_trans = [0, 0, 0, 0]
    cam_trans[0] = camera_coordinate[1] + y_offset
    cam_trans[1] = camera_coordinate[0] + x_offset
    cam_trans[2] = camera_coordinate[2]

    cam_trans[0] *= 1000.0  
    cam_trans[1] *= 1000.0  
    cam_trans[2] *= 1000.0  

    #原点修正
    cam_trans[0] -= 1.0
    cam_trans[1] -= 0.7

    #固定偏移
    cam_trans[0] += 37.0
    cam_trans[1] += 106.5
    return cam_trans


