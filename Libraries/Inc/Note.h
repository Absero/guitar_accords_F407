#ifndef NOTE_H_
#define NOTE_H_

#include "main.h"

class Note {
private:
public:
	uint32_t currentNum = 0;
	int32_t previousValue = 0;
	uint32_t current_wav_i = 0;
	bool reset = false;
	uint32_t N;
	uint32_t totalNum;

//	Note() = default;
	Note(uint32_t f, uint32_t t);
	virtual ~Note() = default;
};

#endif /* NOTE_H_ */
