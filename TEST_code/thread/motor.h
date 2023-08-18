#include <zmotion.h>
#include <zauxdll2.h>
#include <iostream>
#include <math.h> 
#include <vector>
#include "mymath.h"
using namespace std;


struct Point2f {
	float x;
	float y;
};
struct Point3f {
	float x;
	float y;
	float z;
};

struct Pos {
	float x;
	float y;
	float z;

	vector<float> n = { 1.0f, 0.0f, 0.0f };
	vector<float> o = { 0.0f, 1.0f, 0.0f };
	vector<float> a = { 0.0f, 0.0f, 1.0f };
};

#define units 800.0//全局电机细分
#define speed_all 5.0//全局电机转速，单位：rev/s

//减速比
#define ratio_x 25.0
#define ratio_y 19.0
#define ratio_z 1.0
#define ratio_a 100.0

//传动比
#define d_ratio_x 156.0
#define d_ratio_y 78.0
//导程
#define p_z 2.0

//x,y,z距离;
#define distance_x ratio_x*d_ratio_x
#define distance_y ratio_y*d_ratio_y
#define distance_z ratio_z*p_z

//a角度
#define angle_a ratio_a*360.0

//x,y,z距离
//float distance;
//A轴角度
//float angle;

//原始电机转动圈数
void iaxis_move_rev(ZMC_HANDLE g_handle, int iaxis, float speed, float rev);

//x,y,z输出轴圈数
float rev_output_x(float distance);
float rev_output_y(float distance);
float rev_output_z(float distance);
//x,y,z原始轴圈数
float rev_src_x(float rev_output);
float rev_src_y(float rev_output);
float rev_src_z(float rev_output);
//x,y,z 由移动距离转化返回原始电机圈数
float distance_to_srcRev(int iaxis, float distance);
//x,y,z单轴圈数执行
void iaxis_XYZ(ZMC_HANDLE g_handle, int iaxis, float speed, float distance);
void iaxis_XYZ(ZMC_HANDLE g_handle, int iaxis, float speed, float distance, float test);
void iaxis_XYZ_abs(ZMC_HANDLE g_handle, int iaxis, float speed, float distance);
//A轴输出轴圈数
float rev_output_a(float angle);
//A轴原始轴圈数
float rev_src_a(float rev_output);
//A由转动角度转化返回原始电机圈数
float angle_to_srcRev(float angle);
//a单轴圈数执行
void iaxis_A(ZMC_HANDLE g_handle, int iaxis, float speed, float angle);


void iaxis_move_rev_abs(ZMC_HANDLE g_handle, int iaxis, float speed, float rev);


//连续运动
void iaxis_continuous_move(ZMC_HANDLE g_handle, int iaxis, float speed, int dir);
void iaxis_all_reback(ZMC_HANDLE g_handle);


void motor_test(ZMC_HANDLE g_handle, int iaxis, float speed, float distance);
void motor_init(ZMC_HANDLE g_handle);


void model(Pos target);
void model(ZMC_HANDLE g_handle, Pos target);

void cir_inter(ZMC_HANDLE g_handle, Point2f start, Point2f end, float step_angle);