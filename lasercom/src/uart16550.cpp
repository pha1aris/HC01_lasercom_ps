#include "uart16550.h"
#include "uart_dw.h"
#include "Dw_Data_Define.h"


XUartNs550 gUartNs550Instance = {};
XUartNs550 *EDFA_FRONTLOAD = &gUartNs550Instance;

XUartNs550 gUartNs550Instance1 = {};
XUartNs550 *EDFA_AMPLIFIER = &gUartNs550Instance1;

XUartNs550 gUartNs550Instance2 = {};
XUartNs550 *EDFA_LIGHT = &gUartNs550Instance2;

XUartNs550 gUartNs550Instance3 = {};
XUartNs550 *DATA_RADIO = &gUartNs550Instance3;

//前放EDFA数据
u8 ReceivedBufferPL[MAX_LEN];
u8 *ReceivedBuffer_FRONTLOAD = ReceivedBufferPL;
u8 rcvCount_FRONTLOAD = 0;
u8 rcvCount_FRONTLOAD_Cnt = 0;
u16 Read_FRONTLOAD_Power;
u8	Read_FRONTLOAD_Data_Valid = 0; //数据有效信号
//功放EDFA数据
u8 ReceivedBufferPL1[MAX_LEN];
u8 *ReceivedBuffer_AMPLIFIER = ReceivedBufferPL1;
u8 rcvCount_AMPLIFIER = 0;
u8 rcvCount_AMPLIFIER_Cnt = 0;
u16 Read_AMPLIFIER_Power;
u16 Read_PD_Power;
u16 SIGNAL_LIGHT_IN_Power;
u8	Read_AMPLIFIER_Data_Valid = 0; //数据有效信号
//光源数据
u8 ReceivedBufferPL2[MAX_LEN];
u8 *ReceivedBuffer_LIGHT = ReceivedBufferPL2;
u8 rcvCount_LIGHT = 0;
u8 rcvCount_LIGHT_Cnt = 0;
u16 Read_LIGHT_Power;
u8	Read_LIGHT_Data_Valid = 0; //数据有效信号

//数传电台
u8 ReceivedBufferPL3[MAX_LEN];
u8 *ReceivedBuffer_DATA_RADIO = ReceivedBufferPL3;
u8 rcvCount_DATA_RADIO = 0;
u8 rcvCount_DATA_RADIO_Cnt = 0;
u8	Read_DATA_RADIO_Data_Valid = 0; //数据有效信号
/*********************
 * intc
 *********************/
XScuGic IntcInst = {};
XScuGic *IntcInstPtr = &IntcInst;

int Uart16550Init(u32 uartDevId, u32 baudRate, XUartNs550 *uartInstPtr)
{
	XUartNs550_Config *configPtr = XUartNs550_LookupConfig(uartDevId);
	if(NULL == configPtr)
	{
		return XST_FAILURE;
	}

	configPtr->DefaultBaudRate = baudRate;

	int status = XUartNs550_CfgInitialize(uartInstPtr, configPtr, configPtr->BaseAddress);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// 配置中断的初始化,该函数Options未配置就会删除对应的使能
	status = XUartNs550_SetOptions(uartInstPtr, XUN_OPTION_DATA_INTR | XUN_OPTION_FIFOS_ENABLE);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = XUartNs550_SetFifoThreshold(uartInstPtr, XUN_FIFO_TRIGGER_01);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

void UartPlSend(XUartNs550 *InstancePtr, u8 *BufferPtr, u32 NumBytes)//发送数据
{
	for(u32 i = 0; i < NumBytes; i++)
	{
		XUartNs550_SendByte(InstancePtr->BaseAddress, BufferPtr[i]);
	}
}

/*
 *前放EDFA接收中断函数
 */
void DevicePlIntrUartDriverHandler(void *callbackRefPL)
{

	XUartNs550 *uartNs550InstancePtr = (XUartNs550 *)callbackRefPL;

	u8 isrStatus = XUartNs550_ReadReg(uartNs550InstancePtr->BaseAddress, XUN_IIR_OFFSET)
		& XUN_INT_ID_MASK;

	if((IIR_INT_ID_RDA == isrStatus) || (IIR_INT_ID_TIMEOUT == isrStatus))
	{

		u8  rcvCount_Cnt = XUartNs550_Recv(uartNs550InstancePtr, ReceivedBuffer_FRONTLOAD+rcvCount_FRONTLOAD_Cnt, MAX_LEN);
		if(*(ReceivedBuffer_FRONTLOAD) == 0xAA) //判断帧头
		{
			rcvCount_FRONTLOAD_Cnt +=rcvCount_Cnt;
			if(rcvCount_FRONTLOAD_Cnt >1)
			{
				if(*(ReceivedBuffer_FRONTLOAD + 1) == 0x55) //判断帧头
				{
					if(rcvCount_FRONTLOAD_Cnt == (*(ReceivedBuffer_FRONTLOAD+10) + 11))  //数据接收完成
					{
						rcvCount_FRONTLOAD = rcvCount_FRONTLOAD_Cnt;
						rcvCount_FRONTLOAD_Cnt = 0;
						Read_FRONTLOAD_Data_Valid = 1;
						//对数据进行判断并处理
						//UartPlSend(EDFA_AMPLIFIER,ReceivedBuffer_AMPLIFIER,rcvCount_AMPLIFIER);//数据发送模块
					}
					else if(rcvCount_FRONTLOAD_Cnt > (*(ReceivedBuffer_FRONTLOAD+10) + 11))
					{
						rcvCount_FRONTLOAD_Cnt = 0;
						Read_FRONTLOAD_Data_Valid = 0;
					}
				}
				else
				{
					rcvCount_FRONTLOAD_Cnt = 0;
				}
			}

		}
		else
		{
			rcvCount_FRONTLOAD_Cnt = 0;
		}

	}

}
/*
 *功放EDFA接收中断函数
 */
void DevicePlIntrUartDriverHandler1(void *callbackRefPL)
{
	XUartNs550 *uartNs550InstancePtr = (XUartNs550 *)callbackRefPL;

	u8 isrStatus = XUartNs550_ReadReg(uartNs550InstancePtr->BaseAddress, XUN_IIR_OFFSET)
		& XUN_INT_ID_MASK;
	if((IIR_INT_ID_RDA == isrStatus) || (IIR_INT_ID_TIMEOUT == isrStatus))
	{

		u8  rcvCount_Cnt = XUartNs550_Recv(uartNs550InstancePtr, ReceivedBuffer_AMPLIFIER+rcvCount_AMPLIFIER_Cnt, MAX_LEN);
		if(*(ReceivedBuffer_AMPLIFIER) == 0xAA) //判断帧头
		{
			rcvCount_AMPLIFIER_Cnt +=rcvCount_Cnt;
			if(rcvCount_AMPLIFIER_Cnt >1)
			{
				if(*(ReceivedBuffer_AMPLIFIER + 1) == 0x55) //判断帧头
				{
					if(rcvCount_AMPLIFIER_Cnt == (*(ReceivedBuffer_AMPLIFIER+10) + 11))  //数据接收完成
					{
						rcvCount_AMPLIFIER = rcvCount_AMPLIFIER_Cnt;
						rcvCount_AMPLIFIER_Cnt = 0;
						Read_AMPLIFIER_Data_Valid = 1;
					}
					else if(rcvCount_AMPLIFIER_Cnt > (*(ReceivedBuffer_AMPLIFIER+10) + 11))
					{
						rcvCount_AMPLIFIER_Cnt = 0;
						Read_AMPLIFIER_Data_Valid = 0;
						//memset(ReceivedBuffer_AMPLIFIER,0,sizeof(ReceivedBuffer_AMPLIFIER)*sizeof(ReceivedBuffer_AMPLIFIER[0]));
					}
				}
				else
				{
					rcvCount_AMPLIFIER_Cnt = 0;
					//memset(ReceivedBuffer_AMPLIFIER,0,sizeof(ReceivedBuffer_AMPLIFIER)*sizeof(ReceivedBuffer_AMPLIFIER[0]));
				}
			}

		}
		else
		{
			rcvCount_AMPLIFIER_Cnt = 0;
			//memset(ReceivedBuffer_AMPLIFIER,0,sizeof(ReceivedBuffer_AMPLIFIER)*sizeof(ReceivedBuffer_AMPLIFIER[0]));
		}

	}
}

/*
 *光源接收中断函数
 */
void DevicePlIntrUartDriverHandler2(void *callbackRefPL)
{
	XUartNs550 *uartNs550InstancePtr = (XUartNs550 *)callbackRefPL;

	u8 isrStatus = XUartNs550_ReadReg(uartNs550InstancePtr->BaseAddress, XUN_IIR_OFFSET)
		& XUN_INT_ID_MASK;

	if((IIR_INT_ID_RDA == isrStatus) || (IIR_INT_ID_TIMEOUT == isrStatus))
	{
		u8  rcvCount_Cnt = XUartNs550_Recv(uartNs550InstancePtr, ReceivedBuffer_LIGHT+rcvCount_LIGHT_Cnt, MAX_LEN);
		if(*(ReceivedBuffer_LIGHT) == 0xAA) //判断帧头
		{
			rcvCount_LIGHT_Cnt +=rcvCount_Cnt;
			if(rcvCount_LIGHT_Cnt >1)
			{
				if(*(ReceivedBuffer_LIGHT + 1) == 0x55) //判断帧头
				{
					if(rcvCount_LIGHT_Cnt == (*(ReceivedBuffer_LIGHT+10) + 11))  //数据接收完成
					{
						rcvCount_LIGHT = rcvCount_LIGHT_Cnt;
						rcvCount_LIGHT_Cnt = 0;
						Read_LIGHT_Data_Valid = 1;
					}
					else if(rcvCount_LIGHT_Cnt > (*(ReceivedBuffer_LIGHT+10) + 11))
					{
						rcvCount_LIGHT_Cnt = 0;
						Read_LIGHT_Data_Valid = 0;
						//memset(ReceivedBuffer_LIGHT,0,sizeof(ReceivedBuffer_LIGHT)*sizeof(ReceivedBuffer_LIGHT[0]));
					}
				}
				else
				{
					rcvCount_LIGHT_Cnt = 0;
					//memset(ReceivedBuffer_LIGHT,0,sizeof(ReceivedBuffer_LIGHT)*sizeof(ReceivedBuffer_LIGHT[0]));
				}
			}

		}
		else
		{
			rcvCount_LIGHT_Cnt = 0;
			//memset(ReceivedBuffer_LIGHT,0,sizeof(ReceivedBuffer_LIGHT)*sizeof(ReceivedBuffer_LIGHT[0]));
		}
	}
}

/*
	数传电台中断函数
*/

void DevicePlIntrUartDriverHandler3(void *callbackRefPL)///apd
{
	XUartNs550 *uartNs550InstancePtr = (XUartNs550 *)callbackRefPL;

	u8 isrStatus = XUartNs550_ReadReg(uartNs550InstancePtr->BaseAddress, XUN_IIR_OFFSET)
		& XUN_INT_ID_MASK;

	if((IIR_INT_ID_RDA == isrStatus) || (IIR_INT_ID_TIMEOUT == isrStatus))
	{
		u8  rcvCount_Cnt = XUartNs550_Recv(uartNs550InstancePtr, ReceivedBuffer_DATA_RADIO+rcvCount_DATA_RADIO_Cnt, MAX_LEN);
		if(*(ReceivedBuffer_DATA_RADIO) == 0xAA) //判断帧头
		{
			rcvCount_DATA_RADIO_Cnt +=rcvCount_Cnt;
			if(rcvCount_DATA_RADIO_Cnt >1)
			{
				if(*(ReceivedBuffer_DATA_RADIO + 1) == 0x55) //判断帧头
				{
					if(rcvCount_DATA_RADIO_Cnt == (*(ReceivedBuffer_DATA_RADIO+2) + 3))  //数据接收完成
					{
						rcvCount_DATA_RADIO = rcvCount_DATA_RADIO_Cnt;
						rcvCount_DATA_RADIO_Cnt = 0;
						Read_DATA_RADIO_Data_Valid = 1;
					}
					else if(rcvCount_DATA_RADIO_Cnt > (*(ReceivedBuffer_DATA_RADIO+2) + 3))  //数据接收完成
					{
						rcvCount_DATA_RADIO_Cnt = 0;
						Read_DATA_RADIO_Data_Valid = 0;
					}
				}
				else
				{
					rcvCount_DATA_RADIO_Cnt = 0;
				}
			}

		}
		else
		{
			rcvCount_DATA_RADIO_Cnt = 0;
		}
	}
}

void XScuGic_ConnectPl(u32 intId, Xil_InterruptHandler handler, void *CallBackRef)
{
	XScuGic_Connect(IntcInstPtr, intId, handler, CallBackRef);

	XScuGic_SetPriorityTriggerType(IntcInstPtr, intId, INT_TYPE_PRIORITY_FOR_PL,
		INT_TYPE_HIGH_LEVEL);
}

int IntcInit()
{
	XScuGic_Config *IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig)
	{
		return XST_FAILURE;
	}

	int Status = XScuGic_CfgInitialize(IntcInstPtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler,
		IntcInstPtr);

	XScuGic_ConnectPl(UART_INT_PL_IRQ_ID, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler,
		(void *)EDFA_FRONTLOAD);

	XScuGic_ConnectPl(UART_INT_PL_IRQ_ID1, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler1,
		(void *)EDFA_AMPLIFIER);

	XScuGic_ConnectPl(UART_INT_PL_IRQ_ID2, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler2,
		(void *)EDFA_LIGHT);

	XScuGic_ConnectPl(UART_INT_PL_IRQ_ID3, (Xil_ExceptionHandler)DevicePlIntrUartDriverHandler3,
		(void *)DATA_RADIO);

	//XScuGic_Enable(IntcInstPtr, UART_INT_IRQ_ID);

	XScuGic_Enable(IntcInstPtr, UART_INT_PL_IRQ_ID);
	XScuGic_Enable(IntcInstPtr, UART_INT_PL_IRQ_ID1);
	XScuGic_Enable(IntcInstPtr, UART_INT_PL_IRQ_ID2);
	XScuGic_Enable(IntcInstPtr, UART_INT_PL_IRQ_ID3);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

int Uart16550_InitProc()
{

	int status;
	/*uart 16550 init*/
	status = Uart16550Init(UART_16550_DEVICE_ID, UART_16550_BAUD_RATE, EDFA_FRONTLOAD);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = Uart16550Init(UART_16550_DEVICE_ID1, UART_16550_BAUD_RATE1, EDFA_AMPLIFIER);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = Uart16550Init(UART_16550_DEVICE_ID2, UART_16550_BAUD_RATE2, EDFA_LIGHT);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	status = Uart16550Init(UART_16550_DEVICE_ID3, UART_16550_BAUD_RATE3, DATA_RADIO);
	if(status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/*intc init*/
//	status = IntcInit();
//
//	if(status != XST_SUCCESS)
//	{
//		return XST_FAILURE;
//	}

	return XST_SUCCESS;
}
