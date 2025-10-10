#include "Elmo_Can.h"
#include <sleep.h>
#include "uart_dw.h"
#include "Dw_Data_Define.h"

u32 MOTO_X_Speed_ID_Send;
u32 MOTO_Y_Speed_ID_Send;
u32 MOTO_X_Position_ID_Send;
u32 MOTO_Y_Position_ID_Send;

u32 Can_Elmo_TxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
u32 Can_Elmo_RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];

float Motor_Y_Position;
float Motor_X_Position;

XCanPs Can_Elmo;    /* CAN驱动器 */



//初始化CAN
int Can_Config()
{
    int Status;

    XCanPs_Config *ConfigPtr = NULL;

    ConfigPtr = XCanPs_LookupConfig(Can_Elmo_DEVICE_ID);  //查询对应设备
    if (ConfigPtr == NULL) {
		return FALSE;
	}
    Status = XCanPs_CfgInitialize(&Can_Elmo,ConfigPtr,ConfigPtr->BaseAddr); //初始化CAN设备
    if (Status != XST_SUCCESS) {
    		return FALSE;
    }

    //CAN设备自检
    Status = XCanPs_SelfTest(&Can_Elmo);
	if (Status != XST_SUCCESS) {
		return FALSE;
	}


    //将CAN进入配置模式
    XCanPs_EnterMode(&Can_Elmo, XCANPS_MODE_CONFIG);
     usleep(1000);
    //等待进入配置模式
    while (XCanPs_GetMode(&Can_Elmo) != XCANPS_MODE_CONFIG);

    //设置CAN通信波特率
    XCanPs_SetBaudRatePrescaler(&Can_Elmo, CAN_BRPR);  //设置预分频值
    XCanPs_SetBitTiming(&Can_Elmo, BTR_SYNCJUMPWIDTH,
			    BTR_SECOND_TIMESEGMENT,
			    BTR_FIRST_TIMESEGMENT);

    //设置CAN中断函数
    XCanPs_SetHandler(&Can_Elmo, XCANPS_HANDLER_RECV,
			  (void *)Can_RecvHandler, (void *)&Can_Elmo);

    XCanPs_IntrEnable(&Can_Elmo, XCANPS_IXR_RXNEMP_MASK);  //在CAN设备中使能所有中断
    XCanPs_EnterMode(&Can_Elmo, XCANPS_MODE_NORMAL);
    while(XCanPs_GetMode(&Can_Elmo) != XCANPS_MODE_NORMAL);

	return Status;

}

//CAN通信SDO数据发送函数
void Can_SendData_Sdo(XCanPs *CanInstPtr,u32 Send_ID,SDO_ORD Send_Ord,int32_t Send_Order)
{
    u32 Write_ID; 
    u8 *FramePtr;  
    Write_ID = Send_ID + 0x600;
    //Write_ID = Send_ID;
    Can_Elmo_TxFrame[0] = (u32)XCanPs_CreateIdValue((u32)Write_ID, 0, 0, 0, 0);
    //Can_Elmo_TxFrame[0] = Write_ID;
	Can_Elmo_TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)8);

    // Can_Elmo_TxFrame[2] = 0x22000000 + ( (u32)Send_Ord.index << 8) + Send_Ord.subindex;
    // Can_Elmo_TxFrame[3] = Send_Order;
    
    FramePtr = (u8 *)(&Can_Elmo_TxFrame[2]);
    *FramePtr = 0x22;
    *(FramePtr+1) = Send_Ord.index & 0xff;
    *(FramePtr+2) =(Send_Ord.index >> 8) & 0xff;
    *(FramePtr+3) = Send_Ord.subindex;
    *(FramePtr+4) = Send_Order & 0xff;
    *(FramePtr+5) =(Send_Order >> 8) & 0xff;
    *(FramePtr+6) = (Send_Order >> 16) & 0xff;
    *(FramePtr+7) = (Send_Order >> 24) & 0xff;

   //xil_printf("Can_Elmo_TxFrame[1] = %d\n", Can_Elmo_TxFrame[1]);

    XCanPs_Send(CanInstPtr, Can_Elmo_TxFrame);

}

//CAN通信发送数据
void Can_SendData(XCanPs *CanInstPtr,u32 Send_ID,int32_t Send_Data)
{
    u32 Write_ID;   
    u8 *FramePtr;  
     Write_ID = Send_ID + 0x200;
   
    Can_Elmo_TxFrame[0] = (u32)XCanPs_CreateIdValue((u32)Write_ID, 0, 0, 0, 0);
	Can_Elmo_TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)4);

    FramePtr = (u8 *)(&Can_Elmo_TxFrame[2]);
    *FramePtr =     Send_Data & 0xff;
    *(FramePtr+1) = (Send_Data >> 8) & 0xff;
    *(FramePtr+2) = (Send_Data >> 16) & 0xff;
    *(FramePtr+3) = (Send_Data >> 24) & 0xff;

    XCanPs_Send(CanInstPtr, Can_Elmo_TxFrame);
}

//CAN通信接收中断
void Can_RecvHandler(void *CallBackRef)
{
    XCanPs *CanPtr_Rec = (XCanPs *)CallBackRef;
    XCanPs_Recv(CanPtr_Rec, Can_Elmo_RxFrame); //读取数据
    double data_set = 0;
    /*
        下面进行数据处理
    */
    if( Can_Elmo_RxFrame[0] == MOTO_X_Speed_ID_Send)
    {
        Can_Elmo_RxFrame[0] = MOTO_X_Speed_ID;
    }
    else if( Can_Elmo_RxFrame[0] == MOTO_Y_Speed_ID_Send)
    {
        Can_Elmo_RxFrame[0] = MOTO_Y_Speed_ID;
    }
    else if( Can_Elmo_RxFrame[0] == MOTO_X_Position_ID_Send)
    {
        Can_Elmo_RxFrame[0] = MOTO_X_Position_ID;
        data_set = (int32_t)Can_Elmo_RxFrame[2];
        Motor_X_Position = data_set/ELMO_SIZE*360;


    }
    else if( Can_Elmo_RxFrame[0] == MOTO_Y_Position_ID_Send)
    {
        Can_Elmo_RxFrame[0] = MOTO_Y_Position_ID;
        data_set = (int32_t)Can_Elmo_RxFrame[2];
        Motor_Y_Position = data_set/ELMO_SIZE*360;

    }

    Can_Elmo_RxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)8);

}

//初始化ELMO

//ELMO SDO初始化
void Elmo_Config()
{

    //获取ID编号
    MOTO_X_Speed_ID_Send = (u32)XCanPs_CreateIdValue((u32)MOTO_X_Speed_ID, 0, 0, 0, 0);
    MOTO_Y_Speed_ID_Send = (u32)XCanPs_CreateIdValue((u32)MOTO_Y_Speed_ID, 0, 0, 0, 0);
    MOTO_X_Position_ID_Send = (u32)XCanPs_CreateIdValue((u32)MOTO_X_Position_ID, 0, 0, 0, 0);
    MOTO_Y_Position_ID_Send = (u32)XCanPs_CreateIdValue((u32)MOTO_Y_Position_ID, 0, 0, 0, 0);


    u8 *FramePtr_text; 
    Can_Elmo_TxFrame[0] = (u32)XCanPs_CreateIdValue((u32)0, 0, 0, 0, 0);
    Can_Elmo_TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)2);
    FramePtr_text =  (u8 *)(&Can_Elmo_TxFrame[2]);
    *FramePtr_text = 0x01;
    *(FramePtr_text+1) = 0x00;
    XCanPs_Send(&Can_Elmo, Can_Elmo_TxFrame);
    usleep(10000);

     //X轴位置
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_cobid,0x80000482);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_number_of_mapped,0);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_transmission_type,255);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_mapping_object1,0x60640020);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_mapping_object2,0x606c0020);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_number_of_mapped,2);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_event_timer,100);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,tpdo4_cobid,MOTO_X_Position_ID);
     usleep(10000);
    //Y轴位置
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_cobid,0x80000482);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_number_of_mapped,0);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_transmission_type,255);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_mapping_object1,0x60640020);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_mapping_object2,0x606c0020);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_number_of_mapped,2);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_event_timer,100);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,tpdo4_cobid,MOTO_Y_Position_ID);
    usleep(10000);

    // 配置速度为RPDO
   // X轴
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo1_cobid,0x80000201);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo1_number_of_mapped,0);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo1_mapping_object1,0x60ff0020);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo1_number_of_mapped,1);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo1_cobid,0x201);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_cobid,0x80000301);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_number_of_mapped,0);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_mapping_object1,0x607a0020);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_mapping_object2,0x60400010);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_number_of_mapped,2);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,rpdo2_cobid,MOTO_X_Speed_ID);
     usleep(10000);
    //Y轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo1_cobid,0x80000202);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo1_number_of_mapped,0);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo1_mapping_object1,0x60ff0020);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo1_number_of_mapped,1);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo1_cobid,0x202);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_cobid,0x80000302);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_number_of_mapped,0);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_mapping_object1,0x607a0020);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_mapping_object2,0x60400010);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_number_of_mapped,2);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,rpdo2_cobid,MOTO_Y_Speed_ID);
    usleep(10000);

}

//速度模式设置
void Elmo_Speed_Model()
{

     //X轴
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,profile_acceleration,1000 * ELMO_DATA_SIZE);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,profile_deceleration,1000 * ELMO_DATA_SIZE);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,modes_of_operation,modes_of_operation_profile_velocity_mode);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,max_profile_velocity,800 * ELMO_DATA_SIZE);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_shutdown);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_switch_on);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_enable_operation);
     usleep(10000);
     Can_SendData(&Can_Elmo,MOTO_X_ID,0);
     usleep(10000);

    //Y轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,profile_acceleration,1000 * ELMO_DATA_SIZE);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,profile_deceleration,1000 * ELMO_DATA_SIZE);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,modes_of_operation,modes_of_operation_profile_velocity_mode);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,max_profile_velocity,800 * ELMO_DATA_SIZE);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_shutdown);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_switch_on);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_enable_operation);
    usleep(10000);
    Can_SendData(&Can_Elmo,MOTO_Y_ID,0);
    usleep(10000);
}

//位置模式设置
void Elmo_Position_Model()
{
    //X轴
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_shutdown);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_switch_on);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,modes_of_operation,modes_of_operation_profile_position_mode);
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,profile_velocity,300 * ELMO_DATA_SIZE); //最大速度
     usleep(10000);
     Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_enable_operation);
    usleep(10000);
    //Y轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_shutdown);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_switch_on);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,modes_of_operation,modes_of_operation_profile_position_mode);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,profile_velocity,30 * ELMO_DATA_SIZE); //最大速度
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_enable_operation);
    usleep(10000);
}

//设置X轴电机为位置模式
void Elmo_Position_Model_X(void)
{
	//X轴
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_shutdown);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_switch_on);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,modes_of_operation,modes_of_operation_profile_position_mode);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,profile_velocity,300 * ELMO_DATA_SIZE); //最大速度
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_enable_operation);
	usleep(10000);
}

//设置Y轴电机为位置模式
void Elmo_Position_Model_Y(void)
{
	//Y轴
	Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_shutdown);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_switch_on);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,modes_of_operation,modes_of_operation_profile_position_mode);
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,profile_velocity,30 * ELMO_DATA_SIZE); //最大速度
	usleep(10000);
	Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_enable_operation);
	usleep(10000);
}


//电机失能
void Elmo_StopServe()
{
    //X轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_shutdown);
    //Y轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_shutdown);
}

//电机使能
void Elmo_StartServe()
{
    //X轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_X_ID,controlword,controlword_enable_operation);
    //Y轴
    Can_SendData_Sdo(&Can_Elmo,MOTO_Y_ID,controlword,controlword_enable_operation);
}
/*
设置电机速度
*/
void Elmo_Set_Speed(u32 Motor_Number,double Speed_Data)
{
	int32_t Speed_Data_Set;
	Speed_Data_Set = Speed_Data * ELMO_DATA_SIZE;
    Can_SendData(&Can_Elmo,Motor_Number,Speed_Data_Set);
}
//设置电机位置
void Elmo_Set_Position(u32 Motor_Number,double Position_Data)
{
    Position_Data = Position_Data * ELMO_DATA_SIZE;    

    Can_SendData_Sdo(&Can_Elmo,Motor_Number,modes_of_operation,modes_of_operation_profile_position_mode);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,profile_velocity,100*ELMO_DATA_SIZE);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,controlword,controlword_shutdown);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,controlword,controlword_switch_on);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,target_position,(int32_t)Position_Data);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,controlword,controlword_enable_operation);
    usleep(10000);
    Can_SendData_Sdo(&Can_Elmo,Motor_Number,controlword,31);
    usleep(10000);
}
