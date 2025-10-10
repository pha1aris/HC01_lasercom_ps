#ifndef CENTROID_H
#define CENTROID_H


#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"


#define GPIO_T_DEVICE_ID  XPAR_GPIO_1_DEVICE_ID
#define GPIO_A_DEVICE_ID  XPAR_GPIO_2_DEVICE_ID
#define GPIO_B_DEVICE_ID  XPAR_GPIO_3_DEVICE_ID

// 定义宏参数
#define THRESHOLD_A         100  // 10位 阈值
#define THRESHOLD_B			100
#define CENTROID_X_A_CHANNEL  1
#define CENTROID_Y_A_CHANNEL  2
#define CENTROID_X_B_CHANNEL  1
#define CENTROID_Y_B_CHANNEL  2


//质心AXI_GPIO初始化
void Centroid_Gpio_Config(void);

//设置粗相机阈值
void Set_Coaser_Threshold(u32 Threshold);

//设置精相机阈值
void Set_Fine_Threshold(u32 Threshold);

//读取粗相机质心
void Read_Coaser_Centroid(u32 *Coaser_X,u32 *Coaser_Y);

//读取精相机质心
void Read_Fine_Centroid(u32 *Coaser_X,u32 *Coaser_Y);


#endif
