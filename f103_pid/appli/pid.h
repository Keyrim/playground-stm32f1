/*
 * pid.h
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#ifndef PID_H_
#define PID_H_



typedef struct {
	double kP ;
	double kI ;
	double kD ;
	double error ;
	double previous_error ;
	uint16_t frequency ;
	double max_output ;
	double P;
	double I;
	double D;
}PID_t;

double PID_compute(PID_t* pid, double error);


#endif /* PID_H_ */
