#include <iostream>
#include<SFML/Window/Mouse.hpp>
#include<vector>
const int NUM_KEYS = 37;

void HighlightKey(int midiKey, bool highlight); 

/*
double ramp = 0.5;
double accent = 5.0;
double volume = 8.0;
*/

struct Midi
{
	float ramp = 0.0f; // 0.0 - 0.5
	int octave = 1; //  1 - 8 
	float amplitude = 261.0f; //27.5 - 12543.85
	float volume = 5.0f; // 0.0 - 10.0
	int midiNoteNumber = 60; // 21 - 115
	float bpm = 60.0f; // 0 - sizeof(float)
	std::vector<int> midiRectValues; // holds the midi key value for each rectangle

};

class Synth
{
public:
	static void player(short samples[], int sampleCount, bool isMIDI);
	static void sine(double freq, bool isMIDI);
	static void square(double freq, bool isMIDI);
	static void saw(double freq, bool isMIDI);
	static void triangle(double freq, bool isMIDI);

	static void listener();
};