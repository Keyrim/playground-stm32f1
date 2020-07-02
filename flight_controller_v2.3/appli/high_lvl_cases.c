/*
 * high_lvl_cases.c
 *
 *  Created on: 29 juin 2020
 *      Author: Théo
 */

#include "high_lvl_cases.h"

void HIGH_LVL_On_The_Ground(State_drone_t * drone){
	//Si on est au sol on met la consigne des moteurs à zéro
	drone->stabilisation.escs[0].pulsation = PULSATION_MIN ;
	drone->stabilisation.escs[1].pulsation = PULSATION_MIN ;
	drone->stabilisation.escs[2].pulsation = PULSATION_MIN ;
	drone->stabilisation.escs[3].pulsation = PULSATION_MIN ;

	//Conditions de transitions
	if(drone->communication.ppm.channels[SWITCH_1] > 1500 && drone->communication.ppm.is_ok && (drone->capteurs.batterie.voltage > 10.8 || !USE_BATTERIE_CHECK)){	//Conditions générales
		if(drone->communication.ppm.channels[THROTTLE] < 1100 && drone->communication.ppm.channels[THROTTLE] > 1070){								//Pour passer en manuel
			drone->soft.state_flight_mode = MANUAL ;
		}
	}
}

void HIGH_LVL_Manual(State_drone_t * drone){
	//			---------------------------- 		MAIN PART 			----------------------------------------
	drone->consigne.roll = ((double)(drone->communication.ppm.channels[ROLL]- 1500) / (double)500) * (double)roll_and_pitch_max_setpoint ;
	drone->consigne.pitch = ((double)(drone->communication.ppm.channels[PITCH]- 1500) / (double)500) * (double)roll_and_pitch_max_setpoint ;
	drone->consigne.yaw = ((double)(drone->communication.ppm.channels[YAW] - 1500) / (double)500) * (double)yaw_max_setpoint ;		//Consigne en degré par seconde attention ^^
	drone->consigne.throttle = (double)(drone->communication.ppm.channels[THROTTLE] - 1050)  ;

	//			---------------------------- Condition de transition 	----------------------------------------
	if(drone->communication.ppm.channels[SWITCH_1] < 1500 || drone->communication.ppm.channels[THROTTLE] < 1070 || !drone->communication.ppm.is_ok){		// Securité
		drone->soft.state_flight_mode = ON_THE_GROUND ;
	}
	else if(drone->communication.ppm.channels[SWITCH_2] > 1650)														//Passage en parachute
		drone->soft.state_flight_mode = PARACHUTE ;
	else if(drone->communication.ppm.channels[SWITCH_2] > 1350)														//Passage en manuel "stylé"
		drone->soft.state_flight_mode = MANUAL_STYLEE ;
}

void HIGH_LVL_Manual_Stylee(State_drone_t * drone, State_base_t * base){

	//			---------------------------- 		MAIN PART 			----------------------------------------
	drone->consigne.yaw = ((double)(drone->communication.ppm.channels[YAW] - 1500) / (double)500) * (double)yaw_max_setpoint ;	//Consigne en degré par seconde attention ^^
	drone->consigne.throttle = (double)(drone->communication.ppm.channels[THROTTLE] - 1050)  ;
	drone->consigne.roll = base->angle_x ;
	drone->consigne.pitch = base->angle_y ;

	//			---------------------------- Condition de transition 	----------------------------------------
	if(drone->communication.ppm.channels[SWITCH_1] < 1500 || drone->communication.ppm.channels[THROTTLE] < 1070 || !drone->communication.ppm.is_ok)		// Securité
		drone->soft.state_flight_mode = ON_THE_GROUND ;
	else if(drone->communication.ppm.channels[SWITCH_2] > 1650)														//Passage en parachute
		drone->soft.state_flight_mode = PARACHUTE ;
	else if(drone->communication.ppm.channels[SWITCH_2] < 1350 && drone->communication.ppm.channels[SWITCH_2] > 1000 )														//Passage en manuel "stylé"
		drone->soft.state_flight_mode = MANUAL ;

}

void HIGH_LVL_Parachute(State_drone_t * drone){

	//			---------------------------- 		MAIN PART 			----------------------------------------
	drone->consigne.throttle = (double)(drone->communication.ppm.channels[THROTTLE] - 1050)  ;
	if(sub_parachute(drone) != IN_PROGRESS){
		drone->soft.state_flight_mode = ON_THE_GROUND ;
	}

	//			---------------------------- Condition de transition 	----------------------------------------
	if(drone->communication.ppm.channels[SWITCH_1] < 1500 || drone->communication.ppm.channels[THROTTLE] < 1070 || !drone->communication.ppm.is_ok)		// Securité
		drone->soft.state_flight_mode = ON_THE_GROUND ;
	else if(drone->communication.ppm.channels[SWITCH_2] > 1350 && drone->communication.ppm.channels[SWITCH_2] < 1650)														//Passage en parachute
		drone->soft.state_flight_mode = MANUAL_STYLEE ;
	else if(drone->communication.ppm.channels[SWITCH_2] < 1350 && drone->communication.ppm.channels[SWITCH_2] > 1000 )														//Passage en manuel "stylé"
		drone->soft.state_flight_mode = MANUAL ;
}
















