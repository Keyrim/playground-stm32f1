/*
 * sub_receive_data.c
 *
 *  Created on: 13 juin 2020
 *      Author: Théo
 */

#define ID_ATTENTE 0
#define ID_STOP_MOTORS 1
#define ID_PITCH_ROLL 2


#include "sub_action.h"

void sub_receive_data(uint8_t c, uint8_t * state_flying, double * consigne_roll_base, double * consigne_pitch_base){
	static uint8_t state = ID_ATTENTE ;
	static uint8_t index = 0 ;
	static uint8_t buffer[10] = {0};
	if(state == ID_ATTENTE)
		state = c ;
	switch(state){
		case ID_ATTENTE :
			//on n'est pas censé arrivé là
			state = c ;
			break;
		case ID_STOP_MOTORS:
			*state_flying = 0 ;
			state = ID_ATTENTE ;
			break;

		case ID_PITCH_ROLL :
			//Si index nul, on vient d'arriver et le caractère corespond juste à l'id
			if(index)
				buffer[index] = c ;
			//Dans tous les cas on augmente l'index
			index ++ ;
			//Ici on veut juste deux données donc si on est à trois on se casse
			if(index == 3){
				*consigne_roll_base = (double)buffer[1] - 90 ;
				*consigne_pitch_base = (double)buffer[2] - 90 ;
				index = 0 ;
				state = ID_ATTENTE ;
			}
			break;
		default:
			state = ID_ATTENTE ;
			break;

	}
}
