#include "header.h"
#include <signal.h>
#include <cstdlib>
#include<Windows.h>
//#include "RtMidi.h"
//#include <SFML/Audio.hpp>
#include <signal.h>

const int SAMPLE_RATE = 48000;
const short CHANNELS = 1;

//int root = 48;
//int beats = 8;
//double bpm = 90.0;
double ramp = 0.5;
double accent = 5.0;
double volume = 8.0;

double decay = 0.0;
sf::Sound sound;

// MIDI Testing
int status;
int nBytes;
RtMidiIn* midiin;
std::vector<unsigned char> message;

void Synth::player(short samples[], int sampleCount)
{
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], sampleCount, CHANNELS, SAMPLE_RATE);

    sound.setBuffer(buffer);
    sound.play();
    //sound.setPitch(640);      // remove right
    do{
        midiin->getMessage(&message);

        std::cout << status << std::endl;
        sound.setLoop(true);
    } while (sf::Mouse::isButtonPressed(sf::Mouse::Left) || (int)message[0] == 152);
    std::cout << "CALLED" << std::endl;
    //Sleep(1000);
}

void Synth::sine(double freq)
{
    const double sineAMP = 32767 * pow(10, (-6 * (10 - volume)) / 20);
    //const short sineAMP = std::round(0.25 * 32767);
    //std::cout << sineAMP << std::endl;;
    const int samples = SAMPLE_RATE;
    short sine[SAMPLE_RATE];

    // Sine Wave Oscillator
    for (int i = 0; i < samples; ++i)
    {
        sine[i] = (short)(sineAMP * sin((2 * 3.1415 * freq * i) / SAMPLE_RATE));

        /*
        double envelope = ramp * samples;

        if (i < envelope)
            sine[i] = sine[i] * (i / envelope);

        if (i >= envelope)
            sine[i] = sine[i] * ((samples - i) / envelope);
        */
    }
    player(sine, samples);
}

void Synth::square(double freq)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - accent)) / 20);
    //const short squareAMP = std::round(0.8 * 32767); 
    const int samples = SAMPLE_RATE;
    short square[SAMPLE_RATE];

    // Square Wave Oscillator
    for (int i = 0; i < samples; ++i)
    {
        int spc = (SAMPLE_RATE / freq);
        int cycle = i % spc;
        int halfcycle = spc / 2;
        if (cycle > halfcycle)
            square[i] = -squareAMP;
        else
            square[i] = squareAMP;

        /*
        double envelope = ramp * samples;

        if(i < envelope)
            square[i] = square[i] * (i / envelope);

        if(i >= envelope)
            square[i] = square[i] * ((samples - i) / envelope);
        */
    }
    player(square, samples);
}

void Synth::saw(double freq)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - accent)) / 20);
    const int samples = SAMPLE_RATE;
    short saw[SAMPLE_RATE];

    for (int i = 0; i < samples; ++i)
    {
        short sine = (short)(squareAMP * sin((2 * 3.1415 * freq * i) / SAMPLE_RATE));
        if (sine >= 0)
            saw[i] += sine;
        else
            saw[i] -= sine;
    }

    player(saw, samples);
}

void Synth::triangle(double freq)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - accent)) / 20);
    const int samples = SAMPLE_RATE;
    short saw[SAMPLE_RATE];

    for (int i = 0; i < samples; ++i)
    {
        //saw[i] = freq - floor(freq);
        //saw[i] = 1.0 - fabs(fmod(i, 2.0) - 1.0);

        //double sinevalue = sin(2 * 3.1415 * freq);
        //saw[i] = (2 * squareAMP) / 3.1415 * asin(sinevalue) + 128;

        //saw[i] = asin(cos(i)) / 1.0;

        short sine = (short)(squareAMP * sin((2 * 3.1415 * freq * i) / SAMPLE_RATE));
        if (sine >= 0)
            saw[i] += (sine - 0);
        else
            saw[i] = 0;
        //std::cout << saw[i] << std::endl;
    }

    player(saw, samples);
}


