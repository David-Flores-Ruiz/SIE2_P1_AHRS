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
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"

#include "BMI160.h"
#include "rtos_uart.h"
#include "mahony.h"

/* TODO: insert other definitions and declarations here. */
#define acc_range 2	// 2g range (reset)
#define acc_max_val 32768	// INT16 con signo (abs del negativo)
#define gyr_range 2000	// 2000°/s range (reset)
#define gyr_max_val 32768	// INT16 con signo (abs del negativo)

#define HEADER_VAL 0xAAAAAAAA
#define AHRS_IMU_SAMPLE_TIME 	20			// 10 ms  --- 	f= 100 Hz
//#define AHRS_SENDUART_SAMPLE_TIME 	50	//				f= 100 Hz / N
#define EVENT_MUESTRASREADY    (1 << 0)

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


void data_calibration_acc(void);	// Conversion acc to Float
void data_calibration_gyr(void);	// Conversion gyr to Float


#endif /* AHRS_H_ */
