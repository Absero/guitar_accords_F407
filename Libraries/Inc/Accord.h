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
// one fret increases freq by +5,945880354% (1.0594588035432593)
#define FRET_INC 1.0594588035432593
// @formatter:off
static std::map<std::string, std::vector<float>> CHORD = {
		{ "A", { 110, 165, 220, 277, 329} },
		{ "B", { 123, 185, 247, 311, 370} },
		{ "C", { 131, 175, 196, 262, 329 } },
		{ "D", { 147, 220, 294, 370 } },
		{ "E", { 82, 123, 165, 208, 247, 329 } },
		{ "F", { 87, 131, 175, 220, 262, 349} },
		{ "G", { 98, 123, 147, 196, 247, 392} },
		{ "Am", {110, 165, 220, 262, 329} },
		{ "Bm", {123, 147, 247, 294, 370} },
		{ "Cm", {131, 196, 262, 311, 392} },
		{ "Dm", {147, 220, 294, 349} },
		{ "Em", {82, 123, 165, 196, 247, 329} },
		{ "Fm", {87, 131, 175, 208, 262, 349} },
		{ "Gm", {98, 147, 196, 233, 294, 392} },
		{ "A7", {110, 165, 196, 277, 329} },
		{ "B7", {123, 156, 220, 247, 370} },
		{ "C7", {131, 165, 233, 262, 329} },
		{ "D7", {147, 220, 262, 370} },
		{ "E7", {82, 23, 147, 208, 247, 329} },
		{ "F7", {87, 131, 156, 220, 262, 349} },
		{ "G7", {98, 123, 147, 196, 247, 349} },
		{ "Am7", {110, 165, 196, 262, 329} },
		{ "Em7", {123, 147, 247, 294, 370} },
		{ "1Fm", {87*FRET_INC, 131*FRET_INC, 175*FRET_INC, 208*FRET_INC, 262*FRET_INC, 349*FRET_INC} },
		{ "2Bm", {123*FRET_INC*FRET_INC, 147*FRET_INC*FRET_INC, 247*FRET_INC*FRET_INC, 294*FRET_INC*FRET_INC, 370*FRET_INC*FRET_INC} },

		{ "1G", {98*FRET_INC, 123*FRET_INC, 147*FRET_INC, 196*FRET_INC, 247*FRET_INC, 392*FRET_INC} },
		{ "1Em", {82*FRET_INC, 123*FRET_INC, 165*FRET_INC, 196*FRET_INC, 247*FRET_INC, 329*FRET_INC} },
		{ "1C", {131*FRET_INC, 175*FRET_INC, 196*FRET_INC, 262*FRET_INC, 329*FRET_INC } },
		{ "1D", {147*FRET_INC, 220*FRET_INC, 294*FRET_INC, 370*FRET_INC } },
		{ "1D7", {147*FRET_INC, 220*FRET_INC, 262*FRET_INC, 370*FRET_INC} },
		{ "1Am7", {110*FRET_INC, 165*FRET_INC, 196*FRET_INC, 262*FRET_INC, 329*FRET_INC} },
		{ "1Emmaj7", {123*FRET_INC, 147*FRET_INC, 247*FRET_INC, 294*FRET_INC, 370*FRET_INC} },
		{ "1Em7", {123*FRET_INC, 147*FRET_INC, 247*FRET_INC, 294*FRET_INC, 370*FRET_INC} },
		{ "1A7", {110*FRET_INC, 165*FRET_INC, 196*FRET_INC, 277*FRET_INC, 329*FRET_INC} },
		{ "1Am", {110*FRET_INC, 165*FRET_INC, 220*FRET_INC, 262*FRET_INC, 329*FRET_INC} },

		{ "0", {0}}};
// @formatter:on
#endif /* NOTE_H_ */

