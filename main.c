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
#include "task.h"
#include "rtos_i2c.h"
#include "rtos_uart.h"
#include "mahony.h"

/* TODO: insert other definitions and declarations here. */
#define SYSTEM_CLOCK (10500000U)
#define BAUD_RATE (100000)	// 100000 from Chava help antes teniamos 9600
#define PIN_SCL_PTC10 (10U)
#define PIN_SDA_PTC11 (11U)

/* The I2C address of the device is 0b 110_1000 (0x68) if the SDO pin is pulled to GND */
#define BMI160_SLAVE_ADDR_7bits 0x68 // Default I2C 7-bit address of device if SDO = GND
#define LENGHT (1U)		// 1 Byte Read/Write
#define SUBSIZE (1U)	// 1 Byte is the size of each Direction Register of the BMI160

#define BMI160_CMD 0x7E
#define BMI160_CHIP_ID 0x00	  // This register contains the value 0xD1
#define BMI160_CHIP_ID_VAL 0xD1

#define ACCEL_NORMAL_MODE 0x11
#define GYROS_NORMAL_MODE 0x15



/*
 * I2C_0
 * 	PTB2 -SCL AZUL
 * 	PTB3 -SDA
 * 	MODULO IMU
 * 	1 - VCC 3.3v	15 - NC
 * 	2 - NC			16 - R=2.2k
 * 	3 - GND			17 - R=2.2k
 * 	4 - GND			18 - NC
 * 	5 - NC			19 - NC
 * 	6 - NC			20 - NC
 * 	7 - VCC			21 - NC
 * 	8 - NC			22 - NC
 * 	9 - NC			23 - NC
 * 	10 - NC			24 - NC
 * 	11 - NC			25 - NC
 * 	12 - NC			26 - NC
 * 	13 - NC			27 - NC
 * 	14 - NC			28 - NC
 */
const rtos_i2c_config_t g_rtos_i2c_config = {
		BAUD_RATE, 		/**I2C Baud rate*/
		rtos_i2c_1, 	/**I2C to use*/
		rtos_i2c_portC,	/**Kinetis Port*/
		PIN_SCL_PTC10,	/**Pin of Serial Clock*/
		PIN_SDA_PTC11,	/**Pin of Serial Data*/
		kPORT_MuxAlt2	/**Pin Configuration*/
};

/*
 * @brief   Application entry point.
 */

int main(void) {
	/* Init board hardware. */
	    BOARD_InitBootPins();
	    BOARD_InitBootClocks();
	    BOARD_InitBootPeripherals();
	  	/* Init FSL debug console. */
	    BOARD_InitDebugConsole();

	    PRINTF("Hello World\n");



	    // Inicio de BMI160

	    rtos_i2c_init(g_rtos_i2c_config);

	    uint8_t dataToWrite = ACCEL_NORMAL_MODE | GYROS_NORMAL_MODE;	// Byte to write
	    uint8_t dataRead = 0x00;	// Read byte

	    rtos_i2c_receive(rtos_i2c_1, &dataRead,    1, BMI160_SLAVE_ADDR_7bits, BMI160_CHIP_ID, 1);

		if (dataRead == BMI160_CHIP_ID_VAL) {
			printf("\nAqui estoy! BMI160... como sensor IMU externo \n");
			// Subaddress es el registro que deseamos leer del modulo IBM160:
			// Es decir, leer registro 0x00 debe regresar un 0xD1
		}

//	    rtos_i2c_transfer(rtos_i2c_1, &dataToWrite, 1, BMI160_SLAVE_ADDR_7bits, BMI160_CMD, 	1);

		vTaskStartScheduler();

	while (1) {

	}

	return 0;
}
