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
#define acc_range 2	// 2g range (reset)
#define acc_max_val 32768	// INT16 con signo (abs del negativo)
#define gyr_range 2000	// 2000°/s range (reset)
#define gyr_max_val 32768	// INT16 con signo (abs del negativo)

#define HEADER_VAL 0xAAAAAAAA
#define AHRS_IMU_SAMPLE_TIME 	20			// 20 ms --- f= 50 Hz
#define AHRS_SENDUART_SAMPLE_TIME 	50		// 50 ms --- f= 12.5 Hz


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

void data_calibration_acc(void);	// Conversion acc to Float
void data_calibration_gyr(void);	// Conversion gyr to Float
void Ahrs_send_UART_angles_task(void * args);


#endif /* AHRS_H_ */
