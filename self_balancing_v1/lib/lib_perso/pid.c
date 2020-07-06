/*
 * pid.c
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#include "pid.h"


double PID_compute(PID_t* pid, double consigne, double angle){
	pid->error = angle - consigne ;
	pid->P = pid->kP * pid->error ;
	pid->I += pid->kI * pid->error / pid->frequency;
	pid->D = pid->kD * (angle - pid->previous_angle) * (double)pid->frequency ;
	double output = pid->P + pid->I + pid->D ;
	output = MIN(pid->max_output, output);
	output = MAX(-pid->max_output, output);
	pid->previous_error = pid->error ;
	pid->previous_angle = angle ;
	return output ;
}

void PID_init(PID_t* pid, double kp, double ki, double kd, uint16_t frequency_, double max_output){
	pid->kP = kp ;
	pid->kI = ki ;
	pid->kD = kd ;
	pid->frequency = frequency_ ;
	pid->max_output = max_output ;
	pid->I = 0 ;
}
