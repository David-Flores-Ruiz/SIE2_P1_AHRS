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
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"

#include "BMI160.h"
#include "rtos_uart.h"
#include "mahony.h"

/* TODO: insert other definitions and declarations here. */
#define AHRS_IMU_SAMPLE_TIME 1000
#define HEADER_VAL 0xAAAAAAAA

typedef struct {
	uint32_t header;
	float x;
	float y;
	float z;
} comm_msg_t;

typedef struct {
	float x;
	float y;
	float z;
}BMI160_float_acc_data_t;

typedef struct {
	float x;
	float y;
	float z;
}BMI160_float_gyr_data_t;



void data_acquisition_task(void * args);



#endif /* AHRS_H_ */
