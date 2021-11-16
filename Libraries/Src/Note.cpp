#include "Note.h"

Note::Note(uint32_t f, uint32_t t) {
	N = (uint32_t) round(SAMPLING_FREQ / f - 0.5);
	totalNum = t * SAMPLING_FREQ;
	wavetable.resize(N);
	for (uint32_t i = 0; i < N; i++)
		wavetable[i] = (int16_t) randInRange(-30000, 30000);
}

void Note::GetNextPart(float &destArray) {
}

