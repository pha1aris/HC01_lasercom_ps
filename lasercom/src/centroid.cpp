#include "centroid.h"

XGpio AxiGpio_T;
XGpio AxiGpio_A;
XGpio AxiGpio_B;



//d质心AXI_GPIO初始化
void Centroid_Gpio_Config(void)
{

	XGpio_Initialize(&AxiGpio_T, GPIO_T_DEVICE_ID);
	XGpio_Initialize(&AxiGpio_A, GPIO_A_DEVICE_ID);
	XGpio_Initialize(&AxiGpio_B, GPIO_B_DEVICE_ID);
}

//设置粗相机阈值
void Set_Coaser_Threshold(u32 Threshold)
{
	XGpio_DiscreteWrite(&AxiGpio_T, 1, Threshold);
}

//设置精相机阈值
void Set_Fine_Threshold(u32 Threshold)
{
	XGpio_DiscreteWrite(&AxiGpio_T, 2, Threshold);
}

//读取粗相机质心
void Read_Coaser_Centroid(u32 *Coaser_X,u32 *Coaser_Y)
{
	*Coaser_X = XGpio_DiscreteRead(&AxiGpio_A, 1);
	*Coaser_Y = XGpio_DiscreteRead(&AxiGpio_A, 2);
}

//读取精相机质心
void Read_Fine_Centroid(u32 *Coaser_X,u32 *Coaser_Y)
{
	*Coaser_X = XGpio_DiscreteRead(&AxiGpio_B, 1);
	*Coaser_Y = XGpio_DiscreteRead(&AxiGpio_B, 2);
}



