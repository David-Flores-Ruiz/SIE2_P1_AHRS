/*
 * AHRS.h
 *
 *  Created on: Apr 3, 2020
 *      Author: D.F.R. / R.G.P.
 */

#ifndef AHRS_H_
#define AHRS_H_

/* TODO: insert other include files here. */
#include <stdint.h>
#include "rtos_uart.h"
#include "mahony.h"

/* TODO: insert other definitions and declarations here. */
#define HEADER_VAL 0xAAAAAAAA

typedef struct {
	uint32_t header;
	float x;
	float y;
	float z;
} comm_msg_t;





#endif /* AHRS_H_ */
