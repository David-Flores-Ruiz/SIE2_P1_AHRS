/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 */

/*
 * @file:			P1_AHRS.c
 * @brief:   		P1_AHRS_in_FreeRTOS_OS_for_FRDM-K66F
 *
 * @company:			  ITESO
 * @Engineer Team:	 D.F.R. / R.G.P.
 * @contact:		ie717807@iteso.mx
 * @contact:		ie706818@iteso.mx
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h" // "MK66F18.h" // "MK64F12.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "BMI160.h"
#include "AHRS.h"

#include "rtos_uart.h"
#include "rtos_i2c.h"
#include "mahony.h"

#define DUBUG_ON 1

/* TODO: insert other definitions and declarations here. */
/*
 * Physical connections (only 7) of the module BMI160:
 * Board pin | Signal Name | Logic level
 * 		1		VDD				3.3v
 * 		2		VDDIO			3.3v
 * 		3		GND				GND
 * 		7		CSB				3.3v 	= Select I2C protocol
 * 		15		SDO				GND		= Select 7-bits slave address: 0x68
 * 		17		SDA				Freedom K66F (PTC11)
 * 		18		SCL				Freedom K66F (PTC10)
 */

typedef struct
{
	QueueHandle_t queue_freertos;
	SemaphoreHandle_t mutex_print_freertos;
	SemaphoreHandle_t semaphore_FreeRTOs;
	EventGroupHandle_t event_FreeRTOs;
} parameters_task_t;

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	PRINTF("Hello World\n");

	static parameters_task_t parameters_task;

#ifndef DUBUG_ON
	xTaskCreate(BMI160_I2C_ReadChipID, "BMI160_I2C_ReadChipID", 500, (void*)&parameters_task, configMAX_PRIORITIES	, NULL);
	xTaskCreate(data_acquisition_task, "data_acquisition_task", 500, (void*)&parameters_task, configMAX_PRIORITIES-2, NULL);
#endif

	xTaskCreate(Ahrs_send_UART_angles_task, "Ahrs_send_UART_angles_task", 500, (void*)&parameters_task, configMAX_PRIORITIES, NULL);

	vTaskStartScheduler();

	while (1) {
		// If you fall here it is so bad :(
		__asm volatile ("nop");
	}

	return 0;
}
