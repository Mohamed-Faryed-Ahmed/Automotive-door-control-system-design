/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
	
#define ON    (1U)
#define OFF   (0U)

typedef enum{STOPE =0, MOVE =1}SPEED;
typedef enum{OFF_L =0, ON_L =1}LIGHT;
typedef enum{OFF_S =0, ON_S =1}SWITCH;

typedef struct
{
	SPEED Speed   :2;
	LIGHT Light   :2;
	SWITCH Switch :2;
	
}CAN_Data;

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

TaskHandle_t CAN_task_Handle     = NULL;
TaskHandle_t MAIN_task_Handle    = NULL;
/*
EventBits_t uxBits;
EventGroupHandle_t  xEventGroup ;
*/

void CAN_task( void * pvParameters );
void MAIN_task( void * pvParameters );


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
 
/* Global Variable */

CAN_Data Data={STOPE,OFF_L,OFF_S};
unsigned char light_status = OFF;

int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
	
	xTaskCreate(CAN_task ,"CAN" , 100,( void * ) 0, 2,&CAN_task_Handle );
	xTaskCreate(MAIN_task,"MAIN", 100,( void * ) 0, 1,&MAIN_task_Handle);
	
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
volatile pinState_t button_status=PIN_IS_LOW;

void CAN_task( void * pvParameters )
{
	for( ;; )
	{
		//recieve data from ECU_1
	}
}

void MAIN_task( void * pvParameters )
{
	for( ;; )
	{
		//Tack Action
		if(Data.Switch == ON_S)
		{
			if(Data.Speed == MOVE)
			{
				//Buzzer ON, Lights OFF
			}
			else 
			{
				//Buzzer OFF, Lights ON
			}
		}
		else if(Data.Switch == OFF_S)
		{
			if(light_status == ON)
			{
				//Lights are OFF after 3 seconds
				light_status = OFF;
			}
			else
			{}
		}
		if(Data.Speed ==MOVE)
		{
			if(Data.Light ==ON_L)
			{
				//Buzzer OFF, Lights ON
				light_status = ON;
			}
			else{}
		}
		else if(Data.Speed ==STOPE)
		{
			if(Data.Light ==ON_L)
			{
				//Buzzer ON, Lights OFF
				light_status = OFF;
			}
			else{}
		}
	}
}


/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure CAN */
	
	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


