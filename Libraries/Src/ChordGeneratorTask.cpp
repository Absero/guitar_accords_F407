#include "ChordGeneratorTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "AccordTask.h"
#include <cstring>
#include "queue.h"

#define BPM 112*2
const float T_STEP = 1.0 / (BPM / 60.0);

QueueHandle_t chordQueue;
// @formatter:off

// https://tabs.ultimate-guitar.com/tab/brenda-lee/rockin-around-the-christmas-tree-chords-991152
//#define STRUM(x) {x, T_STEP*3}, {x, T_STEP*2}, {x, T_STEP}
//accordInfoString_t gAccordSequence[] = {
//		STRUM("1G"),	STRUM("1Em"),	STRUM("1C"),	STRUM("1D"),
//		STRUM("1G"),	STRUM("1G"),	STRUM("1G"), 	STRUM("1G"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"),
//		STRUM("1Am7"),	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1G"), 	STRUM("1G"),
//		STRUM("1G"),	STRUM("1G"), 	STRUM("1G"), 	STRUM("1G"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"), 	STRUM("1D7"),
//		STRUM("1Am7"),	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1Am7"), 	STRUM("1D7"), 	STRUM("1G"), 	STRUM("1G"),
//		STRUM("1C"),	STRUM("1C"), 	STRUM("1C"), 	STRUM("1C"), 	STRUM("2Bm"), 	STRUM("2Bm"), 	STRUM("2Bm"), 	STRUM("2Bm"),
//		STRUM("1Am"),	STRUM("1Emmaj7"), STRUM("1Em7"), STRUM("1Em7"), STRUM("1A7"), 	STRUM("1A7"), 	STRUM("1D7"),	STRUM("1D7"),
//		{"0",2}
//};

// https://tabs.ultimate-guitar.com/tab/misc-christmas/jingle-bells-chords-273336
//#define STRUM(x) {x, T_STEP}, {x, T_STEP*0.5}, {x, T_STEP*1.5}
//#define STRUM2(x) STRUM(x),STRUM(x)
//accordInfoString_t gAccordSequence[] = {
//		STRUM2("G"),STRUM("C"),STRUM("G"),
//		STRUM("C"),STRUM("G"),
//		STRUM("A7"),STRUM("D7"),
//		STRUM2("G"),STRUM("C"),STRUM("G"),
//		STRUM("C"),STRUM("G"),
//		STRUM("D7"),STRUM("G"),
//		{"0",2}
//};

// https://tabs.ultimate-guitar.com/tab/rihanna/stay-chords-1195964
#define STRUM(x1,x2,x3,x4) {x1, T_STEP*2}, {x2, T_STEP*2}, {x3, T_STEP*2}, {x4, T_STEP*2}
accordInfoString_t gAccordSequence[] = {
		STRUM("C","C","C","Dm"),	STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),
		STRUM("C","C","C","Dm"),	STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),
		STRUM("C","C","C","Dm"),	STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),
		STRUM("C","C","C","Dm"),	STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),
		STRUM("F","F","F","Dm"),	STRUM("Dm","Dm","Dm","Am"),	STRUM("Am","Am","Am","Am"),	STRUM("Am","Am","Am","F"),
		STRUM("F","F","F","Dm"),	STRUM("Dm","Dm","Dm","G"),	STRUM("G","G","G","G"),		STRUM("G","G","G","C"),
		STRUM("C","C","C","Dm"),	STRUM("Dm","Dm","Dm","Am"),	STRUM("Am","Am","Am","F"),	STRUM("F","F","F","C"),
		STRUM("C","C","C","Dm"),	STRUM("Dm","Dm","Dm","Am"),	STRUM("Am","Am","Am","F"),	STRUM("F","F","F","C"),
		STRUM("Am","Am","Am","F"),	STRUM("F","F","F","C"),		STRUM("C","C","C","Dm"),	STRUM("Am","Am","Am","Am7"),
		STRUM("Am","Am","Am","Am7"),STRUM("Am","Am","Am","Am7"),STRUM("G","G","G","G"),		STRUM("G","G","G","C"),
		{"0", 2}
};
// @formatter:on

void StartDefaultTask(void *argument) {
	accordInfo_t accI;
	uint8_t indexas = 0;

	chordQueue = xQueueCreate(3, sizeof(accordInfo_t));
	/* Infinite loop */
	for (;;) {
		strcpy(accI.chord, gAccordSequence[indexas].chord.c_str());
		accI.time = gAccordSequence[indexas].time;
		accI.delay = 0.005;
		xQueueSend(chordQueue, (void* ) &accI, portMAX_DELAY);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		indexas = (indexas + 1) % (sizeof(gAccordSequence) / sizeof(accordInfoString_t));
	}
}
