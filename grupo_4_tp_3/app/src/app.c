#include <ao_led.h>
#include <ao_led.h>
#include <ao_ui.h>
#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "task_button.h"

ao_ui_handle_t ao_ui;

void app_init(void){

	ao_ui_init(&ao_ui);
	ao_led_init ();

	BaseType_t status;
	status = xTaskCreate(task_button, "task_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);

	LOGGER_INFO("Application successfully initialized");

	cycle_counter_init();
}
