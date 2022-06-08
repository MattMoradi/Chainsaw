#include "Synth.h"
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<SFML/Graphics.hpp>
#include"Gui.h"

using std::cout;
using std::endl;

void MapMidiValuesToPianoKeys();
bool ClickedKey(sf::Event& event);
bool ClickedKnob(sf::Sprite knob[], sf::Event& event, Midi& midi);
bool ClickedWaveType(sf::RectangleShape wave[], sf::Event& event);
bool ClickedEnvelopeSwitch(sf::Event& event);
void UpdateValuesFromKnob(int index, int value, sf::Sprite knob[], Midi& midi);
bool UpdateOctave(int amount);
bool UpdateVolume(int amount);
bool UpdateRamp(int value);

Midi midi;
Gui gui;

int main()
{
	sf::RenderWindow window(sf::VideoMode(590, 320), "Chainsaw v1.1");
	gui.GenerateKeyboard(window);
	MapMidiValuesToPianoKeys();
	gui.LoadAssetsFromFile();
	gui.InitalizeGui();

	while (window.isOpen())
	{
		sf::Event event;
		window.setSize(sf::Vector2u(590, 320));
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (!ClickedKnob(gui.drawings.knob, event, midi))
					cout << "Mouse not on knob\n";
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!ClickedKey(event))
						cout << "Mouse out of rect\n";
					if (!ClickedWaveType(gui.drawings.waveBackground, event))
						cout << "mouse not on wave\n";
					if (!ClickedEnvelopeSwitch(event))
						cout << "mouse not on envelope\n";
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					cout << "right mouse clicked\n";
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				for (int i = 0; i < 22; i++)
					gui.drawings.pianoKey[i].setFillColor(sf::Color::White);
				for (int i = 22; i < NUM_KEYS; i++)
					gui.drawings.pianoKey[i].setFillColor(sf::Color::Black);
			}
		}

		window.clear();
		gui.Draw(window);
		window.display();

		//Synth::listener(midi);
	}
}

//Check if user click on a key
bool ClickedKey(sf::Event& event)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);
	for (int i = NUM_KEYS - 1; i >= 0; i--)
	{
		if (gui.drawings.pianoKey[i].getGlobalBounds().contains(mousePos))
		{
			cout << "Mouse is inside rect " << i << endl;
			cout << "Playing midi note: " << midi.midiRectValues.at(i) << endl;

			float freq = 440 * static_cast<float>(pow(2.0, ((static_cast<float>(midi.midiRectValues.at(i) - 69) / 12))));
			HighlightKey(midi.midiRectValues.at(i), true);
			switch (midi.waveType)
			{
				case sine:
				{
					Synth::sine(freq, false, midi);
					break;
				}
				case saw:
				{
					Synth::saw(freq, false, midi);
					break;
				}
				case triangle:
				{
					Synth::triangle(freq, false, midi);
					break;
				}
				case square:
				{
					Synth::square(freq, false, midi);
					break;
				}
				case whiteNoise:
				{
					Synth::noise(freq,false,midi);
					break;
				}
				default:
					break;
			}
			return true;
		}
	}
	return false;
}

//Checks if the user clicked a knob.
bool ClickedKnob(sf::Sprite knob[], sf::Event& event, Midi& midi)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);

	for (int i = 0; i < NUM_KNOBS; i++)
	{
		if (knob[i].getGlobalBounds().contains(mousePos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				cout << "Left clicked on knob " << i << endl;
				UpdateValuesFromKnob(i, -1, knob, midi);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				cout << "right clicked on knob " << i << endl;
				UpdateValuesFromKnob(i, 1, knob, midi);
			}
			else
				cout << "ERROR: ClickedKnob function, mouse was not pressed with left or right mouse button\n";
			return false;
		}
	}
	return false;
}

bool ClickedWaveType(sf::RectangleShape wave[], sf::Event& event)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);
	for (int i = 0; i < 5; i++)
	{
		if (wave[i].getGlobalBounds().contains(mousePos))
		{
			cout << "Clicked wave " << i << endl;
			midi.waveType = waveEnum(i);
			if (midi.waveType == 0)
				cout << "Changed to sinewave\n";
			else if (midi.waveType == 1)
				cout << "Changed to saw wave\n";
			else if (midi.waveType == 2)
				cout << "changed to triangle wave\n";
			else if (midi.waveType == 3)
				cout << "changed to squre wave\n";
			else if (midi.waveType == 4)
				cout << "changed to white noise wave\n";
			else
				cout << "ERROR: did not properly set wave type\n";

			wave[i].setFillColor(sf::Color(222, 116, 44));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for (int j = 0; j < 5; j++)
				{
					if (i != j)
						wave[j].setFillColor(sf::Color::White);
				}
			}
			else
				cout << "oopsie doopsie in wave type\n";
			return false;
		}
	}
	return false;
}

//Checks if user clicked on the envelope switch
bool ClickedEnvelopeSwitch(sf::Event& event)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);
	
		//check if user clicked on evelope switch
		if (gui.drawings.envelopeSwitchSquare.getGlobalBounds().contains(mousePos) || gui.drawings.envelopeSwitchCircle[0].getGlobalBounds().contains(mousePos) || 
			gui.drawings.envelopeSwitchCircle[1].getGlobalBounds().contains(mousePos))
		{
			cout << "Clicked envelope switch " << endl;

			//check if envelope is currently being used
			if (midi.isUsingEnvelope)
			{
				midi.isUsingEnvelope = false;
				gui.drawings.envelopeStatusText.setString("ON");
				gui.drawings.envelopeStatusText.setPosition(500, 133);
				gui.drawings.envelopeStatusCircle.setPosition(526, 128);
				gui.drawings.envelopeSwitchSquare.setFillColor(sf::Color(222, 116, 44));
				gui.drawings.envelopeSwitchCircle[0].setFillColor(sf::Color(222, 116, 44));
				gui.drawings.envelopeSwitchCircle[1].setFillColor(sf::Color(222, 116, 44));
			}
			else
			{
				midi.isUsingEnvelope = true;
				gui.drawings.envelopeStatusText.setString("OFF");
				gui.drawings.envelopeStatusText.setPosition(515, 133);
				gui.drawings.envelopeStatusCircle.setPosition(490, 128);
				gui.drawings.envelopeSwitchSquare.setFillColor(sf::Color::Black);
				gui.drawings.envelopeSwitchCircle[0].setFillColor(sf::Color::Black);
				gui.drawings.envelopeSwitchCircle[1].setFillColor(sf::Color::Black);
			}
			return true;
		}
	return false;
}

//Update midi values from the knobs if user clicks on them
void UpdateValuesFromKnob(int index, int value, sf::Sprite knob[], Midi& midi)
{
	switch (index)
	{
	case 0:
	{
		if (UpdateOctave(value))
		{
			cout << "Updating octave and rotating knob!\n";

			knob[0].rotate(50.0f * value);
		}
		else
			cout << "Did not update or rotate octave knob\n";
		break;
	}
	case 1:
	{
		if (UpdateVolume(value))
		{
			cout << "Updating volume and rotating knob!\n";
			knob[1].rotate(25.0f * value);
		}
		else
			cout << "Did not update or rotate volume knob\n";
		break;
	}
	case 2:
	{
		if (UpdateRamp(value))
		{
			cout << "Updating ramp and rotating knob!\n";
			knob[2].rotate(50.0f * value);
		}
		else
			cout << "Did not update or rotate ramp knob\n";
		break;
	}
	default:
	{
		cout << "ERROR: update Values From knob functioN!\n";
		break;
	}
	}
}

//Changes the status that lets user know if midi is connected
void IsMidiConnected(bool connected)
{
	if (!connected)
	{
		gui.drawings.midiStatusSignal.setFillColor(sf::Color::Red);
		gui.drawings.midiSignalText.setString("NO MIDI");
	}
	else
	{
		gui.drawings.midiStatusSignal.setFillColor(sf::Color::Green);
		gui.drawings.midiSignalText.setString("MIDI CONNECTED");
	}
}

//highlight a key based on user input
void HighlightKey(int midiKey, bool highlight)
{
	bool found = false;
	for (int i = 0; i < midi.midiRectValues.size(); i++)
	{
		if (midi.midiRectValues.at(i) == midiKey)
		{
			cout << "Rectangle " << i << " has midi key" << midiKey << "\n";
			found = true;
			if (highlight)
				gui.drawings.pianoKey[i].setFillColor(sf::Color(222, 116, 44));
			else
			{
				if (i < 21)
					gui.drawings.pianoKey[i].setFillColor(sf::Color::White);
				else
					gui.drawings.pianoKey[i].setFillColor(sf::Color::Black);
			}
			i = midi.midiRectValues.size();
		}
	}
	if (!found)
		cout << "no midikey was found to highlight!\n";
}

//Update the octave of the keyboard.
bool UpdateOctave(int amount)
{
	if (amount > 0)
		amount = 1;
	else
		amount = -1;
	//check if value is a min or max and should not be updated
	if ((midi.octave == 1 && amount < 0) || (midi.octave == 6 && amount > 0))
	{
		cout << "did not update octave. Octave is at " << midi.octave << endl;
		return false;
	}

	midi.octave += amount;

	//update octave key display
	for (int i = 0; i < 4; i++)
		gui.drawings.cKeyText[i].setString("C" + std::to_string(midi.octave + i));

	//regenerate midi key map
	MapMidiValuesToPianoKeys();

	cout << "Octave is now: " << midi.octave << "\n";
	return true;
}

//update the midi volume
bool UpdateVolume(int amount)
{
	if ((amount < 0 && midi.volume == 0) || (amount > 0 && midi.volume == 10))
	{
		cout << "did not update volume. Volume is at " << midi.volume << endl;
		return false;
	}

	if (amount > 0)
		amount = 1;
	else
		amount = -1;

	midi.volume += amount;

	cout << "Volume is now: " << midi.volume << "\n";
	return true;
}

//update the ramp value
bool UpdateRamp(int amount)
{
	float val;
	if ((amount < 0 && midi.ramp < 0.001f) || (amount > 0 && midi.ramp > 0.0045))
	{
		cout << "did not update ramp. ramp is at " << midi.ramp << endl;
		return false;
	}

	if (amount > 0)
	{

		val = .001f;
		cout << "Rotating right\n";
	}
	else
	{
		val = -.001f;
		cout << "Rotating left\n";
	}
	if (midi.ramp < 0.0)
		midi.ramp = 0.0f;
	midi.ramp += val;
	cout << "ramp is now: " << midi.ramp << "\n";

	return true;
}

//fill the midiRectValue vector with its midi key value
void MapMidiValuesToPianoKeys()
{
	//find the first midi key
	int startingMidi = 24 + ((midi.octave - 1) * 12);
	midi.midiRectValues.clear();
	cout << "\n\nIn map midi\n";
	cout << "Octave is: " << midi.octave << "\n\n";
	midi.midiRectValues.push_back(startingMidi);
	midi.midiRectValues.push_back(startingMidi + 2);
	midi.midiRectValues.push_back(startingMidi + 4);
	midi.midiRectValues.push_back(startingMidi + 5);
	midi.midiRectValues.push_back(startingMidi + 7);
	midi.midiRectValues.push_back(startingMidi + 9);
	midi.midiRectValues.push_back(startingMidi + 11);
	midi.midiRectValues.push_back(startingMidi + 12);
	midi.midiRectValues.push_back(startingMidi + 14);
	midi.midiRectValues.push_back(startingMidi + 16);//10
	midi.midiRectValues.push_back(startingMidi + 17);
	midi.midiRectValues.push_back(startingMidi + 19);
	midi.midiRectValues.push_back(startingMidi + 21);
	midi.midiRectValues.push_back(startingMidi + 23);
	midi.midiRectValues.push_back(startingMidi + 24);
	midi.midiRectValues.push_back(startingMidi + 26);
	midi.midiRectValues.push_back(startingMidi + 28);
	midi.midiRectValues.push_back(startingMidi + 29);
	midi.midiRectValues.push_back(startingMidi + 31);
	midi.midiRectValues.push_back(startingMidi + 33);
	midi.midiRectValues.push_back(startingMidi + 35);//20
	midi.midiRectValues.push_back(startingMidi + 36);
	midi.midiRectValues.push_back(startingMidi + 1);
	midi.midiRectValues.push_back(startingMidi + 3);
	midi.midiRectValues.push_back(startingMidi + 6);
	midi.midiRectValues.push_back(startingMidi + 8);
	midi.midiRectValues.push_back(startingMidi + 10);
	midi.midiRectValues.push_back(startingMidi + 13);
	midi.midiRectValues.push_back(startingMidi + 15);
	midi.midiRectValues.push_back(startingMidi + 18);
	midi.midiRectValues.push_back(startingMidi + 20);//30
	midi.midiRectValues.push_back(startingMidi + 22);
	midi.midiRectValues.push_back(startingMidi + 25);
	midi.midiRectValues.push_back(startingMidi + 27);
	midi.midiRectValues.push_back(startingMidi + 30);
	midi.midiRectValues.push_back(startingMidi + 32);
	midi.midiRectValues.push_back(startingMidi + 34);//37
}