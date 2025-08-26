/*
 * web.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hedg
 */

#ifndef INC_WEB_H_
#define INC_WEB_H_


#include <stdint.h>

void Web_HandleRequest(uint8_t *request, int len, int socket);

#endif /* INC_WEB_H_ */
