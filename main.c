
 
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
#define PIN_SDA_PTB3 (2U)
#define GPIO_MUX2 (2U)
/*
 * I2C_0
 * 	PTB2 -SCL AZUL
 * 	PTB3 -SDA
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
	    PRINTF("Hello World\n");
	    rtos_i2c_init(g_rtos_i2c_config);
	   // rtos_i2c_transfer(rtos_i2c_0, &dato, 7, 0x69, 0x00, 7);
	    rtos_i2c_receive(rtos_i2c_0, &dato, 7, 0x69, 0x00, 7);


    while(1) {

    }
    return 0 ;
}
