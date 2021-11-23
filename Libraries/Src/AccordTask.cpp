#include "AccordTask.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_audio.h"
#include <memory>

#include "Accord.h"

extern osThreadId_t AccordTaskHandle;

#define newaccord new Accord( {80}, 3, 0.01)
std::unique_ptr<Accord> gAccord;

void StartAccordTask(void *argument) {
	uint8_t partN = 0;
	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, SAMPLING_FREQ) != AUDIO_OK) Error_Handler();

	gAccord.reset(newaccord);

	for (int i = 0; i < BUFF_SIZE; i++) {
		gAudioBuffer[(i * 2)] = gAudioBuffer[(i * 2) + 1] = (int16_t) gAccord->GetNext();
	}

	BSP_AUDIO_OUT_Play((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);

	for (;;) {
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
		if (partN == 1) BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);
		if (gAccord->reset) gAccord.reset(newaccord);

		for (int i = (partN ? BUFF_SIZE / 2 : 0); i < (partN ? BUFF_SIZE : BUFF_SIZE / 2); i++) {
			gAudioBuffer[(i * 2)] = gAudioBuffer[(i * 2) + 1] = (int16_t) gAccord->GetNext();
		}

		partN = !partN;
	}
}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR((TaskHandle_t) AccordTaskHandle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*** Back to Buffer beginning ***/
void BSP_AUDIO_OUT_TransferComplete_CallBack(void) {
	BSP_AUDIO_OUT_HalfTransfer_CallBack();
}
