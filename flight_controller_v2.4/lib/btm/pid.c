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
	pid->D_filtered[0] = pid->alpha * pid->D + pid->beta * pid->D_filtered[1] + pid->gamma * pid->D_filtered[2];

	//Ad filtered or not D
	pid->output = pid->P + pid->I ;
	if(pid->use_D_filtered)
		pid->output += pid->D_filtered[0] ;
	else
		pid->output += pid->D;

	//Check mid and max
	pid->output = MIN(pid->max_output, pid->output);
	pid->output = MAX(-pid->max_output, pid->output);

	//Update previous values
	pid->previous_angle = angle ;
	pid->D_filtered[2] = pid->D_filtered[1] ;
	pid->D_filtered[1] = pid->D_filtered[0] ;
	return pid->output ;
}

void PID_init(PID_t* pid, double kp, double ki, double kd, double alpha, double beta, double gamma, uint16_t frequency_, double max_output, bool_e use_D_filtered){
	pid->use_D_filtered = use_D_filtered ;
	//Set coef pid
	pid->kP = kp ;
	pid->kI = ki ;
	pid->kD = kd ;

	//Set coefs filter
	pid->alpha = alpha ;
	pid->beta = beta ;
	pid->gamma = gamma ;
	//Set other settings
	pid->frequency = frequency_ ;
	pid->max_output = max_output ;

	//Init values
	pid->I = 0 ;
	pid->D_filtered[1] = 0 ;
	pid->D_filtered[2] = 0 ;
}
