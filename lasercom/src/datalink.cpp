#include "datalink.h"


// 协议格式 AA 55 [Data Length] [Pitch] [Roll] [Yaw]  [CRC]


// 己方航姿数据打包发送pl uart
void Uartplsend_datalink(XUartNs550 *InstancePtr,float latitude, float longitude, float altitude)
{

    u8 buffer[20];

    int32_t latitude_datalink = (int32_t)(latitude * 1000000);
    int32_t longitude_datalink = (int32_t)(longitude * 1000000);
    int32_t altitude_datalink = (int32_t)(altitude * 1000000);

 // 将数据填充到buffer中

    buffer[0] = 0xAA; //帧头0
    buffer[1] = 0x55; //帧头1
    buffer[2] = 14;   //数据长度
    buffer[3] = latitude_datalink >> 24;
    buffer[4] = latitude_datalink >> 16;
    buffer[5] = latitude_datalink >> 8;
    buffer[6] = latitude_datalink;
    buffer[7] = longitude_datalink >> 24;
    buffer[8] = longitude_datalink >> 16;
    buffer[9] = longitude_datalink >> 8;
    buffer[10] = longitude_datalink;
    buffer[11] = altitude_datalink >> 24;
    buffer[12] = altitude_datalink >> 16;
    buffer[13] = altitude_datalink >> 8;
    buffer[14] = altitude_datalink;

   // 计算并填充CRC
    u16 crc = Crc16_Ccitt(buffer, 15);  // 从帧头0开始计算

    buffer[15] = crc >> 8;
    buffer[16] = crc;

    UartPlSend(InstancePtr,buffer,17);
}




