/*
 * sub.h
 *
 *  Created on: 5 juin 2020
 *      Author: Theo
 */

#ifndef SUB_H_
#define SUB_H_

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "branchements.h"
#include "GPS.h"
#include "compas.h"
#include "stm32f1_i2c.h"

//Sub pour test le gps
void sub_test_gps(bool_e bloquant, bool_e init);

void sub_test_i2c();

#endif /* SUB_H_ */
