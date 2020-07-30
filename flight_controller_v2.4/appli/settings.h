/*
 * settings.h
 *
 *  Created on: 28 mars 2020
 *      Author: Theo
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

//radio settings
#define TIME_OUT_IBUS 20000 	//(en micros seconde)
#define NB_CHANNEL 8
#define PITCH 1
#define ROLL 0
#define THROTTLE 2
#define YAW 3
#define SWITCH_1 4
#define SWITCH_2 5
#define SWITCH_3 6
#define SWITCH_4 7



//Gps settings
#define TIME_OUT_GPS 2000000 //(en microsecondes)
#define UART_GPS UART2_ID
#define GPS_RELAY	FALSE

//Main loop settings
#define LOOP_PERIODE 4000

//PWM Settings
#define PULSATION_MIN 1000
#define PULSATION_MAX 2000

//pid max setpoint order (in degrees or degrees per second for the yaw)
#define roll_and_pitch_max_setpoint 70
#define yaw_max_setpoint 400

//PID settings
#define kp_roll 				1.5 //2.94
#define ki_roll 				0
#define kd_roll 				0.2 //0.27
#define alpha_roll 				0.1517386723
#define beta_roll				1.506849315
#define gamma_roll				-0.6585879874

#define kp_pitch 				2.47 //2.94
#define ki_pitch 				0
#define kd_pitch 				0.33 //0.27
#define alpha_pitch 			0.1517386723
#define beta_pitch				1.506849315
#define gamma_pitch				-0.6585879874

#define kp_yaw 					6
#define ki_yaw 					0
#define kd_yaw 					2
#define alpha_yaw				0.1
#define beta_yaw				0.9
#define gamma_yaw				0

#define PID_MAX_OUTPUT 250

//Batterie
#define USE_BATTERIE_CHECK FALSE
#define BATTERIE_RESISTANCES_COEF ((double)0.00550)

#endif /* SETTINGS_H_ */
