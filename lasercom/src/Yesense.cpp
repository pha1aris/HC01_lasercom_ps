#include"Yesense.h"
#include "xuartps.h"
#include "xscugic.h"


XScuGic Intc;
XUartPs Uart_Ps;

u8 Yesense_buffer[1024];
u8 Yesense_data_num = 0;
u8 Yesense_data_cnt = 0;
u8 Yesense_data_valid = 0;
unsigned char g_recv_buf[2048] = {0};
protocol_info_t g_output_info = {0};
//static ASYNC_COMM_CONTEXT g_context;


int uart_init(XUartPs* uart_ps)
{
    int status;
    XUartPs_Config *uart_cfg;

    uart_cfg = XUartPs_LookupConfig(UART_PS1_DEVICE_ID);
    if (NULL == uart_cfg)
        return XST_FAILURE;
    status = XUartPs_CfgInitialize(uart_ps, uart_cfg, uart_cfg->BaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;


    status = XUartPs_SelfTest(uart_ps);
    if (status != XST_SUCCESS)
        return XST_FAILURE;


    XUartPs_SetOperMode(uart_ps, XUARTPS_OPER_MODE_NORMAL);

    XUartPs_SetBaudRate(uart_ps,460800);

    XUartPs_SetFifoThreshold(uart_ps, 1);

    return XST_SUCCESS;
}

void yesense_intr_handler(void *CallbackRef) {
    ASYNC_COMM_CONTEXT *context = (ASYNC_COMM_CONTEXT *)CallbackRef;
    u8 BytesRead;
    u8 buffer[1];


    while (XUartPs_IsReceiveData(context->uart_instance.Config.BaseAddress)) {
        BytesRead = XUartPs_Recv(&context->uart_instance, buffer, 1);

        if (BytesRead > 0) {
            Yesense_buffer[Yesense_data_cnt++] = buffer[0];


            if (Yesense_data_cnt == 1 && Yesense_buffer[0] != 0x59) {
                Yesense_data_cnt = 0;
                //memset(Yesense_buffer,0,sizeof(Yesense_buffer)*sizeof(Yesense_buffer[0]));
            }
            else if (Yesense_data_cnt == 2 && Yesense_buffer[1] != 0x53) {
                Yesense_data_cnt = 0;
                //memset(Yesense_buffer,0,sizeof(Yesense_buffer)*sizeof(Yesense_buffer[0]));
            }
            else if (Yesense_data_cnt > 4) {
                u8 expected_len = Yesense_buffer[4] + 7;

                if (Yesense_data_cnt == expected_len) {
                    Yesense_data_num = Yesense_data_cnt;
                    Yesense_data_cnt = 0;
                    Yesense_data_valid = 1;
                }
                else if(Yesense_data_cnt > expected_len)
                {
                	Yesense_data_cnt = 0;
                	//memset(Yesense_buffer,0,sizeof(Yesense_buffer)*sizeof(Yesense_buffer[0]));
                }
            }
        }
    }


    XUartPs_WriteReg(context->uart_instance.Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_RXOVR);
}



int uart_intr_init(XScuGic *intc, XUartPs *uart_ps)
{
    int status;

    XScuGic_Config *intc_cfg;
    intc_cfg = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == intc_cfg)
        return XST_FAILURE;
    status = XScuGic_CfgInitialize(intc, intc_cfg,
            intc_cfg->CpuBaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;


    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
            (Xil_ExceptionHandler)XScuGic_InterruptHandler,
            (void *)intc);
    Xil_ExceptionEnable();


    XScuGic_Connect(intc, UART_PS1_INT_IRQ_ID,
            (Xil_ExceptionHandler) yesense_intr_handler,(void *) uart_ps);

    XUartPs_SetInterruptMask(uart_ps, XUARTPS_IXR_RXOVR);

    XScuGic_Enable(intc, UART_PS1_INT_IRQ_ID);
    return XST_SUCCESS;
}
