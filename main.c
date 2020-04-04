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
#include "rtos_i2c.h"
#include "rtos_uart.h"
#include "mahony.h"

/* TODO: insert other definitions and declarations here. */
#define SYSTEM_CLOCK (10500000U)
#define BAUD_RATE (9600U)
#define PIN_SCL_PTB2 (2U)
#define PIN_SDA_PTB3 (3U)
#define GPIO_MUX2 (2U)

#define BMI160_SLAVE_ADDR 0x68 // Default I2C 7-bit address of device if SDO = GND
#define LENGHT (8U)		// Byte Read/Write
#define SUBSIZE (8U)	// Size of each Direction Register of the BMI160

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
		rtos_i2c_0, 	/**I2C to use*/
		rtos_i2c_portB,	/**Kinetis Port*/
		PIN_SCL_PTB2,	/**Pin of Serial Clock*/
		PIN_SDA_PTB3,	/**Pin of Serial Data*/
		GPIO_MUX2		/**Pin Configuration*/
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

	    uint8_t dataToWrite = ACCEL_NORMAL_MODE & GYROS_NORMAL_MODE;	// Byte to write
	    uint8_t dataRead = 0x00;	// Read byte

	    rtos_i2c_transfer(rtos_i2c_0, &dataToWrite, 8, BMI160_SLAVE_ADDR, BMI160_CMD, 	  SUBSIZE);

	    rtos_i2c_receive(rtos_i2c_0, &dataRead,    8, BMI160_SLAVE_ADDR, BMI160_CHIP_ID, SUBSIZE);

		if (dataRead == BMI160_CHIP_ID_VAL) {
			printf("\nAqui estoy! BMI160... como sensor IMU externo \n");
		}


	    /*
	     *
	     * ¿length=8?
	     * ¿subadress es el registro que deseamos leer del modulo?
	     * 				es decir leer registro 00 regresaria un D1
	     * 	¿Subsize es el tamaño del dato leído?
	     */

	while (1) {

	}

	return 0;
}
