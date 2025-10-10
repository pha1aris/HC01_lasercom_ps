#include "uart_dw.h"
#include "Camera_Set.h"

XScuGic Intc_dw_uart;              //中断控制器驱动程序实例
XUartPs Uart_Ps_dw;           //串口驱动程序实例

//迪文屏接收数据

u8 Dw_Read_Data[100];  //接收数据数组
u8 Dw_Read_Data_Len = 0; //接收数据长度
u8 Dw_Read_Data_Cnt = 0;
u8 Dw_Read_Data_valid = 0; //数据有效信号



/*
 * 精跟踪相机默认参数
 */
extern u32 Fine_ROI_Width 		;
extern u32 Fine_ROI_Height		;
extern u32 Fine_ROI_Width_Par	;
extern u32 Fine_ROI_Height_Par	;

/*
 * 粗跟踪相机默认参数
 */
extern u32 Coarse_ROI_Width 		;
extern u32 Coarse_ROI_Height		;
extern u32 Coarse_ROI_Width_Par		;
extern u32 Coarse_ROI_Height_Par	;


//UART初始化函数
int uart_dw_init(XUartPs* uart_ps,u32 BaudRate)
{
    int status;
    XUartPs_Config *uart_cfg;

    uart_cfg = XUartPs_LookupConfig(UART_PS0_DEVICE_ID);
    if (NULL == uart_cfg)
        return XST_FAILURE;
    status = XUartPs_CfgInitialize(uart_ps, uart_cfg, uart_cfg->BaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    //UART设备自检
    status = XUartPs_SelfTest(uart_ps);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    //设置工作模式:正常模式
    XUartPs_SetOperMode(uart_ps, XUARTPS_OPER_MODE_NORMAL);
    //设置波特率
    XUartPs_SetBaudRate(uart_ps,BaudRate);
    //设置RxFIFO的中断触发等级
    XUartPs_SetFifoThreshold(uart_ps, 1);

    return XST_SUCCESS;
}

//UART中断处理函数
void uart_dw_intr_handler(void *call_back_ref)
{
    XUartPs *uart_instance_ptr = (XUartPs *) call_back_ref;
    u8 rec_data = 0 ;
    u32 isr_status ;                           //中断状态标志

    //读取中断ID寄存器，判断触发的是哪种中断
    isr_status = XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,
                   XUARTPS_IMR_OFFSET);
    isr_status &= XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,
                   XUARTPS_ISR_OFFSET);

    //判断中断标志位RxFIFO是否触发
    if (isr_status & (u32)XUARTPS_IXR_RXOVR){
    	rec_data = XUartPs_Recv(uart_instance_ptr,Dw_Read_Data + Dw_Read_Data_Cnt,1);
    	if(*Dw_Read_Data == 0x5A)
    	{
    		Dw_Read_Data_Cnt += rec_data;
    		if(Dw_Read_Data_Cnt > 1)
    		{
    			if(*(Dw_Read_Data + 1) == 0xA5)
    			{
    				if(Dw_Read_Data_Cnt == (*(Dw_Read_Data + 2) + 3))
    				{
    					Dw_Read_Data_Len = Dw_Read_Data_Cnt;
    					Dw_Read_Data_Cnt = 0;
    					Dw_Read_Data_valid = 1;
    				}
    				else if(Dw_Read_Data_Cnt > (*(Dw_Read_Data + 2) + 3))
    				{
    					Dw_Read_Data_Cnt = 0;
    					Dw_Read_Data_valid = 0;
    					//memset(Dw_Read_Data,0,sizeof(Dw_Read_Data)*sizeof(Dw_Read_Data[0]));
    				}
    			}
    			else
    			{
    				Dw_Read_Data_Cnt = 0;
    				//memset(Dw_Read_Data,0,sizeof(Dw_Read_Data)*sizeof(Dw_Read_Data[0]));
    			}
    		}
    	}
    	else
    	{
    		Dw_Read_Data_Cnt = 0;
    		//memset(Dw_Read_Data,0,sizeof(Dw_Read_Data)*sizeof(Dw_Read_Data[0]));
    	}

    }
    //清除中断标志
     XUartPs_WriteReg(uart_instance_ptr->Config.BaseAddress,
                    XUARTPS_ISR_OFFSET, XUARTPS_IXR_RXOVR) ;
}

//串口中断初始化
int uart_dw_intr_init(XScuGic *intc, XUartPs *uart_ps)
{
    int status;
    //初始化中断控制器
    XScuGic_Config *intc_cfg;
    intc_cfg = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == intc_cfg)
        return XST_FAILURE;
    status = XScuGic_CfgInitialize(intc, intc_cfg,
            intc_cfg->CpuBaseAddress);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    //设置并打开中断异常处理功能
   Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
            (Xil_ExceptionHandler)XScuGic_InterruptHandler,
            (void *)intc);
    Xil_ExceptionEnable();

    //为中断设置中断处理函数
    XScuGic_Connect(intc, UART_PS0_INT_IRQ_ID,
            (Xil_ExceptionHandler) uart_dw_intr_handler,(void *) uart_ps);
    //设置UART的中断触发方式
    XUartPs_SetInterruptMask(uart_ps, XUARTPS_IXR_RXOVR);
    //使能GIC中的串口中断
    XScuGic_Enable(intc, UART_PS0_INT_IRQ_ID);
    return XST_SUCCESS;
}
//发送数据
void uart_dw_send_data(XUartPs* uart_ps, u8* data, u32 length)
{
	for (u32 i = 0; i < length; ++i) {
		XUartPs_SendByte(uart_ps->Config.BaseAddress, data[i]);
	}
}

//初始化迪文屏串口
void Dw_Uart_Config()
{
	uart_dw_init(&Uart_Ps_dw,115200);
	//uart_dw_intr_init(&Intc_dw_uart, &Uart_Ps_dw);
}

//迪文屏读数据
void Dw_Data_Read(u8 *data_in,u16 *Out_Addr,int32_t *Out_data)
{
	*Out_data  = 0;
	*Out_Addr = ((u16)*(data_in+4) << 8) +	*(data_in+5);
	if(*(data_in+6) == 0x01)
	{
		*Out_data = (int32_t)(((u16)*(data_in+7) << 8) + *(data_in+8));
	}
	else if(*(data_in+6) == 0x02)
	{
		*Out_data = (int32_t)(((u32)*(data_in+7) << 24) + ((u32)*(data_in+8)<<16) + ((u32)*(data_in+9) << 8) + *(data_in+10));
	}

}
//迪文屏写数据
void Dw_Data_Write(u16 Write_Addr,int32_t Write_Data,u8 Write_Len)
{

	u32 send_data_len =Write_Len + 6;

	u8 send_data[10];
	send_data[0] = 0x5A;
	send_data[1] = 0xA5;
	send_data[2] = Write_Len + 3;
	send_data[3] = 0x82;

	send_data[4] = Write_Addr >> 8;
	send_data[5] = Write_Addr;

	if(Write_Len == 2)
	{
		send_data[6] = Write_Data >> 8;
		send_data[7] = Write_Data;
	}
	else if(Write_Len == 4)
	{
		send_data[6] = Write_Data >> 24;
		send_data[7] = Write_Data >> 16;
		send_data[8] = Write_Data >> 8;
		send_data[9] = Write_Data;
	}

	uart_dw_send_data(&Uart_Ps_dw, send_data, send_data_len);
}

//显示精跟踪相机光斑
void Dw_Fine_Spot_P(double Spot_X,double Spot_Y)
{

	int Fine_Show_X_Start = 15;
	int Fine_Show_X_Stop = 327;

	int Fine_Show_Y_Start = 114;
	int Fine_Show_Y_Stop = 332;

	u16 Fine_Show_Spot_X = 0;
	u16 Fine_Show_Spot_Y = 0;

	Fine_Show_Spot_X = Spot_X/Fine_ROI_Width*(Fine_Show_X_Stop - Fine_Show_X_Start) + Fine_Show_X_Start;
	Fine_Show_Spot_Y = Spot_Y/Fine_ROI_Height*(Fine_Show_Y_Stop - Fine_Show_Y_Start) + Fine_Show_Y_Start;

	u32 send_data_len = 18;
	u8 send_data[18];

	send_data[0] = 0x5A;
	send_data[1] = 0xA5;
	send_data[2] = 0x0F;
	send_data[3] = 0x82;
	send_data[4] = 0x33;
	send_data[5] = 0x33;
	send_data[6] = 0x29;
	send_data[7] = 0x07;
	send_data[8] = 0x00;
	send_data[9] = 0x01;
	send_data[10] = Fine_Show_Spot_X >> 8;
	send_data[11] = Fine_Show_Spot_X;
	send_data[12] = Fine_Show_Spot_Y >> 8;
	send_data[13] = Fine_Show_Spot_Y;
	send_data[14] = 0x00;
	send_data[15] = 0x27;
	send_data[16] = 0xFF;
	send_data[17] = 0x00;
	uart_dw_send_data(&Uart_Ps_dw, send_data, send_data_len);

}

//显示粗跟踪相机光斑
void Dw_Coarse_Spot_P(double Spot_X,double Spot_Y)
{

	int Fine_Show_X_Start = 701;
	int Fine_Show_X_Stop = 1013;

	int Fine_Show_Y_Start = 114;
	int Fine_Show_Y_Stop = 332;

	u16 Fine_Show_Spot_X = 0;
	u16 Fine_Show_Spot_Y = 0;

	Fine_Show_Spot_X = Spot_X/Coarse_ROI_Width*(Fine_Show_X_Stop - Fine_Show_X_Start) + Fine_Show_X_Start;
	Fine_Show_Spot_Y = Spot_Y/Coarse_ROI_Height*(Fine_Show_Y_Stop - Fine_Show_Y_Start) + Fine_Show_Y_Start;

	u32 send_data_len = 18;
	u8 send_data[18];

	send_data[0] = 0x5A;
	send_data[1] = 0xA5;
	send_data[2] = 0x0F;
	send_data[3] = 0x82;
	send_data[4] = 0x22;
	send_data[5] = 0x22;
	send_data[6] = 0x29;
	send_data[7] = 0x07;
	send_data[8] = 0x00;
	send_data[9] = 0x01;
	send_data[10] = Fine_Show_Spot_X >> 8;
	send_data[11] = Fine_Show_Spot_X;
	send_data[12] = Fine_Show_Spot_Y >> 8;
	send_data[13] = Fine_Show_Spot_Y;
	send_data[14] = 0x00;
	send_data[15] = 0x27;
	send_data[16] = 0xFF;
	send_data[17] = 0x00;
	uart_dw_send_data(&Uart_Ps_dw, send_data, send_data_len);

}



