#include "Note.h"

Note::Note(uint32_t f, uint32_t t, float delayN) {
	this->delayN = delayN;
	N = (uint32_t) round(SAMPLING_FREQ / f - 0.5);
	totalNum = t * SAMPLING_FREQ;
	wavetable.resize(N);
	for (uint32_t i = 0; i < N; i++)
		wavetable[i] = (int16_t) randInRange(-30000, 30000);
}

int32_t Note::GetNext() {
	if (delayCounter < delayN) {
		delayCounter++;
		return 0;
	}

	previousValue = wavetable[current_wav_i] =
			((wavetable[current_wav_i] / 2) + (previousValue / 2));
	current_wav_i++;
	current_wav_i %= N;

	currentNum++;
	if (currentNum >= totalNum) reset = true;

	return previousValue;
}

float noteDelay = 0.01;
Accord::Accord(std::vector<float> freqs, float time, float delay) {
	uint32_t noteDelayN = (uint32_t) round(SAMPLING_FREQ * delay);
	uint32_t currentDelay = 0;
	for (float f : freqs) {
		Note *n = new Note(f, time, currentDelay);
		noteList.push_back(*n);
		currentDelay += noteDelayN;
	}
}

int32_t Accord::GetNext() {
	int32_t value = 0;
	for (Note &note : noteList) {
		value += (note.GetNext() / (float) noteList.size());

		if (note.reset) reset = note.reset; //TODO: skaiciuot akorde, ne natoj

	}

	return value;
}
