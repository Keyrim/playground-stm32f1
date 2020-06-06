/*
 * sub_test_i2c.c
 *
 *  Created on: 5 juin 2020
 *      Author: Theo
 */
#include "sub.h"

void sub_test_i2c(){
	printf("Test de toutes les adresses i2C \n");
	for(uint16_t device = 0; device < 256; device ++){
		printf("Device at the adresse %d ? %d \n", (int16_t)device, I2C_IsDeviceConnected(I2C1, device));
		HAL_Delay(50);
	}
}
