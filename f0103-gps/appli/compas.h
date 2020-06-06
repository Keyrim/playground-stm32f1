/*
 * compas.h
 *
 *  Created on: 3 mai 2020
 *      Author: Theo
 */

#ifndef COMPAS_H_
#define COMPAS_H_
#include "macro_types.h"

//I2C pour le compas
#define COMPASS_I2C I2C1
//Adresse I2C du compas
#define COMPASS_ADRESSE 0x1E
//Fnction d'init

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
	bool_e initialized  ;
}Compass_t;

void COMPAS_init(bool_e is_i2c_init, Compass_t * compass);
void COMPAS_read(Compass_t * compass);

#endif /* COMPAS_H_ */
