#include "motor.h"

//状态记录
bool x_state[2] = { 0,0 };
bool y_state[2] = { 0,0 };
bool x_init = 0;
bool y_init = 0;

//x,y,z输出轴圈数
float rev_output_x(float distance)
{
    return distance / d_ratio_x;
}
float rev_output_y(float distance)
{
    return distance / d_ratio_y;
}
float rev_output_z(float distance)
{
    return distance / p_z;
}
//x,y,z原始轴圈数
float rev_src_x(float rev_output)
{
    return rev_output * ratio_x;
}
float rev_src_y(float rev_output)
{
    return rev_output * ratio_y;
}
float rev_src_z(float rev_output)
{
    return rev_output * ratio_z;
}
//x,y,z 由移动距离转化返回原始电机圈数
float distance_to_srcRev(int iaxis, float distance)
{
    if(iaxis == 0)
    	return rev_src_x(rev_output_x(distance));
    if(iaxis == 1)
    	return rev_src_y(rev_output_y(distance));
    if(iaxis == 2)
    	return rev_src_z(rev_output_z(distance));
}

//单轴圈数执行, 此函数给下面函数打包用
void iaxis_move_rev(ZMC_HANDLE g_handle, int iaxis, float speed, float rev)
{
    ZAux_Direct_SetAtype(g_handle, iaxis, 1);

    ZAux_Direct_SetUnits(g_handle, iaxis, units);
    ZAux_Direct_SetInvertStep(g_handle, iaxis, 1);
    ZAux_Direct_SetSpeed(g_handle, iaxis, speed);
    ZAux_Direct_SetAccel(g_handle, iaxis, 2000);
    ZAux_Direct_SetDecel(g_handle, iaxis, 2000);
    ZAux_Direct_SetSramp(g_handle, iaxis, 100);
    ZAux_Direct_Single_Move(g_handle, iaxis, rev);
}

void iaxis_move_rev_abs(ZMC_HANDLE g_handle, int iaxis, float speed, float rev)
{
    ZAux_Direct_SetAtype(g_handle, iaxis, 1);

    ZAux_Direct_SetUnits(g_handle, iaxis, units);
    ZAux_Direct_SetInvertStep(g_handle, iaxis, 1);
    ZAux_Direct_SetSpeed(g_handle, iaxis, speed);
    ZAux_Direct_SetAccel(g_handle, iaxis, 2000);
    ZAux_Direct_SetDecel(g_handle, iaxis, 2000);
    ZAux_Direct_SetSramp(g_handle, iaxis, 100);
    ZAux_Direct_Single_MoveAbs(g_handle, iaxis, rev);
}

//x,y,z单轴圈数执行
void iaxis_XYZ(ZMC_HANDLE g_handle, int iaxis, float speed, float distance)
{
    if(iaxis == 0)
    {
        if(distance >= 0)
        {
            x_state[1] = 1;
        }
        else
        {
            x_state[1] = 0;
        }

        if (x_init == 0)
        {
            if (distance >= 0)
            {
                distance += 5.3;
            }
            else
            {
                distance -= 5.3;
            }
            x_init = 1;
        }
        else {
            if (x_state[0] != x_state[1])
            {
                if (distance >= 0)
                {
                    distance += 5.3;
                }
                else
                {
                    distance -= 5.3;
                }
            }
        }
        
    }

    if(iaxis == 1)
    {
        if(distance >= 0)
        {
            y_state[1] = 1;
        }
        else
        {
            y_state[1] = 0;
        }

        if (y_init == 0)
        {
            if (distance >= 0)
            {
                distance += 1.0;
            }
            else
            {
                distance -= 1.0;
            }
            y_init = 1;
        }
        else {
            if (y_state[0] != y_state[1])
            {
                if (distance >= 0)
                {
                    distance += 1.0;
                }
                else
                {
                    distance -= 1.0;
                }
            }
        }
    }
    iaxis_move_rev(g_handle, iaxis, speed, distance_to_srcRev(iaxis, distance));
    if (iaxis == 0)
    {
        x_state[0] = x_state[1];
    }
    if(iaxis == 1)
    {
        y_state[0] = y_state[1];
    }
    
}

//x,y,z单轴圈数执行
void iaxis_XYZ(ZMC_HANDLE g_handle, int iaxis, float speed, float distance, float test)
{
    iaxis_move_rev(g_handle, iaxis, speed, distance_to_srcRev(iaxis, distance));
}

//iaxis_move_rev_abs
void iaxis_XYZ_abs(ZMC_HANDLE g_handle, int iaxis, float speed, float distance)
{
    iaxis_move_rev_abs(g_handle, iaxis, speed, distance_to_srcRev(iaxis, distance));
}


//A轴输出轴圈数
float rev_output_a(float angle)
{
    return angle / 360.0;
}
//A轴原始轴圈数
float rev_src_a(float rev_output)
{
    return rev_output * ratio_a;
}
//A 由转动角度转化返回原始电机圈数
float angle_to_srcRev(float angle)
{
	return rev_src_a(rev_output_a(angle));
}

//a单轴圈数执行
void iaxis_A(ZMC_HANDLE g_handle, int iaxis, float speed, float angle)
{
    speed = speed * 4.0;
    iaxis_move_rev(g_handle, iaxis, speed, angle_to_srcRev(angle));
}


//连续运动
/**
 *连续运动
 *dir:-1, 往回跑；
 */
void iaxis_continuous_move(ZMC_HANDLE g_handle, int iaxis, float speed, int dir)
{

    ZAux_Direct_SetAtype(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲输出轴
    ZAux_Direct_SetUnits(g_handle, iaxis, units); //设置轴 iaxis 轴参数以 units 个脉冲为单位
    ZAux_Direct_SetInvertStep(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲+方向模式
    ZAux_Direct_SetSpeed(g_handle, iaxis, speed); //设置轴 iaxis 速度
    ZAux_Direct_SetAccel(g_handle, iaxis, 2000); //设置轴 iaxis 加速度为 2000units/s/s
    ZAux_Direct_SetDecel(g_handle, iaxis, 2000); //设置轴 iaxis 减速度为 2000units/s/s
    ZAux_Direct_SetSramp(g_handle, iaxis, 100); //设置轴 iaxisS 曲线时间 0(梯形加减速)

    ZAux_Direct_Single_Vmove(g_handle, iaxis, dir);
}



/**
 *连续运动
 *全部往回跑；
 */
void iaxis_all_reback(ZMC_HANDLE g_handle)  //复位调这个函数
{
    iaxis_continuous_move(g_handle, 0, 5, 1);
    iaxis_continuous_move(g_handle, 1, 5, -1);
    iaxis_continuous_move(g_handle, 2, 1, 1);
    iaxis_continuous_move(g_handle, 3, 1, 1);
}



void motor_test(ZMC_HANDLE g_handle, int iaxis, float speed, float distance)
{
    float pulse = 5000.0;
    //distance = distance * 19.0;

    ZAux_Direct_SetAtype(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲输出轴
    ZAux_Direct_SetUnits(g_handle, iaxis, pulse); //设置轴 iaxis 轴参数以 units 个脉冲为单位
    ZAux_Direct_SetInvertStep(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲+方向模式
    ZAux_Direct_SetSpeed(g_handle, iaxis, speed); //设置轴 iaxis 速度
    ZAux_Direct_SetAccel(g_handle, iaxis, 2000); //设置轴 iaxis 加速度为 2000units/s/s
    ZAux_Direct_SetDecel(g_handle, iaxis, 2000); //设置轴 iaxis 减速度为 2000units/s/s
    ZAux_Direct_SetSramp(g_handle, iaxis, 100); //设置轴 iaxisS 曲线时间 0(梯形加减速)

    ZAux_Direct_Single_Move(g_handle, iaxis, distance);
}

void motor_init(ZMC_HANDLE g_handle)
{
    int iaxis = 0;
    for (; iaxis < 4; iaxis++)
    {
        ZAux_Direct_SetAtype(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲输出轴
        ZAux_Direct_SetUnits(g_handle, iaxis, units); //设置轴 iaxis 轴参数以 units 个脉冲为单位
        ZAux_Direct_SetInvertStep(g_handle, iaxis, 1); //设置轴 iaxis 为脉冲+方向模式
        ZAux_Direct_SetSpeed(g_handle, iaxis, speed_all); //设置轴 iaxis 速度
        ZAux_Direct_SetAccel(g_handle, iaxis, 2000); //设置轴 iaxis 加速度为 2000units/s/s
        ZAux_Direct_SetDecel(g_handle, iaxis, 2000); //设置轴 iaxis 减速度为 2000units/s/s
        ZAux_Direct_SetSramp(g_handle, iaxis, 100); //设置轴 iaxisS 曲线时间 0(梯形加减速)
    }
}





/**
 * @brief 直线插补函数，使用相对运动
 * @param current     参数1 当前点
 * @param next        参数2 目标点
 *
 * @return 返回说明
 *     -<em>false</em> fail
 *     -<em>true</em> succeed
 */
void linear_interpolation(Point2f& current, Point2f next)
{

}

void model(Pos target)
{
    float z0 = 89.75;
    float z1 = 0.0;

    float theta = asin_deg(target.o[2]);
    float X = target.x;
    float Y = z1 * sin_deg(theta) + target.y;
    float Z = -z0 - z1 * cos_deg(theta) + target.z;

    cout << "theta = " << theta << " " << "X = " << X << " " << "Y = " << Y << " " << "Z = " << Z << endl;
}

void model(ZMC_HANDLE g_handle, Pos target)
{
    float z0 = 89.75;
    float z1 = 0.0;

    float theta = asin_deg(target.o[2]);
    float X = target.x;
    float Y = z1 * sin_deg(theta) + target.y;
    float Z = -z0 - z1 * cos_deg(theta) + target.z;

    iaxis_XYZ_abs(g_handle, 0, 5, -X);
    iaxis_XYZ_abs(g_handle, 1, 5, Y);
    iaxis_XYZ_abs(g_handle, 2, 5, Z);
    iaxis_A(g_handle, 3, 5, -theta);

}

void cir_inter(ZMC_HANDLE g_handle, Point2f start, Point2f end, float step_angle)
{
    int run_state = 0; //轴运动状态
    iaxis_XYZ_abs(g_handle, 0, 5, -start.x);
    do
    {
        ZAux_Direct_GetIfIdle(g_handle, 0, &run_state); //读取轴 0 运动状态， 0-运动， 1-
    } while (run_state == 0); //等待轴 0 停止
    iaxis_XYZ_abs(g_handle, 1, 5, start.y);
    do
    {
        ZAux_Direct_GetIfIdle(g_handle, 1, &run_state); //读取轴 0 运动状态， 0-运动， 1-
    } while (run_state == 0); //等待轴 1 停止




    
    Point2f mid;
    mid.x = (start.x + end.x) / 2.0;
    mid.y = (start.y + end.y) / 2.0;
    float r = sqrt(pow((end.x - mid.x), 2) + pow((end.y - mid.y), 2));

    int Point_num = (int)(180.0 / step_angle);
    cout << "Point_num = " << Point_num << endl;

    Point2f* continuous = new Point2f[Point_num];

    float theta = 0.0;

    for (int i = 0; i <= Point_num; i++)
    {
        continuous[i].x = mid.x - r * cos_deg(theta);
        continuous[i].y = mid.y + r * sin_deg(theta);

        theta += step_angle;
    }

    for (int i = 0; i <= Point_num; i++)
    {
        iaxis_XYZ_abs(g_handle, 0, 5, -continuous[i].x);
        do
        {
            ZAux_Direct_GetIfIdle(g_handle, 0, &run_state); //读取轴 0 运动状态， 0-运动， 1-
        } while (run_state == 0); //等待轴 0 停止
        iaxis_XYZ_abs(g_handle, 1, 5, continuous[i].y);
        do
        {
            ZAux_Direct_GetIfIdle(g_handle, 1, &run_state); //读取轴 0 运动状态， 0-运动， 1-
        } while (run_state == 0); //等待轴 1 停止
    }

    
        
}