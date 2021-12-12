#include "AccordTask.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_audio.h"
#include <memory>
#include <string>
#include <stdlib.h>

#include "Accord.h"

extern osThreadId_t AccordTaskHandle;
static std::unique_ptr<Accord> gAccord;

//std::string gChords = "Am|C|D|F|Am|C|D|F||";
static uint16_t position = 0;
//std::string substringas = "";
//int16_t temp;

static std::vector<std::string> gChords = { "C", "D", "E", "F", "G", "A" };

void fillBuffer(uint8_t part);

void StartAccordTask(void *argument) {
	uint8_t partN = 0;
	if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, SAMPLING_FREQ) != AUDIO_OK) Error_Handler();

	gAccord.reset(new Accord( { 0 }, 5, 0.01));
	position++;

	fillBuffer(0);
	fillBuffer(1);

	BSP_AUDIO_OUT_Play((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);

	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if (partN == 1) BSP_AUDIO_OUT_ChangeBuffer((uint16_t*) gAudioBuffer, BUFF_SIZE * 2);

		fillBuffer(partN);

		partN = !partN;
	}
}

void fillBuffer(uint8_t part) {
	if (gAccord->reset) {
//		TODO paimt is eiles struktura

		gAccord.reset(new Accord(CHORD[gChords[position]], 2, 0.01));
		position = position + 1 >= (uint16_t) gChords.size() ? 0 : position + 1;
	}

	for (int i = (part ? BUFF_SIZE / 2 : 0); i < (part ? BUFF_SIZE : BUFF_SIZE / 2); i++) {
		gAudioBuffer[(i * 2)] = gAudioBuffer[(i * 2) + 1] = (int16_t) gAccord->GetNext();
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
