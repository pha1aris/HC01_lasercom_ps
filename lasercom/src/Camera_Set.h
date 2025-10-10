#ifndef CAMERA_SET_H
#define CAMERA_SET_H


#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"


#define GPIO_CAM_SEL_ID   XPAR_GPIO_5_DEVICE_ID //片选

#define GPIO_CAM_EN_ID    XPAR_GPIO_6_DEVICE_ID //使能

#define GPIO_CAM_ADDR_ID  XPAR_GPIO_7_DEVICE_ID //地址

#define GPIO_CAM_DATA_ID  XPAR_GPIO_8_DEVICE_ID //数据

#define GPIO_CAM_RATA_ID  XPAR_GPIO_9_DEVICE_ID //速率

#define FINE_CAMERA    2
#define COARSE_CAMERA  1


//相机寄存器定义
#define CAMERA_EX_ADDR			201

#define CAMERA_CONFIG_ADDR		192

#define CAMERA_X				256

#define	CAMERA_Y_START			257

#define	CAMERA_Y_END			258

#define CAMERA_AEC_X			253

#define CAMERA_AEC_Y_START		254

#define CAMERA_AEC_Y_END		255



void Camera_Set_Gpio_Config(void); //初始化AXI-GPIO
void Fine_Camera_ExSet(u32 ExData);//精跟踪相机曝光设置
void Coarse_Camera_ExSet(u32 ExData);//粗跟踪相机曝光设置
void Fine_Camera_ROI_Set(u32 X_Start,u32 X_Size,u32 Y_Start,u32 Y_Size);;//精跟踪相机开窗设置
void Coarse_Camera_ROI_Set(u32 X_Start,u32 X_Size,u32 Y_Start,u32 Y_Size);//粗跟踪相机开窗设置
//void Camera_Rata_Read(u32 *Fine_Coaser_Rata,u32 *Coarse_Coaser_Rata);//获取相机帧率



#endif
