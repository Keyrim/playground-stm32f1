/*
 * Low_lvl.c
 *
 *  Created on: 8 juil. 2020
 *      Author: Théo
 */

#include "Low_lvl.h"
#include "systick.h"


void LOW_LVL_Wait(Self_Balancing_t * self_b){
	uint32_t time = SYSTICK_get_time_us();
	if(time > self_b->soft.loop_time){
		self_b->soft.state_low_lvl = UPDATE_IMU ;
		self_b->soft.loop_time += LOW_LVL_PERIODE ;
	}

}
void LOW_LVL_Update_imu(Self_Balancing_t * self_b){
	MPU6050_ReadAll(&self_b->sensors.imu.raw_data_mpu);
	DRONE_mpu6050_update_angles(&self_b->sensors.imu);

}
