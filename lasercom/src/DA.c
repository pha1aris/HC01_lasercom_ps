#include "DA.h"

XGpio AxiGpio_D;

//初始化DA——Gpio
void DA_Gpio_Config(void)
{
	XGpio_Initialize(&AxiGpio_D, GPIO_D_DEVICE_ID);
}
//设置DA输出电压
void Set_DA_Out(float DA_Out_X,float DA_Out_Y)
{
	u16 data_x;
	u16 data_y;
	data_x = DA_Out_X * DA_BIT /DA_BIG;
	data_y = DA_Out_Y * DA_BIT /DA_BIG;

	XGpio_DiscreteWrite(&AxiGpio_D, 1, data_x);
	XGpio_DiscreteWrite(&AxiGpio_D, 2, data_y);

}


void Set_PZT_X_Out(float DA_Out_X)
{
	u16 data_x;
	data_x = DA_Out_X * DA_BIT /DA_BIG;
	XGpio_DiscreteWrite(&AxiGpio_D, 2, data_x);
}

void Set_PZT_Y_Out(float DA_Out_Y)
{
	u16 data_y;
	data_y = DA_Out_Y * DA_BIT /DA_BIG;
	XGpio_DiscreteWrite(&AxiGpio_D, 1, data_y);
}
