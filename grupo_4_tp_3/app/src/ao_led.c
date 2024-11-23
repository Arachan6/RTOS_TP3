#include <ao_led.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "app.h"
#include "ao_led.h"
#include "ao_ui.h"

#define EVENT_LED_PERIOD_        			(5000)

void task_led(void *argument){
	bool rtrn;
	EventPriority recvd_prt;
	TickType_t previousTime;
	while (true){
		rtrn = PriorityQueue_Receive(&recvd_prt);
		if (rtrn){
			previousTime = xTaskGetTickCount();
			switch (recvd_prt){
				case EVENT_PRIORITY_HIGH:
					LOGGER_INFO("Executing high priority event");
					HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
					vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(EVENT_LED_PERIOD_));
					HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
					break;
				case EVENT_PRIORITY_MEDIUM:
					LOGGER_INFO("Executing medium priority event");
					previousTime = xTaskGetTickCount();
					HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
					vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(EVENT_LED_PERIOD_));
					HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
					break;
				case EVENT_PRIORITY_LOW:
					LOGGER_INFO("Executing low priority event");
					previousTime = xTaskGetTickCount();
					HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
					vTaskDelayUntil(&previousTime, pdMS_TO_TICKS(EVENT_LED_PERIOD_));
					HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
					break;
				default:
					break;
			}
		}
	}
}

void ao_led_init (){
	BaseType_t status;
	status = xTaskCreate(task_led, "task_ao_led", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}
