#ifndef ELMO_CAN_H
#define ELMO_CAN_H


#include "xparameters.h"
#include "xuartps.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "xcanps.h"
#include "Elmo_Sdo.h"
#include "sleep.h"



#define INTC XScuGic    //中断寄存器
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID //中断控制器ID
#define Can_Elmo_DEVICE_ID		XPAR_PSU_CAN_1_DEVICE_ID   //CAN0 ID
#define Can_Elmo_INTR_VEC_ID		XPAR_XCANPS_1_INTR        //CAN中断ID

//CAN接收与发送数据组
#define XCANPS_MAX_FRAME_SIZE_IN_WORDS (XCANPS_MAX_FRAME_SIZE / sizeof(u32))
//SDO命令有关定义
#define DATA_LEN 8 //8/4
/*
输入时钟100M
波特率计算公式：freq = F/{(CAN_BRPR+1)*(BTR_SYNCJUMPWIDTH + BTR_SECOND_TIMESEGMENT + BTR_FIRST_TIMESEGMENT)}

500khZ

*/
#define CAN_BRPR                49  //设置预分辨率为29efine
#define BTR_SYNCJUMPWIDTH       3   //设置同步跳转宽度
#define BTR_SECOND_TIMESEGMENT	2   //设置相位缓冲段1
#define BTR_FIRST_TIMESEGMENT	15  //设置相位缓冲段2



/*
    CAN初始化函数
*/

#define ELMO_DATA_SIZE 524287/360.0   //电机编码器位数
#define ELMO_SIZE   524287

#define MOTO_X_Speed_ID 769   //X轴速度PDOID
#define MOTO_Y_Speed_ID 770   //Y轴速度PDOID
#define MOTO_X_Position_ID 1153   //X轴位置PDOID
#define MOTO_Y_Position_ID 1154   //Y轴位置PDOID



int Can_Config(void);

void Can_RecvHandler(void *CallBackRef); //CAN接收中断函数

void Can_SendData_Sdo(XCanPs *CanInstPtr,u32 Send_ID,SDO_ORD Send_Ord,int32_t Send_Order); //CAN发送Sdo配置函数

void Can_SendData(XCanPs *CanInstPtr,u32 Send_ID,int32_t Send_Data);  //CAN发送数据函数

void Elmo_Config(void); //初始化ELMO，配置SDO

void Elmo_Speed_Model(void); //设置电机为速度模式

void Elmo_Position_Model(void); //设置电机为位置模式

void Elmo_StopServe(void);  //电机失能

void Elmo_StartServe(void); //电机使能

void Elmo_Set_Speed(u32 Motor_Number,double Speed_Data); //设置电机速度

void Elmo_Set_Position(u32 Motor_Number,double Position_Data); //设置电机位置

void Elmo_Position_Model_X(void);//设置X轴电机为位置模式

void Elmo_Position_Model_Y(void);//设置Y轴电机为位置模式





#endif
