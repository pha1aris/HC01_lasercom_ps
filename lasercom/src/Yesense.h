#ifndef YESENSE_H
#define YESENSE_H

#include "xparameters.h"
#include "xuartps.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "stdio.h"
#include "xil_io.h"
#include "analysis_data.h"

#define UART_PS1_DEVICE_ID XPAR_XUARTPS_1_DEVICE_ID
#define INTC_DEVICE_ID XPAR_SCUGIC_SINGLE_DEVICE_ID
#define UART_PS1_INT_IRQ_ID XPAR_XUARTPS_1_INTR

extern u8 Yesense_buffer[1024];
extern u8 Yesense_data_num;
extern u8 Yesense_data_cnt;
extern u8 Yesense_data_valid;
extern unsigned char g_recv_buf[2048];
extern protocol_info_t g_output_info;
extern XScuGic Intc;
extern XUartPs Uart_Ps;

typedef struct {
    XUartPs uart_instance;
    char Recv[256];
} ASYNC_COMM_CONTEXT;

int uart_init(XUartPs* uart_ps);
int uart_intr_init(XScuGic *intc, XUartPs *uart_ps);
void yesense_intr_handler(void *CallbackRef);

#endif
