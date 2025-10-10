#include "EDFA_Control.h"




/*crc16校验代码*/
u16 Crc16_Ccitt(u8 *data,u8 data_len)
{
    u16 Crc = 0X0000;
    for (unsigned int i = 0; i < data_len; i++) 
    {
        Crc = (unsigned char)(Crc >> 8) | (Crc << 8);
        Crc ^= data[i];
        Crc ^= (unsigned char)(Crc & 0xFF) >> 4;
        Crc ^= (Crc << 8) << 4;
        Crc ^= ((Crc & 0xFF) << 4) << 1;
    }
    return Crc;
}
/*EDFA开泵*/
void EDFA_On(XUartNs550 *InstancePtr)
{
    u8 EDFA_On_Order[15] = {0x55,0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0x11,0x00,0x04,0x02,0x00,0x08,0xF7};
    UartPlSend(InstancePtr,EDFA_On_Order,15);
}
/*EDFA关泵*/
void EDFA_Off(XUartNs550 *InstancePtr)
{
    u8 EDFA_Off_Order[15] = {0x55,0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0x11,0x00,0x04,0x02,0x01,0x18,0xD6};
    UartPlSend(InstancePtr,EDFA_Off_Order,15);
}
/*读取EDFA数据*/
void EDFA_ReadAll(XUartNs550 *InstancePtr)
{
    u8 EDFA_ReadAll_Order[13] = {0x55,0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x02,0x14,0x7E};
    UartPlSend(InstancePtr,EDFA_ReadAll_Order,13);
}
/*设置输出功率*/
void EDFA_SetPower(XUartNs550 *InstancePtr,u16 SetPower)
{
    u16 Crc16_Data;
    u8 EDFA_Set_Power[15] = {0x55,0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0x12,0x00,0x04,0x00,0x00,0x80,0x47};
    EDFA_Set_Power[11] = SetPower >> 8;
    EDFA_Set_Power[12] = SetPower;
    Crc16_Data = Crc16_Ccitt(&(EDFA_Set_Power[2]),11);
    EDFA_Set_Power[13] = Crc16_Data >> 8;
    EDFA_Set_Power[14] = Crc16_Data;
    UartPlSend(InstancePtr,EDFA_Set_Power,15);

}
/*
 *设置输出电流
 */
/*设置输出功率*/
void EDFA_Set_Light_Power(XUartNs550 *InstancePtr,u16 SetPower)
{
    u16 Crc16_Data;
    u8 EDFA_Set_Power[15] = {0x55,0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0x10,0x00,0x04,0x00,0x00,0x80,0x47};
    EDFA_Set_Power[11] = SetPower >> 8;
    EDFA_Set_Power[12] = SetPower;
    Crc16_Data = Crc16_Ccitt(&(EDFA_Set_Power[2]),11);
    EDFA_Set_Power[13] = Crc16_Data >> 8;
    EDFA_Set_Power[14] = Crc16_Data;
    UartPlSend(InstancePtr,EDFA_Set_Power,15);

}




