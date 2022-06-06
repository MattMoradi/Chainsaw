#include <iostream>
#include <SFML/Audio.hpp>
#include<SFML/Window/Mouse.hpp>

#include "RtMidi.h"

extern sf::Sound sound;
//extern RtMidiIn* midiin;
//extern int status;

class Synth
{
public:
	static void player(short samples[], int sampleCount);
	static void sine(double freq);
	static void square(double freq);
	static void saw(double freq);
	static void triangle(double freq);

	static void listener();
};