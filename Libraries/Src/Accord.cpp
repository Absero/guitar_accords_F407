#include "Accord.h"

Accord::Note::Note(uint32_t f, float delayN) {
	this->delayN = delayN;
	if (f > 0) {
		N = (uint32_t) (SAMPLING_FREQ / f - 0.5);
		wavetable.resize(N);
		for (uint32_t i = 0; i < N; i++)
			wavetable[i] = randInRange(-10000, 10000);
	} else {
		N = 1;
		wavetable.resize(N);
		for (uint32_t i = 0; i < N; i++)
			wavetable[i] = 0;
	}
}

float Accord::Note::GetNext() {
	if (delayCounter < delayN) {
		delayCounter++;
		return 0;
	}

	previousValue = wavetable[current_wav_i] = (wavetable[current_wav_i] + previousValue) / 2;
	current_wav_i = (current_wav_i + 1) % N;

	return previousValue;
}

Accord::Accord(std::vector<float> freqs, float time, float delay) {
	uint32_t noteDelayN = SAMPLING_FREQ * delay;
	uint32_t currentDelay = 0;
	totalNum = time * SAMPLING_FREQ;
	for (float f : freqs) {
		noteList.push_back(std::move(std::make_unique<Note>(f, currentDelay)));
		currentDelay += noteDelayN;
	}
}

float Accord::GetNext() {
	float value = 0;
	for (uint8_t i = 0; i < noteList.size(); i++)
		value += noteList[i]->GetNext();

	if (++currentNum >= totalNum) reset = true;

	return value / noteList.size();
}
