/*
 * pid.c
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#include "pid.h"

double PID_compute(PID_t* pid, double error_){
	pid->error = error_ ;
	pid->P = pid->kP * error_ ;
	pid->I += pid->kI * error_ / pid->frequency;
	pid->D = pid->kD * (error_ - pid->previous_error) * pid->frequency ;
	double output = pid->P + pid->I + pid->D ;
	output = MIN(pid->max_output, output);
	output = MAX(-pid->max_output, output);
	pid->previous_error = error_ ;
	return output ;
}
