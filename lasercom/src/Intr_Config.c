#include "Intr_Config.h"
XScuGic IntcInstance; /* 中断控制器 */
extern XCanPs Can_Elmo;    /* CAN驱动器 */
extern XUartPs Uart_Ps_dw;           //串口驱动程序实例

extern XUartNs550 *EDFA_FRONTLOAD;  //前放EDFA
extern XUartNs550 *EDFA_AMPLIFIER; //功放EDFA
extern XUartNs550 *EDFA_LIGHT;     //光源
extern XUartNs550 *DATA_RADIO;    //数传电台


#define INT_TYPE_RISING_EDGE    0x03
#define INT_TYPE_HIGHLEVEL      0x01
#define INT_TYPE_MASK           0x03
#define INT_CFG0_OFFSET 0x00000C00


//


//Uart16550中断配置
void XScuGic_Connect_Uart16550(u32 intId, Xil_InterruptHandler handler, void *CallBackRef)
{
	XScuGic_Connect(&IntcInstance, intId, handler, CallBackRef);

	XScuGic_SetPriorityTriggerType(&IntcInstance, intId, INT_TYPE_PRIORITY_FOR_PL,
		INT_TYPE_HIGH_LEVEL);
}


//中断类型设置：上升沿/高电平
void IntcTypeSetup(XScuGic *InstancePtr, int intId, int intType)
{
    int mask;

    intType &= INT_TYPE_MASK;
    mask = XScuGic_DistReadReg(InstancePtr, INT_CFG0_OFFSET + (intId/16)*4);
    mask &= ~(INT_TYPE_MASK << (intId%16)*2);
    mask |= intType << ((intId%16)*2);
    XScuGic_DistWriteReg(InstancePtr, INT_CFG0_OFFSET + (intId/16)*4, mask);
}


//中断初始化函数
int All_Intr_Config(void)
{
	int Status;
	XScuGic_Config *IntcConfig = NULL;
	//中断控制器设置
	 Xil_ExceptionInit(); //初始化处理系统中断的异常控制模块
	 IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);//查询设备
	 if(IntcConfig == NULL)
	 {
	     return FALSE;
	 }
	 Status = XScuGic_CfgInitialize(&IntcInstance, IntcConfig,IntcConfig->CpuBaseAddress);  //初始化中断控制器
	 if(Status != XST_SUCCESS)
	 {
	    return FALSE;
	 }
	 //设置并打开中断异常处理功能
	 Xil_ExceptionInit();

	 Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 &IntcInstance); //将中断控制器的中断连接到硬件中断处理器

	 Xil_ExceptionEnable(); //在处理器中使能中断

	 //ELMO中断
	 Status = XScuGic_Connect(&IntcInstance, Can_Elmo_INTR_VEC_ID,
	 		(Xil_InterruptHandler)XCanPs_IntrHandler,
	 		(void *)&Can_Elmo); //连接设备驱动处理程序
	 if (Status != XST_SUCCESS) {
	 	return FALSE;
	 }
	 //迪文屏中断
	 XScuGic_Connect(&IntcInstance, UART_PS0_INT_IRQ_ID,
	 (Xil_ExceptionHandler) uart_dw_intr_handler,(void *) &Uart_Ps_dw);

	 //相机中断
	 XScuGic_Connect(&IntcInstance, INTC_FINE_INT_ID,
	 					 (Xil_ExceptionHandler)Track_Fine,
	 					 (void *)&IntcInstance);
	 XScuGic_Connect(&IntcInstance, INTC_COARSE_INT_ID,
	 					(Xil_ExceptionHandler)Track_Coarse,
	 					(void *)&IntcInstance);
	 //设置相机中断类型
	 IntcTypeSetup(&IntcInstance, INTC_FINE_INT_ID, INT_TYPE_RISING_EDGE);
	 IntcTypeSetup(&IntcInstance, INTC_COARSE_INT_ID, INT_TYPE_RISING_EDGE);

	 //设置UART的中断触发方式
	 XUartPs_SetInterruptMask(&Uart_Ps_dw, XUARTPS_IXR_RXOVR);
	 //前放EDFA
	 XScuGic_Connect_Uart16550(UART_INT_PL_IRQ_ID, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler,
	 		(void *)EDFA_FRONTLOAD);
	 //功放EDFA
	 XScuGic_Connect_Uart16550(UART_INT_PL_IRQ_ID1, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler1,
	 		(void *)EDFA_AMPLIFIER);
	 //光源
	 XScuGic_Connect_Uart16550(UART_INT_PL_IRQ_ID2, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler2,
	 		(void *)EDFA_LIGHT);
	 //数传电台
	 XScuGic_Connect_Uart16550(UART_INT_PL_IRQ_ID3, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler3,
	 		(void *)DATA_RADIO);
	 //航姿中断
	 XScuGic_Connect(&IntcInstance, UART_PS1_INT_IRQ_ID,
	             (Xil_ExceptionHandler) yesense_intr_handler,(void *) &Uart_Ps);
	 XUartPs_SetInterruptMask(&Uart_Ps, XUARTPS_IXR_RXOVR);

	 //使能elmo中断
	 XScuGic_Enable(&IntcInstance, Can_Elmo_INTR_VEC_ID); //使能CAN设备中断
	 //使能GIC中的串口中断
	 XScuGic_Enable(&IntcInstance, UART_PS0_INT_IRQ_ID);

	 XScuGic_Enable(&IntcInstance, UART_INT_PL_IRQ_ID);
	 XScuGic_Enable(&IntcInstance, UART_INT_PL_IRQ_ID1);
	 XScuGic_Enable(&IntcInstance, UART_INT_PL_IRQ_ID2);
	 XScuGic_Enable(&IntcInstance, UART_INT_PL_IRQ_ID3);
	XScuGic_Enable(&IntcInstance, UART_PS1_INT_IRQ_ID);
	XScuGic_Enable(&IntcInstance, INTC_FINE_INT_ID);
	XScuGic_Enable(&IntcInstance, INTC_COARSE_INT_ID);



	 return Status;

}

