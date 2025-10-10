#ifndef TRACK_H
#define TRACK_H

#include <stdio.h>
#include <stdlib.h>
#include "xil_io.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xscugic.h"
#include "xil_util.h"
#include "xplatform_info.h"

#include "DA.h"
#include "Elmo_Can.h"
#include "centroid.h"


#define INTC_GIC_ID		XPAR_SCUGIC_0_DEVICE_ID


#define INTC_FINE_INT_ID	126  //精跟踪相机中断ID

#define INTC_COARSE_INT_ID	125  //粗跟踪相机中断ID


void Track_Fine(void *CallbackRef);   //精跟踪处理函数
void Track_Coarse(void *CallbackRef); //粗跟踪处理函数

void Camera_Gic_Intc(void);


#endif
