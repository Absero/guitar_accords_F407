#ifndef NOTE_H_
#define NOTE_H_

#include "main.h"
#include <vector>

#define randInRange(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))

class Note {
private:
	std::vector<int> wavetable;
	uint32_t currentNum = 0;
	int32_t previousValue = 0;
	uint32_t current_wav_i = 0;
	uint32_t N;
	uint32_t totalNum;

public:
	bool reset = false;

	int32_t GetNext();

	Note(uint32_t f, uint32_t t);
	virtual ~Note() = default;
};

#endif /* NOTE_H_ */
