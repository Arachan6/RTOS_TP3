#ifndef TASK_LED_H_
#define TASK_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "app.h"

/********************** forward declarations *********************************/


/********************** typedef **********************************************/


/********************** external functions declaration ***********************/

void ao_led_init();

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/
