#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H


#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"

#define GPIO_SDI_DEVICE_ID  XPAR_GPIO_4_DEVICE_ID



void Sdi_Gpio_Config(void);  //初始化SDI_GPIO

void Sdi_Show_Choice(u32 Choice_Mark);  //SDI选择显示函数


#endif
