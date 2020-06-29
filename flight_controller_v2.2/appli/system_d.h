/*
 * system_d.h
 *
 *  Created on: 29 juin 2020
 *      Author: Théo
 */


#include "../lib/lib_perso/GPS.h"
#include "stm32f1_mpu6050.h"
#include "../lib/lib_perso\complementary_filter.h"
#include "settings.h"
#include "macro_types.h"
#include "MAE.h"
#include "../lib/lib_perso\esc.h"

#ifndef SYSTEM_D_H_
#define SYSTEM_D_H_

// Struct du drone
typedef struct{

	//Gps
	gps_datas_t gps ;
	bool_e gps_is_ok  ;

	//Batterie
	double v_bat ;

	//Consignes
	double roll_consigne ;
	double pitch_consigne ;
	double yaw_consigne ;
	double throttle_consigne ;

	// PWM escs signal variables
	ESC_e escs[4];

	//PPM variables
	uint16_t channels [NB_CHANNEL] ;
	bool_e ppm_is_ok  ;

	//MPU 6050 variables
	MPU6050_t mpu_data ;
	MPU6050_Result_t mpu_result ;
	bool_e mpu_is_ok  ;
	COMP_FILTER_angles_e mpu_angles ;

	//Flight mode
	Flight_Mode_SM state_flight_mode ;


}State_drone_t;

#endif /* SYSTEM_D_H_ */
