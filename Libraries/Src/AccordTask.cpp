#include "main.h"
//#include "FreeRTOS.h"
#include "cmsis_os.h"

void StartAccordTask(void *argument) {

	for (;;) {
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	}
}
