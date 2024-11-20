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
ao_led_handle_t ao_red_led_h;
ao_led_handle_t ao_green_led_h;
ao_led_handle_t ao_blue_led_h;

void app_init(void){

	ao_ui_init(&ao_ui);
	ao_led_init (&ao_red_led_h, LED_RED_PORT, LED_RED_PIN);
	ao_led_init (&ao_green_led_h, LED_GREEN_PORT, LED_GREEN_PIN);
	ao_led_init (&ao_blue_led_h, LED_BLUE_PORT, LED_BLUE_PIN);

	BaseType_t status;
	status = xTaskCreate(task_button, "task_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);

	LOGGER_INFO("Application successfully initialized");

	cycle_counter_init();
}
