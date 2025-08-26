/*
 * relay.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hedg
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "stm32f1xx_hal.h"

void Relay_Init(void);
void Relay_On(void);
void Relay_Off(void);
uint8_t Relay_GetState(void);

#endif /* INC_RELAY_H_ */
