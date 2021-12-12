#include "ChordGeneratorTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "AccordTask.h"
#include <cstring>
#include "queue.h"

#define BPM 141*2
const float T_STEP = 1.0 / (BPM / 60.0);

QueueHandle_t chordQueue;
// @formatter:off

// https://tabs.ultimate-guitar.com/tab/brenda-lee/rockin-around-the-christmas-tree-chords-991152
#define STRUM(x) {x, T_STEP*3}, {x, T_STEP*2}, {x, T_STEP}
accordInfoString_t gAccordSequence[] = {
		STRUM("1G"),	STRUM("1Em"),	STRUM("1C"),	STRUM("1D"),
		STRUM("1G"),	STRUM("1G"),	STRUM("1G"), 	STRUM("1G"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"),
		STRUM("1Am7"),	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1G"), 	STRUM("1G"),
		STRUM("1G"),	STRUM("1G"), 	STRUM("1G"), 	STRUM("1G"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"),
		STRUM("1Am7"),	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1G"), 	STRUM("1G"),
		STRUM("1C"),	STRUM("1C"), 	STRUM("1C"), 	STRUM("1C"), 	STRUM("2Bm"), 	STRUM("2Bm"), 	STRUM("2Bm"), 	STRUM("2Bm"),
		STRUM("1Am"),	STRUM("1Emmaj7"), STRUM("1Em7"), STRUM("1Em7"), STRUM("1A7"), 	STRUM("1A7"), 	STRUM("1D7"),	STRUM("1D7"),
		{"0",2}
};

// https://tabs.ultimate-guitar.com/tab/imagine-dragons/demons-chords-1148110
//#define STRUM(x) {x, T_STEP},{x, T_STEP},{x, T_STEP},{x, T_STEP},{x, T_STEP},{x, T_STEP},{x, T_STEP},{x, T_STEP}
//accordInfoString_t gAccordSequence[] = {
//		STRUM("D"),
//		STRUM("A"),
//		STRUM("2Bm"),
//		STRUM("G"),
//		STRUM("D"),
//		STRUM("A"),
//		STRUM("2Bm"),
//		STRUM("G"),
//		STRUM("D"),
//		{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"Bm", T_STEP},
//		STRUM("2Bm"),
//		STRUM("G"),
//		STRUM("D"),
//		{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"A", T_STEP},{"Bm", T_STEP},
//		STRUM("2Bm"),
//		STRUM("G"),
//		{"0",2}
//};

// @formatter:on

void StartDefaultTask(void *argument) {
	accordInfo_t accI;
	uint8_t indexas = 0;

	chordQueue = xQueueCreate(3, sizeof(accordInfo_t));
	/* Infinite loop */
	for (;;) {
		strcpy(accI.chord, gAccordSequence[indexas].chord.c_str());
		accI.time = gAccordSequence[indexas].time;
		accI.delay = 0.01;
		xQueueSend(chordQueue, (void* ) &accI, portMAX_DELAY);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		indexas = (indexas + 1) % (sizeof(gAccordSequence) / sizeof(accordInfoString_t));
	}
}
