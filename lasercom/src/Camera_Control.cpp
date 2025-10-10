#include "Camera_Control.h"


XGpio AxiGpio_SDI;

//配置SDI选择输出GPIO初始化
void Sdi_Gpio_Config(void)
{
	XGpio_Initialize(&AxiGpio_SDI, GPIO_SDI_DEVICE_ID);
}

//SDI显示选择；0：粗跟踪相机；1：精跟踪相机
void Sdi_Show_Choice(u32 Choice_Mark)
{
	XGpio_DiscreteWrite(&AxiGpio_SDI, 1, Choice_Mark);
}
