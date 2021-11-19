#ifndef NOTE_H_
#define NOTE_H_

#include "main.h"
#include <vector>
#include <memory>

#define randInRange(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))

class Accord {
private:
	class Note {
	private:
		std::vector<int16_t> wavetable;
		int32_t previousValue = 0;
		uint16_t N, current_wav_i = 0, delayN = 0, delayCounter = 0;

	public:
		float GetNext();
		Note(uint32_t f, float delayN = 0);
	};

	uint32_t currentNum = 0;
	uint32_t totalNum;
	std::vector<std::unique_ptr<Note>> noteList;
public:
	bool reset = false;

	Accord(std::vector<float> freqs, float time, float delay = 0.01);
	float GetNext();

};

#endif /* NOTE_H_ */
