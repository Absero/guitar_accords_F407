#include "AccordTask.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_audio.h"
#include <memory>
#include <string>
#include <stdlib.h>
#include "queue.h"

extern osThreadId_t AccordTaskHandle;
int16_t gAudioBuffer[BUFF_SIZE * 2] = { 0 };
static std::unique_ptr<Accord> gCurrentAccord;
extern QueueHandle_t chordQueue;

void fillBuffer(uint8_t part);

void StartAccordTask(void *argument) {
	uint8_t partN = 0;
	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, SAMPLING_FREQ) != AUDIO_OK) Error_Handler();

	gCurrentAccord.reset(new Accord( { 0 }, 1, 0));

	fillBuffer(0);
	fillBuffer(1);

	BSP_AUDIO_OUT_Play((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);

	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if (partN == 1) BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);
		fillBuffer(partN);

		partN ^= 1;
	}
}

void fillBuffer(uint8_t part) {
	if (gCurrentAccord->reset) {
		accordInfo_t newAccord;
		if (xQueueReceive(chordQueue, (void*) &newAccord, portMAX_DELAY)) {
			gCurrentAccord.reset(new Accord(CHORD[newAccord.chord],
				newAccord.time,
				newAccord.delay));
		}
	}

	for (int i = (part ? BUFF_SIZE / 2 : 0); i < (part ? BUFF_SIZE : BUFF_SIZE / 2); i++) {
		gAudioBuffer[(i * 2)] = gAudioBuffer[(i * 2) + 1] = (int16_t) gCurrentAccord->GetNext();
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
