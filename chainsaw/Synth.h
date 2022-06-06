#include <iostream>
#include<SFML/Window/Mouse.hpp>

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