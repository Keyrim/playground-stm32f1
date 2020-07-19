/*
 * ppm.h
 *
 *  Created on: 22 mars 2020
 *      Author: Theo
 */

#ifndef PPM_H_
#define PPM_H_

#include "macro_types.h"
#include "stm32f103xb.h"
#include "stm32f1_gpio.h"
#include "systick.h"

void PPM_init(uint16_t* channels_array_, uint8_t nb_channel_, uint8_t pin_number, GPIO_TypeDef* gpio_, uint16_t gpio_pin_, bool_e enable_it);
void enable_it(void);
void disable_it(void);
void to_print(void);

#endif /* PPM_H_ */
