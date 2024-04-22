#include <STC15F2K60S2.H>

// 滑动平均滤波器函数
// 参数：new_data - 新的输入数据
// 返回值：滤波后的数据
int Moving_Average_Filter(int ch,int new_data); 

// 中值滤波器函数
// 参数：new_data - 新的输入数据
// 返回值：滤波后的数据
int Median_Filter(int new_data);