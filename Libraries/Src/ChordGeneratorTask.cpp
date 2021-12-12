#include "ChordGeneratorTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "AccordTask.h"
#include <cstring>
#include "queue.h"

#define T_STEP 0.25

QueueHandle_t chordQueue;
// @formatter:off
accordInfoString_t gAccordSequence[] = { { "G", 0.5 }, { "D7", 1 }, { "Am7", 0.5 }, { "D7", 1 } };
// @formatter:on

void StartDefaultTask(void *argument) {
	accordInfo_t accI;
	uint8_t indexas = 0;

	chordQueue = xQueueCreate(3, sizeof(accordInfo_t));
	/* Infinite loop */
	for (;;) {
		strcpy(accI.chord, gAccordSequence[indexas].chord.c_str());
		accI.time = gAccordSequence[indexas].time;
		accI.delay = 0.02;
		xQueueSend(chordQueue, (void* ) &accI, portMAX_DELAY);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		indexas = (indexas + 1) % (sizeof(gAccordSequence) / sizeof(accordInfoString_t));
	}
}
