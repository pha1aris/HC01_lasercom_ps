#ifndef DA_H
#define DA_H

#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"

#define GPIO_D_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID

#define DA_BIT 65535
#define DA_BIG 10


//初始化DA——Gpio
void DA_Gpio_Config(void);


//设置DA输出电压
void Set_DA_Out(float DA_Out_X,float DA_Out_Y);
void Set_PZT_X_Out(float DA_Out_X);
void Set_PZT_Y_Out(float DA_Out_Y);

#endif
