#ifndef LOCATION_COMPUTE_H
#define LOCATION_COMPUTE_H

#include "stdio.h"
#include "math.h"


#define a 6378137 //地球椭球长半径
#define e 0.006694379995 //第一偏心率平方


void Initial_Point(double log_M,double lat_M,double ele_M,
				   double log_S,double lat_S,double ele_S,
				   double cou_M,double pit_M,double rol_M,
				   double *cou_MS,double *pit_MS);//位置解算函数


#endif
