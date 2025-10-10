#ifndef UART_H
#define UART_H

#include "xparameters.h"
#include "xuartps.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "stdio.h"

#define UART_PS0_DEVICE_ID     XPAR_XUARTPS_0_DEVICE_ID     //串口设备ID
#define INTC_DEVICE_ID     XPAR_SCUGIC_SINGLE_DEVICE_ID //中断ID
#define UART_PS0_INT_IRQ_ID    XPAR_XUARTPS_0_INTR          //串口中断ID




//UART初始化函数
int uart_dw_init(XUartPs* uart_ps,u32 BaudRate);

//UART中断处理函数
void uart_dw_intr_handler(void *call_back_ref);

//串口中断初始化
int uart_dw_intr_init(XScuGic *intc, XUartPs *uart_ps);

// UART发送数据函数
void uart_dw_send_data(XUartPs* uart_ps, u8* data, u32 length);

//迪文屏配置函数
void Dw_Uart_Config(void);

//迪文屏读数据
void Dw_Data_Read(u8 *data_in,u16 *Out_Addr,int32_t *Out_data);

//迪文屏写数据
void Dw_Data_Write(u16 Write_Addr,int32_t Write_Data,u8 Write_Len);

//显示精跟踪相机光斑
void Dw_Fine_Spot_P(double Spot_X,double Spot_Y);

//显示粗跟踪相机光斑
void Dw_Coarse_Spot_P(double Spot_X,double Spot_Y);

#endif // UART_H
