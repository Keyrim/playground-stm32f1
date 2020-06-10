/*
 * esc.c
 *
 *  Created on: 23 mars 2020
 *      Author: Theo
 */


#include "esc.h"
#include "stm32f1_gpio.h"
#include "stdio.h"


static uint8_t nb_escs ;
static ESC_e* escs ;

static uint32_t compteur = 0 ;

void ESC_init(ESC_e* escs_, uint8_t nb_escs_){
	nb_escs = nb_escs_ ;
	escs = escs_ ;
	for(uint8_t e =0; e < nb_escs_; e++){
		BSP_GPIO_PinCfg(escs_[e].gpio, escs_[e].gpio_pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH );
	}
	TIMER_run_us(TIMER_ID_PWM, PERIODE_TIMER, TRUE);


}

void ESC_it(void){
	if(compteur >= PERIODE_PWM){
		compteur = 0 ;
		for(uint8_t e = 0; e < nb_escs; e ++){
			escs[e].gpio->BSRR = escs[e].gpio_pin ;
		}
	}
	else{
		for(uint8_t e = 0; e < nb_escs; e ++){
			if(compteur >= escs[e].pulsation)
				escs[e].gpio->BSRR = (uint32_t)(escs[e].gpio_pin) << 16U ;
		}
	}
	compteur += PERIODE_TIMER ;
}


void TIMER1_user_handler_it(void){
	ESC_it();
}
void TIMER2_user_handler_it(void){
	ESC_it();
}
void TIMER3_user_handler_it(void){
	ESC_it();
}
void TIMER4_user_handler_it(void){
	ESC_it();
}


