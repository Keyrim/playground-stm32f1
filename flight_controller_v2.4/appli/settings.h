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

//Pid coefs changes
#define SET_COEF_ON_RATE_PID 1


//Flags request
#define NB_FLAGS 		5
#define FLAG_CALIB_MPU 	0
#define FLAG_MANUAL_PC 	1
#define FLAG_STOP_MOTOR 2


//Gps settings
#define TIME_OUT_GPS 2000000 //(en microsecondes)
#define UART_GPS UART2_ID
#define GPS_RELAY	FALSE

//Main loop settings
#define LOOP_PERIODE 4000

//PWM Settings
#define PULSATION_MIN 1000
#define PULSATION_MAX 2000

#define SETPOINT_MAX_LVLED_PITCH_ROLL 	70
#define SETPOINT_MAX_ACCRO_PITCH_ROLL 	180
#define SETPOINT_MAX_LVLED_YAW 			150
#define SETPOINT_MAX_ACCRO_YAW			400





//Batterie
#define USE_BATTERIE_CHECK FALSE
#define BATTERIE_RESISTANCES_COEF ((double)0.00550)

#endif /* SETTINGS_H_ */
