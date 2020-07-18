/*
 * pid.c
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#include "pid.h"


double PID_compute(PID_t* pid, double consigne, double angle){
	//Store previous d


	//Calcule error
	pid->error = angle - consigne ;

	//Calcule P I D

	pid->P = pid->kP * pid->error ;
	pid->I += pid->kI * pid->error / pid->frequency;
	pid->D = pid->kD * (angle - pid->previous_angle) * (double)pid->frequency ;

	//Calcul filtered D
	pid->D_filtered = pid->D_filtered + (pid->D - pid->D_filtered) * pid->kD_filter;

	//Ad filtered or not D
	double output = pid->P + pid->I ;
	if(pid->use_D_filtered)
		output += pid->D_filtered ;
	else
		output += pid->D;

	//Check mid and max
	output = MIN(pid->max_output, output);
	output = MAX(-pid->max_output, output);
	pid->previous_error = pid->error ;
	pid->previous_angle = angle ;
	return output ;
}

void PID_init(PID_t* pid, double kp, double ki, double kd, double kd_filter, uint16_t frequency_, double max_output, bool_e use_D_filtered){
	pid->use_D_filtered = use_D_filtered ;
	pid->kP = kp ;
	pid->kI = ki ;
	pid->kD = kd ;
	pid->kD_filter = kd_filter ;
	pid->frequency = frequency_ ;
	pid->max_output = max_output ;
	pid->I = 0 ;
}
