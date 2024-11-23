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
#include "priority_queue.h"

#define AO_UI_QUEUE_LENGTH_    (5)
#define AO_UI_QUEUE_ITEM_SIZE_ (sizeof(button_type_t))

void task_ui(void *argument){
	ao_ui_handle_t *huiao = (ao_ui_handle_t *)argument;
	button_type_t button_press_type;
	bool rtrn;

	while (true){
		if (pdTRUE == xQueueReceive(huiao->hqueue, (void *)&button_press_type, portMAX_DELAY)){
			switch (button_press_type){
				case BUTTON_TYPE_PULSE:
					rtrn = PriorityQueue_Send(EVENT_PRIORITY_HIGH);
					break;
				case BUTTON_TYPE_SHORT:
					rtrn = PriorityQueue_Send(EVENT_PRIORITY_MEDIUM);
					break;
				case BUTTON_TYPE_LONG:
					rtrn = PriorityQueue_Send(EVENT_PRIORITY_LOW);
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

