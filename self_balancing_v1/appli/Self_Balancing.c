/*
 * Self_Balancing.c
 *
 *  Created on: 7 juil. 2020
 *      Author: Théo
 */

#include "Self_Balancing.h"
#include "Low_lvl.h"

void Self_Balancing_init(Self_Balancing_t * self_b){
	//init des valeurs
	self_b->soft.loop_time = 0 ;
	self_b->soft.state_low_lvl = WAIT ;

	DRONE_mpu6050_init(&self_b->sensors.imu,MPU6050_Accelerometer_16G, MPU6050_Gyroscope_500s, 0.999, LOW_LVL_FREQUENCY );
}

void Self_Balancing_procces_main(Self_Balancing_t * self_b){

	switch(self_b->soft.state_low_lvl){
		case WAIT:
			LOW_LVL_Wait(self_b);
			break;

		case UPDATE_IMU:
			LOW_LVL_Update_imu(self_b);
			break;

		default :
			break;
	}
}

