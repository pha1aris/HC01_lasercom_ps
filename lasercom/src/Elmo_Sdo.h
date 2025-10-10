#ifndef ELMO_SDO_H
#define ELMO_SDO_H

#include "xil_types.h"
//CAN通信命令结构体
struct SDO_ORD{
    u16 index;
    u8 subindex;
    inline SDO_ORD(u16 i, u8 s) : index(i),subindex(s){}
};
//电机ID
#define MOTO_X_ID 1
#define MOTO_Y_ID 2

//Elmo Sdo有关命令设置
const u8 NMT_stop = 0x02;
const u8 NMT_start = 0x01;
const u8 NMT_reset = 0x81;

const SDO_ORD controlword(0x6040, 0x0);
const SDO_ORD statusword(0x6041, 0x0);
const SDO_ORD modes_of_operation(0x6060, 0x0);
const SDO_ORD modes_of_operation_display(0x6061, 0x0);
const SDO_ORD target_velocity(0x60ff, 0x0);
const SDO_ORD target_torque(0x6071, 0x0);
const SDO_ORD target_position(0x607a, 0x0);
const SDO_ORD actual_position(0x6064, 0x0);
const SDO_ORD profile_velocity(0x6081, 0);
const SDO_ORD max_profile_velocity(0x607f, 0);
const SDO_ORD max_motor_speed(0x6080, 0);
const SDO_ORD max_acceleration(0x60c5, 0x0);
const SDO_ORD profile_acceleration(0x6083, 0x0);
const SDO_ORD profile_deceleration(0x6084, 0x0);
const SDO_ORD home_offset(0x607c, 0x0);
const SDO_ORD home_method(0x6098, 0x0);
const SDO_ORD home_speed(0x6099, 0x2);
const SDO_ORD home_acceleration(0x609a, 0x0);
const SDO_ORD acceleration(0x6083, 0x0);
const SDO_ORD deceleration(0x6084, 0x0);
const SDO_ORD max_velocity(0x607f, 0x0);
const SDO_ORD rpdo1_number_of_entries(0x1400, 0x0);
const SDO_ORD rpdo1_cobid(0x1400, 0x1);
const SDO_ORD rpdo1_transmission_type(0x1400, 0x2);
const SDO_ORD rpdo2_number_of_entries(0x1401, 0x0);
const SDO_ORD rpdo2_cobid(0x1401, 0x1);
const SDO_ORD rpdo2_transmission_type(0x1401, 0x2);
const SDO_ORD rpdo3_number_of_entries(0x1402, 0x0);
const SDO_ORD rpdo3_cobid(0x1402, 0x1);
const SDO_ORD rpdo3_transmission_type(0x1402, 0x2);
const SDO_ORD rpdo4_number_of_entries(0x1403, 0x0);
const SDO_ORD rpdo4_cobid(0x1403, 0x1);
const SDO_ORD rpdo4_transmission_type(0x1403, 0x2);
const SDO_ORD rpdo1_number_of_mapped(0x1600, 0x0);
const SDO_ORD rpdo1_mapping_object1(0x1600, 0x1);
const SDO_ORD rpdo1_mapping_object2(0x1600, 0x2);
const SDO_ORD rpdo2_number_of_mapped(0x1601, 0x0);
const SDO_ORD rpdo2_mapping_object1(0x1601, 0x1);
const SDO_ORD rpdo2_mapping_object2(0x1601, 0x2);
const SDO_ORD rpdo3_number_of_mapped(0x1602, 0x0);
const SDO_ORD rpdo3_mapping_object(0x1602, 0x1);
const SDO_ORD rpdo4_number_of_mapped(0x1603, 0x0);
const SDO_ORD rpdo4_mapping_object(0x1603, 0x1);
const SDO_ORD tpdo1_number_of_entries(0x1800, 0x0);
const SDO_ORD tpdo1_cobid(0x1800, 0x1);
const SDO_ORD tpdo1_transmission_type(0x1800, 0x2);
const SDO_ORD tpdo1_event_timer(0x1800, 0x5);
const SDO_ORD tpdo2_number_of_entries(0x1801, 0x0);
const SDO_ORD tpdo2_cobid(0x1801, 0x1);
const SDO_ORD tpdo2_transmission_type(0x1801, 0x2);
const SDO_ORD tpdo2_event_timer(0x1801, 0x5);
const SDO_ORD tpdo3_number_of_entries(0x1802, 0x0);
const SDO_ORD tpdo3_cobid(0x1802, 0x1);
const SDO_ORD tpdo3_transmission_type(0x1802, 0x2);
const SDO_ORD tpdo3_event_timer(0x1802, 0x5);
const SDO_ORD tpdo4_number_of_entries(0x1803, 0x0);
const SDO_ORD tpdo4_cobid(0x1803, 0x1);
const SDO_ORD tpdo4_transmission_type(0x1803, 0x2);
const SDO_ORD tpdo4_event_timer(0x1803, 0x5);
const SDO_ORD tpdo1_number_of_mapped(0x1a00, 0x0);
const SDO_ORD tpdo1_mapping_object1(0x1a00, 0x1);
const SDO_ORD tpdo1_mapping_object2(0x1a00, 0x2);
const SDO_ORD tpdo2_number_of_mapped(0x1a01, 0x0);
const SDO_ORD tpdo2_mapping_object(0x1a01, 0x1);
const SDO_ORD tpdo3_number_of_mapped(0x1a02, 0x0);
const SDO_ORD tpdo3_mapping_object(0x1a02, 0x1);
const SDO_ORD tpdo4_number_of_mapped(0x1a03, 0x0);
const SDO_ORD tpdo4_mapping_object1(0x1a03, 0x1);
const SDO_ORD tpdo4_mapping_object2(0x1a03, 0x2);
const SDO_ORD interpolation_time_units(0x60c2, 0x1);
const SDO_ORD interpolation_time_index(0x60c2, 0x2);


const u8 modes_of_operation_homing_mode = 0x6;
const u8 modes_of_operation_profile_position_mode = 0x1;
const u8 modes_of_operation_velocity_mode = 0x2;
const u8 modes_of_operation_profile_velocity_mode = 0x3;
const u8 modes_of_operation_torque_profile_mode = 0x4;
const u8 modes_of_operation_interpolated_position_mode = 0x7;
const u8 modes_of_operation_cyclic_synchronous_velocity_mode = 0x9;
const u8 modes_of_operation_cyclic_synchronous_position_mode = 0x8;
const u16 controlword_shutdown = 6;
const u16 controlword_switch_on = 7;
const u16 controlword_start_homing = 16;
const u16 controlword_enable_operation = 15;


#endif
