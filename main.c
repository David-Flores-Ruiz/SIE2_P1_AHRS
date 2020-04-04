
 
/**
 * @file    Practica_1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
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
		BAUD_RATE, rtos_i2c_0, rtos_i2c_portB,
		PIN_SCL_PTB2,
		PIN_SDA_PTB3,
		GPIO_MUX2
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
	    uint8_t dato=0x0;
	    //HEY
	    PRINTF("Hello World\n");
	    rtos_i2c_init(g_rtos_i2c_config);
	    //rtos_i2c_transfer(rtos_i2c_0, &dato, 7, 0xD2, 0x00, 8);
	    rtos_i2c_receive(rtos_i2c_0, &dato, 8, 0x68,0x00, 8);
	    /*
	     *
	     * ¿length=8?
	     * ¿subadress es el registro que deseamos leer del modulo?
	     * 				es decir leer registro 00 regresaria un D1
	     * 	¿Subsize es el tamaño del dato leído?
	     */

    while(1) {

    }
    return 0 ;
}
