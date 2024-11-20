#include <ao_led.h>
#include <ao_ui.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_button.h"
#include "ao_ui.h"
#include "app.h"

#define AO_UI_QUEUE_LENGTH_    (5)
#define AO_UI_QUEUE_ITEM_SIZE_ (sizeof(button_type_t))

extern ao_led_handle_t ao_red_led_h;
extern ao_led_handle_t ao_green_led_h;
extern ao_led_handle_t ao_blue_led_h;

static void callback_free(msg_t* pmsg) {
	vPortFree((void*)pmsg);
}

void task_ui(void *argument){
	ao_ui_handle_t *huiao = (ao_ui_handle_t *)argument;
	button_type_t button_press_type;

	while (true){
		if (pdTRUE == xQueueReceive(huiao->hqueue, (void *)&button_press_type, portMAX_DELAY)){
			msg_t* pmsg = (msg_t*)pvPortMalloc(sizeof(msg_t));
			switch (button_press_type){
				case BUTTON_TYPE_PULSE:
					LOGGER_INFO("UI red led blink");
					if(NULL != pmsg) {
						LOGGER_INFO("Memoria alocada: %d", sizeof(msg_t));
						pmsg->led_h = &ao_red_led_h;
						pmsg->callback_process = callback_free;
						if(pdPASS == ao_led_send(pmsg)) {
							LOGGER_INFO("Mensaje enviado");
						} else {
							LOGGER_INFO("Mensaje no enviado");
							vPortFree((void*)pmsg);
							LOGGER_INFO("Memoria liberada desde UI");
						}
					} else {
					  LOGGER_INFO("Memoria insuficiente");
					}
					break;
				case BUTTON_TYPE_SHORT:
					LOGGER_INFO("UI green led blink");
					if(NULL != pmsg) {
						LOGGER_INFO("Memoria alocada: %d", sizeof(msg_t));
						pmsg->led_h = &ao_green_led_h;
						pmsg->callback_process = callback_free;

						if(true == ao_led_send(pmsg)) {
							LOGGER_INFO("Mensaje enviado");
						} else {
							LOGGER_INFO("Mensaje no enviado");
							vPortFree((void*)pmsg);
							LOGGER_INFO("Memoria liberada desde UI");
						}
					} else {
					  LOGGER_INFO("Memoria insuficiente");
					}
					break;
				case BUTTON_TYPE_LONG:
					LOGGER_INFO("UI blue led blink");

					if(NULL != pmsg) {
						LOGGER_INFO("Memoria alocada: %d", sizeof(msg_t));
						pmsg->led_h = &ao_blue_led_h;
						pmsg->callback_process = callback_free;
						if(pdPASS == ao_led_send(pmsg)) {
							LOGGER_INFO("Mensaje enviado");
						} else {
							LOGGER_INFO("Mensaje no enviado");
							vPortFree((void*)pmsg);
							LOGGER_INFO("Memoria liberada desde UI");
						}
					} else {
					  LOGGER_INFO("Memoria insuficiente");
					}
					break;
				default:
					LOGGER_INFO("Unknown event for UI object");
					break;
			}
		}
	}
}

bool ao_ui_send(ao_ui_handle_t *hao, button_type_t btn_press_type){
    return (pdPASS == xQueueSend(hao->hqueue, (void *)&btn_press_type, 0));
}

void ao_ui_init(ao_ui_handle_t *hao){
    hao->hqueue = xQueueCreate(AO_UI_QUEUE_LENGTH_, AO_UI_QUEUE_ITEM_SIZE_);
    while (NULL == hao->hqueue){}

    BaseType_t status;
    status = xTaskCreate(task_ui, "task_ao_ui", 128, (void*)hao, tskIDLE_PRIORITY, NULL);
    configASSERT(pdPASS == status);
}

