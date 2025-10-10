#ifndef EDFA_CONTROL_H
#define EDFA_CONTROL_H


#include "uart16550.h"



void EDFA_On(XUartNs550 *InstancePtr);                          //EDFA开泵
void EDFA_Off(XUartNs550 *InstancePtr);                        //EDFA关泵
void EDFA_ReadAll(XUartNs550 *InstancePtr);                     //读取EDFA数据
void EDFA_SetPower(XUartNs550 *InstancePtr,u16 SetPower);     //设置EDFA功率

void EDFA_Set_Light_Power(XUartNs550 *InstancePtr,u16 SetPower);

u16 Crc16_Ccitt(u8 *data,u8 data_len);   //CRC16校验算法



#endif
