/*
 * branchement.h
 *
 *  Created on: 28 mars 2020
 *      Author: Theo
 */

#ifndef BRANCHEMENT_H_
#define BRANCHEMENT_H_

//Batterie
#define BATTERIE_ADC_VOLTAGE ADC_9
#define PIN_BATTERIE	GPIO_PIN_1
#define PORT_BATTERIE	GPIOB

//state led
#define GPIO_STATE_LED GPIOC
#define GPIO_PIN_STATE_LED GPIO_PIN_13

//Branchements ppm
#define GPIO_PPM GPIOB
#define GPIO_PIN_PPM GPIO_PIN_12
#define PIN_NUMBER 12

//Gps
#define UART_GPS UART2_ID

#endif /* BRANCHEMENT_H_ */
