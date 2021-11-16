#include "Note.h"

Note::Note(uint32_t f, uint32_t t) {
	N = (uint32_t) round(SAMPLING_FREQ / f - 0.5);
	totalNum = t * SAMPLING_FREQ;
}

