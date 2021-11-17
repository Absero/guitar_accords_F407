#ifndef NOTE_H_
#define NOTE_H_

#include "main.h"
#include <vector>
#include <memory>

#define randInRange(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))

class Note {
private:
	std::vector<int> wavetable;
	uint32_t currentNum = 0;
	int32_t previousValue = 0;
	uint32_t current_wav_i = 0;
	uint32_t N;
	uint32_t totalNum;
	uint32_t delayN = 0;
	uint32_t delayCounter = 0;

public:
	bool reset = false;

	int32_t GetNext();

	Note(uint32_t f, uint32_t t, float delayN = 0);
};

class Accord {
private:
	std::vector<std::unique_ptr<Note>> noteList;
public:
	bool reset = false;

	Accord(std::vector<float> freqs, float time, float delay = 0.01);
	int32_t GetNext();

};

#endif /* NOTE_H_ */
