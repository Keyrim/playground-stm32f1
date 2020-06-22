/*
 * sub_receive_pc.c
 *
 *  Created on: 15 juin 2020
 *      Author: Théo
 */

#include "sub.h"
#include "IDs.h"

const uint8_t liste_id[] = {0, 20, 100, 101, 102, 120, 160, 161, 162, 163, 164, 165, 200};

void sub_transmit(uint8_t c, uart_struct_e * uart, transmit_t * transmit, receive_struct_t * receive){

	switch(transmit->state){
		case WAIT_TRANSMIT:
			//Si on reçoit 255, on ignore car c l octect de "recallage"
			if(c != 255){
				transmit->id = c ;
				//On regarde si l'id existe
				uint8_t is_a_valid_id = 0;
				for(uint8_t id = 0; id < ID_NOMBRE_ID; id++)
					if(c == liste_id[id])
						is_a_valid_id= 1 ;
				//Si l'id existe on fait le travail sinon on ignore
				if(is_a_valid_id){
					//On détermince la longeur de nos données (1 à 5 octects)
					transmit->data_size = (uint8_t)(((transmit->id % 100) / 20) + 1) ;	//Ex : 124 => (%100) => 24 => (/20) => 1 => (+1) => 2 octects (voir répartitions adresses)
					transmit->data_size ++ ; //On augmente de 1 car on save l'id
					transmit->buffer[0] = transmit->id ;	//On save l'id
					transmit->compteur = 1 ;	//On met le compteur à 1 car c est là qu on va commencer à écrire
					transmit->state = BUFFERING ;
				}
			}
			break;
		case BUFFERING :
			transmit->buffer[transmit->compteur] = c;
			transmit->compteur ++ ;

			if(transmit->compteur == transmit->data_size){
				//Une fois qu'on à tout reçu on envoi au buffer d'envoit ^^
				if(transmit->id < 200)
					uart_add_few(uart, transmit->buffer, (uint16_t)transmit->data_size);
				else{
					switch(transmit->buffer[0]){
						case ID_BASE_CONSIGNE_BASE:
							switch(transmit->buffer[1]){
								case SUB_ID_BASE_CONSIGNE_START_SENDING_ANGLES:
									receive->send_angles = 1 ;
									break;
								case SUB_ID_BASE_CONSIGNE_STOP_SENDING_ANGLES:
									receive->send_angles = 0 ;
									break;
								default:
									break;
							}
							break;
						default:
							break;
					}
			}
				transmit->state = WAIT_TRANSMIT ;
			}
			break;
		default :
			transmit->state = WAIT_TRANSMIT ;
			break;

	}

}





































