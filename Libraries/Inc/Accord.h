#ifndef NOTE_H_
#define NOTE_H_

#include "main.h"
#include <vector>
#include <memory>
#include <map>

#define randInRange(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))

typedef struct {
	char chord[10];
	float time;
	float delay;
} accordInfo_t;

typedef struct {
	std::string chord;
	float time;
} accordInfoString_t;

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
// vienas fret padidina per +5,945880354%
// @formatter:off
static std::map<std::string, std::vector<float>> CHORD = {
		{ "G", { 98, 123, 147, 196, 247, 392} },
		{ "Em", {82, 123, 165, 196, 247, 329} },
		{ "C", { 131, 175, 196, 262, 329 } },
		{ "D", { 147, 220, 294, 370 } },
		{ "D7", {147, 220, 262, 370} },
		{ "Am7", {110, 165, 196, 262, 329} },
		{ "Bm", {123, 147, 247, 294, 370} },
		{ "Emmaj7", {123, 147, 247, 294, 370} },
		{ "Em7", {123, 147, 247, 294, 370} },
		{ "A7", {110, 165, 196, 277, 329} },
		{ "Am", {110, 165, 220, 262, 329} },
		{ "0", {0}}};
// @formatter:on
#endif /* NOTE_H_ */
