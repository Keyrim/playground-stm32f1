/*
 * pid.h
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#ifndef PID_H_
#define PID_H_
#include "macro_types.h"


typedef struct {
	double output ;
	double kP ;
	double kI ;
	double kD ;
	double alpha ;
	double beta ;
	double gamma ;
	double error ;
	double previous_angle ;
	uint16_t frequency ;
	double max_output ;
	double P;
	double I;
	double D;
	double D_filtered[3];//D[0] coresspond à mtn, D[1] coresspond à D à t-1 et l'autre t-2
	bool_e use_D_filtered ;
}PID_t;

double PID_compute(PID_t* pid, double consigne, double angle);
void PID_init(PID_t* pid, double kp, double ki, double kd, double alpha, double beta, double gamma, uint16_t frequency_, double max_output, bool_e use_D_filtered);

#endif /* PID_H_ */
