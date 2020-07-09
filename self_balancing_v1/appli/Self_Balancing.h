/*
 * Self_Balancing.h
 *
 *  Created on: 7 juil. 2020
 *      Author: Théo
 */

#ifndef SELF_BALANCING_H_
#define SELF_BALANCING_H_

#include "../lib/lib_perso/DRONE_mpu6050.h"
#include "mae.h"
#include "branchement.h"
#include "settings.h"

typedef struct{
	DRONE_mpu6050_t imu ;
}Self_Balancing_sensors_t;

typedef struct{
	Low_Lvl_states_e state_low_lvl ;
	uint32_t loop_time ;
}Self_Balancing_soft_t;

typedef struct{
	Self_Balancing_sensors_t sensors;
	Self_Balancing_soft_t soft;
}Self_Balancing_t;


void Self_Balancing_init(Self_Balancing_t * self_b);
void Self_Balancing_procces_main(Self_Balancing_t * self_b);

#endif /* SELF_BALANCING_H_ */
