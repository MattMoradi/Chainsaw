#include "Synth.h"
#include "RtMidi.h"
#include <cstdlib>
//#include<Windows.h>
#include <SFML/Audio.hpp>

const int SAMPLE_RATE = 48000;
const short CHANNELS = 1;

sf::Sound sound;

// MIDI
RtMidiIn* inputMIDI;
std::vector<unsigned char> message;

void Synth::player(short samples[], int sampleCount, bool isMIDI, struct Midi)
{
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samples[0], sampleCount, CHANNELS, SAMPLE_RATE);
    sound.setBuffer(buffer);
    sound.play();

    // oh man does this loop have a history
    while(isMIDI)
    {
        if (inputMIDI)
        {
            inputMIDI->getMessage(&message);

            // for whatever reason vectors crash if nBytes is null
            if (message.size())
            {
                // only key release works for some reason
                if ((int)message[0] == 136)
                    isMIDI = false;
            }
        }

        // setloop only loops for one iteration apparently
        sound.setLoop(true);
    }

    while (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMIDI)
        sound.setLoop(true);

    //Sleep(1000);
}

void Synth::sine(double freq, bool isMIDI, Midi data)
{
    const double sineAMP = 32767 * pow(10, (-6 * (10 - data.volume)) / 20);
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
    player(sine, samples, isMIDI, data);
}

void Synth::square(double freq, bool isMIDI, Midi data)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - data.volume)) / 20);
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
    player(square, samples, isMIDI, data);
}

void Synth::saw(double freq, bool isMIDI, Midi data)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - data.volume)) / 20);
    const int samples = SAMPLE_RATE;
    short saw[SAMPLE_RATE];

    for (int i = 0; i < samples; ++i)
    {
        short sine = (short)(squareAMP * sin((2 * 3.1415 * freq * i) / SAMPLE_RATE));
        if (sine >= 0)
            saw[i] += (sine - 0);
        else
            saw[i] = 0;
    }

    player(saw, samples, isMIDI, data);
}

void Synth::triangle(double freq, bool isMIDI, Midi data)
{
    const double squareAMP = 32767 * pow(10, (-6 * (10 - data.volume)) / 20);
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

    player(saw, samples, isMIDI, data);
}

void Synth::listener(Midi data)
{
    //HighlightKey(70, true);

    inputMIDI = new RtMidiIn();

    unsigned int nPorts = inputMIDI->getPortCount();
    if (nPorts == 0)
    {
        std::cout << "No ports detected!\n";
        delete inputMIDI;
        return;
    }

    inputMIDI->openPort(0);

    // The RtMidi docs say not to ignore this but I'm gonna do it anyway
    inputMIDI->ignoreTypes(true, true, true);

    std::cout << "Listening for MIDI from port...\n";
    while (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        inputMIDI->getMessage(&message);
        if (message.size())
        {
            std::cout << "Key: " << " = " << (int)message[1] << ", " << std::endl;
            std::cout << "Status: " << " = " << (int)message[0] << ", " << std::endl << std::endl;
    
            if ((int)message[0] == 152)
            {
                HighlightKey(message[1], true);
                //sine(440 * pow(2, ((float)message[1] - 69) / 12), true, data);
                std::cout << "OUT" << std::endl;
            }
        }
    }
    delete inputMIDI;
}
