#ifndef DATALINK_H
#define DATALINK_H

#include "uart16550.h"
#include "EDFA_Control.h"

// 数传电台数据发送
void Uartplsend_datalink(XUartNs550 *InstancePtr,float latitude, float longitude, float altitude);

#endif
