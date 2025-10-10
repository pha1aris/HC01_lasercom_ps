#ifndef UART16550_H
#define UART16550_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xuartns550.h"


#define UART_16550_DEVICE_ID	XPAR_UARTNS550_0_DEVICE_ID
#define UART_16550_DEVICE_ID1	XPAR_UARTNS550_1_DEVICE_ID
#define UART_16550_DEVICE_ID2	XPAR_UARTNS550_2_DEVICE_ID
#define UART_16550_DEVICE_ID3	XPAR_UARTNS550_3_DEVICE_ID

#define TIMER_DEVICE_ID			XPAR_XSCUTIMER_0_DEVICE_ID

#define UART_INT_PL_IRQ_ID		XPAR_FABRIC_UARTNS550_0_VEC_ID
#define UART_INT_PL_IRQ_ID1		XPAR_FABRIC_UARTNS550_1_VEC_ID
#define UART_INT_PL_IRQ_ID2		XPAR_FABRIC_UARTNS550_2_VEC_ID
#define UART_INT_PL_IRQ_ID3		XPAR_FABRIC_UARTNS550_3_VEC_ID

#define INTC_DEVICE_ID			XPAR_SCUGIC_SINGLE_DEVICE_ID

#define IIR_INT_ID_THRE					2
#define IIR_INT_ID_RDA      			4
#define IIR_INT_ID_TIMEOUT      		12

#define UART_16550_BAUD_RATE			115200
#define UART_16550_BAUD_RATE1			115200
#define UART_16550_BAUD_RATE2			115200
#define UART_16550_BAUD_RATE3			57600


#define INT_TYPE_PRIORITY_FOR_PL   	    0xA0
#define INT_TYPE_HIGH_LEVEL             0x01

#define  MAX_LEN 120



int Uart16550Init(u32 uartDevId, u32 baudRate, XUartNs550 *uartInstPtr); //初始化16550UART模块
int IntcInit(); //初始化接收中断函数
void UartPlSend(XUartNs550 *InstancePtr, u8 *BufferPtr, u32 NumBytes);//数据发送模块
int Uart16550_InitProc(); //初始化UART模块，带中断

//中断函数
void DevicePlIntrUartDriverHandler(void *callbackRefPL);//前放EDFA中断函数
void DevicePlIntrUartDriverHandler1(void *callbackRefPL);//功放EDFA中断函数
void DevicePlIntrUartDriverHandler2(void *callbackRefPL);//光源中断函数
void DevicePlIntrUartDriverHandler3(void *callbackRefPL);//数传电台中断函数



#endif
