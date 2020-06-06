/*
 * compas.c
 *
 *  Created on: 4 juin 2020
 *      Author: Theo
 */

#include "compas.h"
#include "stm32f1_i2c.h"

void COMPAS_init(bool_e is_i2c_init, Compass_t * compass){
	//Init de l'I2C si c pas fait
	if(!is_i2c_init)
		I2C_Init(COMPASS_I2C, 400000);
	if(!I2C_IsDeviceConnected(COMPASS_I2C, COMPASS_ADRESSE))
		printf("No compass detected");
	else
		compass->initialized = 1 ;

	//Configuration des registres
//	HWire.write(0x00);                                            //We want to write to the Configuration Register A (00 hex).
//	  HWire.write(0x78);                                            //Set the Configuration Regiser A bits as 01111000 to set sample rate (average of 8 at 75Hz).
//	  HWire.write(0x20);                                            //Set the Configuration Regiser B bits as 00100000 to set the gain at +/-1.3Ga.
//	  HWire.write(0x00);                                            //Set the Mode Regiser bits as 00000000 to set Continues-Measurement Mode.
	uint8_t configs[3] = {0x78, 0x20, 0x00};
	I2C_WriteMulti(COMPASS_I2C, COMPASS_ADRESSE, 0x00, configs, 3);

	//Normalement c bon ^^

}

void COMPAS_read(Compass_t * compass){
	uint8_t data[6] ;
	I2C_ReadMulti(COMPASS_I2C, COMPASS_ADRESSE, 0x03,data, 6);
	compass->x = (int16_t)(data[0] | data[1]) ;
	compass->z = (int16_t)(data[2] | data[3]) ;
	compass->y = (int16_t)(data[4] | data[5]) ;
	//Idem NORMALEMENT ça marche :p
}

