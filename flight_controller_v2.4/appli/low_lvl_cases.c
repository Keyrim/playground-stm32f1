/*
 * low_lvl_cases.c
 *
 *  Created on: 29 juin 2020
 *      Author: Théo
 */


#include "low_lvl_cases.h"

void LOW_LVL_Wait_Loop(State_drone_t * drone){

	uint32_t time = SYSTICK_get_time_us();
	if(time >= drone->soft.previous_time_loop + LOOP_PERIODE){
		drone->soft.previous_time_loop = time ;
		drone->soft.state_low_level = PWM_HIGH ;
		drone->soft.dead_line = 0 ;	//Si c'est l'heure on passe direct au state suivant
	}
	else
		drone->soft.dead_line = drone->soft.previous_time_loop + LOOP_PERIODE  ;	//si on a du temps dispo, on le dit
}
void LOW_LVL_Pwm_High(State_drone_t * drone){
	//Starting pwm signals for the escs
	sub_esc_start_pulse(drone->stabilisation.escs);
	drone->soft.state_low_level = UPDATE_ANGLES ;
}

void LOW_LVL_Update_Angles(State_drone_t * drone){
	//read mpu data and appli the filter

	if(!drone->capteurs.mpu.mpu_result)
		DRONE_mpu6050_update_angles(&drone->capteurs.mpu);

	drone->soft.state_low_level = VERIF_SYSTEM ;
}

void LOW_LVL_Verif_System(State_drone_t * drone){
	uint32_t time = SYSTICK_get_time_us();
	//Verif si le ppm est toujours bien reçu
	if(time > drone->communication.ibus.last_update + TIME_OUT_IBUS)
		drone->communication.ibus.is_ok = FALSE ;
	else
		drone->communication.ibus.is_ok = TRUE ;

	//Verif si le gps est ok
	if(time > drone->capteurs.gps.last_time_read_gps + TIME_OUT_GPS)
		drone->capteurs.gps.is_ok = FALSE ;
	else
		drone->capteurs.gps.is_ok = TRUE ;

	//Mesure batterie
	DRONE_batterie_update(&drone->capteurs.batterie);

	drone->soft.state_low_level = PWM_LOW ;
}

void LOW_LVL_Pwm_Low(State_drone_t * drone){
	drone->soft.state_low_level = try_action(sub_esc_end_pulse(drone->stabilisation.escs), drone->soft.state_low_level, HIGH_LVL, ERROR_HIGH_LEVEL);
}


void LOW_LVL_Process_High_Lvl(State_drone_t * drone, State_base_t * base){
	//Détermine un changement de mode de vol
	static Flight_Mode_SM previous_state_flight_mode = PARACHUTE ;
	drone->soft.entrance_flight_mode = drone->soft.state_flight_mode != previous_state_flight_mode ;
	previous_state_flight_mode = drone->soft.state_flight_mode ;
	switch(drone->soft.state_flight_mode){

		case ON_THE_GROUND :
			HIGH_LVL_On_The_Ground(drone);
			break;

		case MANUAL :
			HIGH_LVL_Manual(drone);
			break;

		case MANUAL_STYLEE:
			HIGH_LVL_Manual_Stylee(drone, base);
			break;

		case PARACHUTE :
			HIGH_LVL_Parachute(drone);
			break;

		case CALIBRATE_MPU6050:
			HIGH_LVL_Calibrate_MPU(drone);
			break;

		case MANUAL_PC:
			HIGH_LVL_Manual_Pc(drone);
			break;

		case MANUAL_ACCRO :
			HIGH_LVL_Manual_Accro(drone);
			break;

		case IMU_FAILED_INIT:
			HIGH_LVL_IMU_Failed_Init(drone);
			break;

		case POSITION_HOLD:
			//TODO Position hold function
			break;
		case ALTITUDE_HOLD:
			//TODO Altitude hold function
			break;
	}
	drone->soft.state_low_level = STABILISATION ;


}

void LOW_Lvl_Stabilisation(State_drone_t * drone){
	//If we fly, we compute our outputs, according to our targets, to keep the drone leveled
	double roll_output = 0 ;
	double pitch_output = 0 ;
	double yaw_output	= 0 ;
	switch(drone->stabilisation.stab_mode){

		case STAB_OFF :
			drone->stabilisation.escs[0].pulsation = PULSATION_MIN ;
			drone->stabilisation.escs[1].pulsation = PULSATION_MIN ;
			drone->stabilisation.escs[2].pulsation = PULSATION_MIN ;
			drone->stabilisation.escs[3].pulsation = PULSATION_MIN ;
			break;

		case LEVELLED :
			roll_output 	= PID_compute_SOF_d(&drone->stabilisation.pid_roll, drone->consigne.roll, drone->capteurs.mpu.y);
			pitch_output 	= PID_compute_SOF_d(&drone->stabilisation.pid_pitch, drone->consigne.pitch, drone->capteurs.mpu.x);
			yaw_output 		= PID_compute_FOF_d(&drone->stabilisation.pid_yaw, drone->consigne.yaw, drone->capteurs.mpu.z);

			//TELEMETRIE_send_double(drone->stabilisation.pid_roll.D_filtered[0], ID_PC_PID_D_ROLL, &drone->communication.uart_telem);
			//TELEMETRIE_send_double(drone->stabilisation.pid_roll.D_filtered, ID_PC_PID_P_ROLL, &drone->communication.uart_telem);


			ESC_Set_pulse(&drone->stabilisation.escs[0], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(- roll_output + pitch_output + yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[1], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(+ roll_output + pitch_output - yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[2], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(- roll_output - pitch_output - yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[3], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(+ roll_output - pitch_output + yaw_output)));
			break;

		case ACCRO :
			roll_output 	= PID_compute(&drone->stabilisation.pid_roll_rate, drone->consigne.roll_rate, drone->capteurs.mpu.y_gyro);
			pitch_output 	= PID_compute_FOF_d(&drone->stabilisation.pid_pitch_rate, drone->consigne.pitch_rate, drone->capteurs.mpu.x_gyro);
			yaw_output 		= PID_compute_FOF_d(&drone->stabilisation.pid_yaw_rate, drone->consigne.yaw_rate, drone->capteurs.mpu.z_gyro);

			ESC_Set_pulse(&drone->stabilisation.escs[0], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(- roll_output + pitch_output + yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[1], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(+ roll_output + pitch_output - yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[2], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(- roll_output - pitch_output - yaw_output)));
			ESC_Set_pulse(&drone->stabilisation.escs[3], (uint16_t)(1000 + drone->consigne.throttle + (int16_t)(+ roll_output - pitch_output + yaw_output)));
			break;

		default:
			break;

	}
	drone->soft.state_low_level = SEND_DATA ;
}


void LOW_LVL_Send_Data(State_drone_t * drone){
	if(drone->communication.ibus.channels[SWITCH_4] > 1500)
		sub_send_data(drone);
	drone->soft.state_low_level = WAIT_LOOP ;
}



