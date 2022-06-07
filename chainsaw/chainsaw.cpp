#include "Synth.h"
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>

using namespace std;
const int NUM_KNOBS = 3;
const int NUM_TEXTURES = 10;

struct Drawings
{
	sf::Texture texture[NUM_TEXTURES];
	sf::Sprite logo;
	sf::Sprite background;
	sf::Sprite knob[NUM_KNOBS];
	sf::Text text[NUM_KNOBS];
	sf::RectangleShape nameBar;
	sf::Sprite chainsawLogo;
	sf::Sprite wave[5];
	sf::RectangleShape waveBackground[5];
	sf::Text cKey[4];
	sf::RectangleShape cKeyBoarder;
	sf::Text cKeyBoarderText;
	sf::Text midiText;
	sf::CircleShape midiSignal;
	sf::RectangleShape midiBoarder;
	sf::Text midiBoarderText;
	sf::CircleShape octaveTickValues[6];
	sf::CircleShape gainTickValues[11];
	sf::CircleShape rampTickValues[6];
	sf::Text envelopeText;
	sf::CircleShape envelopeCircle[2];
	sf::RectangleShape envelopeSquare;
	sf::CircleShape envelopeStatusCircle;
	sf::Text envelopeStatusText;
};

void MapMidiKeysToRect();
void IsMidiConnected(bool connected);
bool ClickedKey(vector<sf::RectangleShape>& rectangles, sf::Event& event);
bool ClickedKnob(sf::Sprite knob[], sf::Event& event, Midi& midi);
bool ClickedWaveType(sf::RectangleShape wave[], sf::Event& event);
bool ClickedEnvelopeSwitch(sf::Event& event);
void GenerateKeyboardKeys(vector<sf::RectangleShape>& rectangles, sf::RenderWindow& window);
void UpdateValuesFromKnob(int index, int value, sf::Sprite knob[], Midi& midi);
void LoadTextures(Drawings& drawings, sf::Font& font);
void DrawSprites(sf::RenderWindow& window, Drawings& drawings);
bool UpdateOctave(int amount);
bool UpdateVolume(int amount);
bool UpdateRamp(int value);

Midi midi;
Drawings drawings;
vector<sf::RectangleShape> rectangles;

int main()
{
	sf::Font font;
	sf::RenderWindow window(sf::VideoMode(590, 320), "Chainsaw Demo v1.0");
	font.loadFromFile("Resources/Fonts/Arial.ttf");
	GenerateKeyboardKeys(rectangles, window);
	MapMidiKeysToRect();
	LoadTextures(drawings, font);
	IsMidiConnected(true);

	while (window.isOpen())
	{
		sf::Event event;
		window.setSize(sf::Vector2u(590, 320));

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//Synth::listener();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (!ClickedKnob(drawings.knob, event, midi))
					cout << "Mouse not on knob\n";
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!ClickedKey(rectangles, event))
						cout << "Mouse out of rect\n";
					if (!ClickedWaveType(drawings.waveBackground, event))
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
					rectangles[i].setFillColor(sf::Color::White);
				for (int i = 22; i < NUM_KEYS; i++)
					rectangles[i].setFillColor(sf::Color::Black);
			}
		}

		window.clear(sf::Color::Black);
		DrawSprites(window, drawings);
		window.display();
	}
}

//Check if user click on a key
bool ClickedKey(vector<sf::RectangleShape>& rectangles, sf::Event& event)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);
	for (int i = NUM_KEYS - 1; i >= 0; i--)
	{
		if (rectangles[i].getGlobalBounds().contains(mousePos))
		{
			cout << "Mouse is inside rect " << i << endl;
			cout << "Playing midi note: " << midi.midiRectValues.at(i) << endl;

			float freq = 440 * static_cast<float>(pow(2.0, ((static_cast<float>(midi.midiRectValues.at(i) - 69) / 12))));

			switch (midi.waveType)
			{
				sf::sleep(sf::milliseconds(10));
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

bool ClickedEnvelopeSwitch(sf::Event& event)
{
	int mx = event.mouseButton.x;
	int my = event.mouseButton.y;
	sf::Vector2f mousePos(mx, my);
	
		if (drawings.envelopeSquare.getGlobalBounds().contains(mousePos) || drawings.envelopeCircle[0].getGlobalBounds().contains(mousePos) || 
			drawings.envelopeCircle[1].getGlobalBounds().contains(mousePos))
		{
			cout << "Clicked envelope switch " << endl;
			if (midi.isUsingEnvelope)
			{
				midi.isUsingEnvelope = false;
				drawings.envelopeStatusText.setString("ON");
				drawings.envelopeStatusText.setPosition(500, 133);
				drawings.envelopeStatusCircle.setPosition(526, 128);
				drawings.envelopeSquare.setFillColor(sf::Color(222, 116, 44));
				drawings.envelopeCircle[0].setFillColor(sf::Color(222, 116, 44));
				drawings.envelopeCircle[1].setFillColor(sf::Color(222, 116, 44));
			}
			else
			{
				midi.isUsingEnvelope = true;
				drawings.envelopeStatusText.setString("OFF");
				drawings.envelopeStatusText.setPosition(515, 133);
				drawings.envelopeStatusCircle.setPosition(490, 128);
				drawings.envelopeSquare.setFillColor(sf::Color::Black);
				drawings.envelopeCircle[0].setFillColor(sf::Color::Black);
				drawings.envelopeCircle[1].setFillColor(sf::Color::Black);
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

//Generates the size and location of each key that will be on screen
void GenerateKeyboardKeys(vector<sf::RectangleShape>& rectangles, sf::RenderWindow& window)
{
	sf::Vector2u windowSize = window.getSize();
	float wX = windowSize.x;
	float wY = windowSize.y;
	float x = 1;

	sf::RectangleShape whiteKey(sf::Vector2f(25.0f, 120.0f));
	sf::RectangleShape BlackKey(sf::Vector2f(13.0f, 80));
	vector<sf::RectangleShape> white;
	vector<sf::RectangleShape> black;
	BlackKey.setFillColor(sf::Color::Black);

	//set location of white keys
	for (int i = 0; i < 29; i++)
	{
		white.push_back(whiteKey);
		white[i].setPosition(x, wY - 120);
		x += 27;
	}

	x = 20;

	//set location of black keys
	for (int i = 0; i < 15; i++)
	{
		black.push_back(BlackKey);
		black[i].setPosition(x, wY - 120);
		if (i == 1 || i == 4 || i == 6 || i == 9 || i == 11 || i == 14)
			x += 27;
		x += 27;
	}

	for (int i = 0; i < 22; i++)
		rectangles.push_back(white[i]);
	for (int i = 0; i < 15; i++)
		rectangles.push_back(black[i]);
}

void IsMidiConnected(bool connected)
{
	if (!connected)
	{
		drawings.midiSignal.setFillColor(sf::Color::Red);
		drawings.midiText.setString("NO MIDI");
	}
	else
	{
		drawings.midiSignal.setFillColor(sf::Color::Green);
		drawings.midiText.setString("MIDI CONNECTED");
	}
}

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
				rectangles[i].setFillColor(sf::Color(222, 116, 44));
			else
			{
				if (i < 21)
					rectangles[i].setFillColor(sf::Color::White);
				else
					rectangles[i].setFillColor(sf::Color::Black);
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
		drawings.cKey[i].setString("C" + std::to_string(midi.octave + i));

	//regenerate midi key vector
	MapMidiKeysToRect();

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

bool UpdateRamp(int amount)
{
	float val;
	if ((amount < 0 && midi.ramp < 0.1f) || (amount > 0 && midi.ramp > 0.45))
	{
		cout << "did not update ramp. ramp is at " << midi.ramp << endl;
		return false;
	}

	if (amount > 0)
	{

		val = .1f;
		cout << "Rotating right\n";
	}
	else
	{
		val = -.1f;
		cout << "Rotating left\n";
	}
	if (midi.ramp < 0.0)
		midi.ramp = 0.0f;
	midi.ramp += val;
	cout << "ramp is now: " << midi.ramp << "\n";


	return true;
}


//fill the midiRectValue vector with its midi key value
void MapMidiKeysToRect()
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

//draws all the drawings onto the window
void DrawSprites(sf::RenderWindow& window, Drawings& drawings)
{
	window.draw(drawings.background);
	window.draw(drawings.logo);
	window.draw(drawings.nameBar);
	window.draw(drawings.chainsawLogo);
	window.draw(drawings.midiBoarder);
	window.draw(drawings.midiSignal);
	window.draw(drawings.midiText);
	window.draw(drawings.midiBoarderText);
	window.draw(drawings.cKeyBoarder);
	window.draw(drawings.cKeyBoarderText);
	window.draw(drawings.envelopeText);
	for (int i = 0; i < 6; i++)
		window.draw(drawings.rampTickValues[i]);
	for (int i = 0; i < 11; i++)
		window.draw(drawings.gainTickValues[i]);
	for (int i = 0; i < 6; i++)
		window.draw(drawings.octaveTickValues[i]);
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.knob[i]);
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.text[i]);
	for (int i = 0; i < 5; i++)
		window.draw(drawings.waveBackground[i]);
	for (int i = 0; i < 5; i++)
		window.draw(drawings.wave[i]);
	for (int i = 0; i < NUM_KEYS; i++)
		window.draw(rectangles[i]);
	for (int i = 0; i < 4; i++)
		window.draw(drawings.cKey[i]);
	for (int i = 0; i < 2; i++)
		window.draw(drawings.envelopeCircle[i]);
	window.draw(drawings.envelopeSquare);
	window.draw(drawings.envelopeStatusCircle);
	window.draw(drawings.envelopeStatusText);
}

void LoadTextures(Drawings& drawings, sf::Font& font)
{
	drawings.nameBar.setPosition(0, 170);
	drawings.nameBar.setSize(sf::Vector2f(590.0f, 30.0f));
	drawings.nameBar.setFillColor(sf::Color(100, 100, 100));
	drawings.texture[0].loadFromFile("Resources/Images/background.jpg", sf::IntRect(0, 0, 645, 450));
	drawings.background.setTexture(drawings.texture[0]);

	drawings.texture[1].loadFromFile("Resources/Images/chainsawlogo.png", sf::IntRect(0, 0, 267, 65));
	drawings.logo.setTexture(drawings.texture[1]);
	drawings.logo.setPosition(160, 100);

	drawings.texture[2].loadFromFile("Resources/Images/AudioKnobLarge.png", sf::IntRect(0, 0, 65, 65));
	drawings.texture[8].loadFromFile("Resources/Images/AudioKnobSmall.png", sf::IntRect(0, 0, 50, 50));
	drawings.knob[0].setTexture(drawings.texture[8]);
	drawings.knob[0].setPosition(80, 125);
	drawings.knob[0].setRotation(-125.0f);
	drawings.knob[1].setTexture(drawings.texture[8]);
	drawings.knob[1].setPosition(80, 40);
	drawings.knob[2].setTexture(drawings.texture[2]);
	drawings.knob[2].setPosition(510, 70);
	drawings.knob[2].setRotation(-125.0f);

	drawings.texture[3].loadFromFile("Resources/Images/chainsawText.png", sf::IntRect(0, 0, 212, 26));
	drawings.chainsawLogo.setTexture(drawings.texture[3]);
	drawings.chainsawLogo.setPosition(190, 172);

	drawings.texture[4].loadFromFile("Resources/Images/sineWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[0].setTexture(drawings.texture[4]);
	drawings.wave[0].setPosition(170, 15);

	drawings.texture[5].loadFromFile("Resources/Images/sawWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[1].setTexture(drawings.texture[5]);
	drawings.wave[1].setPosition(220, 2);

	drawings.texture[6].loadFromFile("Resources/Images/triangleWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[2].setTexture(drawings.texture[6]);
	drawings.wave[2].setPosition(270, 12);


	drawings.texture[7].loadFromFile("Resources/Images/squareWave.png", sf::IntRect(0, 0, 42, 36));
	drawings.wave[3].setTexture(drawings.texture[7]);
	drawings.wave[3].setPosition(328, 17);

	drawings.texture[9].loadFromFile("Resources/Images/whtieNoiseWave.png", sf::IntRect(0, 0, 43, 40));
	drawings.wave[4].setTexture(drawings.texture[9]);
	drawings.wave[4].setPosition(375, 15);


	for (int i = 1; i < 5; i++)
		drawings.wave[i].setColor(sf::Color::Black);

	drawings.waveBackground[0].setPosition(175, 15);
	drawings.waveBackground[0].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[0].setFillColor(sf::Color(222, 116, 44));

	drawings.waveBackground[1].setPosition(225, 15);
	drawings.waveBackground[1].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[1].setFillColor(sf::Color::White);

	drawings.waveBackground[2].setPosition(275, 15);
	drawings.waveBackground[2].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[2].setFillColor(sf::Color::White);

	drawings.waveBackground[3].setPosition(325, 15);
	drawings.waveBackground[3].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[3].setFillColor(sf::Color::White);

	drawings.waveBackground[4].setPosition(375, 15);
	drawings.waveBackground[4].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[4].setFillColor(sf::Color::White);

	//Octave tick values
	for (int i = 0; i < 6; i++)
	{
		drawings.octaveTickValues[i].setFillColor(sf::Color::White);
		drawings.octaveTickValues[i].setRadius(2.0f);
	}

	drawings.octaveTickValues[0].setPosition(52, 140);
	drawings.octaveTickValues[1].setPosition(48, 117);
	drawings.octaveTickValues[2].setPosition(65, 96);
	drawings.octaveTickValues[3].setPosition(91, 96);
	drawings.octaveTickValues[4].setPosition(106, 118);
	drawings.octaveTickValues[5].setPosition(101, 139);

	//gain tick values
	for (int i = 0; i < 11; i++)
	{
		drawings.gainTickValues[i].setFillColor(sf::Color::White);
		drawings.gainTickValues[i].setRadius(2.0f);
	}

	drawings.gainTickValues[0].setPosition(53, 55);
	drawings.gainTickValues[1].setPosition(48, 42);
	drawings.gainTickValues[2].setPosition(49, 30);
	drawings.gainTickValues[3].setPosition(53, 20);
	drawings.gainTickValues[4].setPosition(63, 12);
	drawings.gainTickValues[5].setPosition(76, 9);
	drawings.gainTickValues[6].setPosition(90, 12);
	drawings.gainTickValues[7].setPosition(101, 20);
	drawings.gainTickValues[8].setPosition(107, 30);
	drawings.gainTickValues[9].setPosition(106, 42);
	drawings.gainTickValues[10].setPosition(102, 56);

	//ramp tick value
	for (int i = 0; i < 6; i++)
	{
		drawings.rampTickValues[i].setFillColor(sf::Color::White);
		drawings.rampTickValues[i].setRadius(2.0f);
	}

	drawings.rampTickValues[0].setPosition(476, 87);
	drawings.rampTickValues[1].setPosition(472, 58);
	drawings.rampTickValues[2].setPosition(489, 35);
	drawings.rampTickValues[3].setPosition(526, 35);
	drawings.rampTickValues[4].setPosition(544, 58);
	drawings.rampTickValues[5].setPosition(538, 87);

	drawings.envelopeText.setFont(font);
	drawings.envelopeText.setFillColor(sf::Color::White);
	drawings.envelopeText.setCharacterSize(12);
	drawings.envelopeText.setString("ENVELOPE");
	drawings.envelopeText.setPosition(480, 150);
	
	drawings.envelopeStatusText.setFont(font);
	drawings.envelopeStatusText.setFillColor(sf::Color::White);
	drawings.envelopeStatusText.setCharacterSize(9);
	drawings.envelopeStatusText.setString("OFF");
	drawings.envelopeStatusText.setPosition(515, 133);

	drawings.envelopeStatusCircle.setRadius(10.0f);
	drawings.envelopeStatusCircle.setFillColor(sf::Color::White);
	drawings.envelopeStatusCircle.setPosition(490, 128);

	for (int i = 0; i < 2; i++)
	{
		drawings.envelopeCircle[i].setFillColor(sf::Color::Black);
		drawings.envelopeCircle[i].setRadius(12.5f);
	}

	drawings.envelopeCircle[0].setPosition(485, 125);
	drawings.envelopeCircle[1].setPosition(525, 125);

	drawings.envelopeSquare.setPosition(495, 125);
	drawings.envelopeSquare.setSize(sf::Vector2f(40, 25));
	drawings.envelopeSquare.setFillColor(sf::Color::Black);

	drawings.cKeyBoarder.setPosition(170, 10);
	drawings.cKeyBoarder.setOutlineColor(sf::Color(222, 116, 44));
	drawings.cKeyBoarder.setOutlineThickness(1);
	drawings.cKeyBoarder.setSize(sf::Vector2f(255.0f, 75.0f));
	drawings.cKeyBoarder.setFillColor(sf::Color::Black);

	drawings.cKeyBoarderText.setString("WAVEFORM");
	drawings.cKeyBoarderText.setPosition(260, 65);
	drawings.cKeyBoarderText.setFont(font);
	drawings.cKeyBoarderText.setFillColor(sf::Color::White);
	drawings.cKeyBoarderText.setCharacterSize(12);

	for (int i = 0; i < 4; i++)
	{
		drawings.cKey[i].setFont(font);
		drawings.cKey[i].setFillColor(sf::Color::Black);
		drawings.cKey[i].setCharacterSize(12);
	}

	drawings.cKey[0].setString("C1");
	drawings.cKey[0].setPosition(1, 305);

	drawings.cKey[1].setString("C2");
	drawings.cKey[1].setPosition(195, 305);

	drawings.cKey[2].setString("C3");
	drawings.cKey[2].setPosition(382, 305);

	drawings.cKey[3].setString("C4");
	drawings.cKey[3].setPosition(572, 305);

	for (int i = 0; i < NUM_KNOBS; i++)
	{
		drawings.text[i].setFont(font);
		drawings.text[i].setFillColor(sf::Color::White);
		drawings.text[i].setCharacterSize(12);
	}

	drawings.text[0].setString("OCTAVE");
	drawings.text[0].setPosition(55, 150);

	drawings.text[1].setString("GAIN");
	drawings.text[1].setPosition(65, 66);

	drawings.text[2].setString("RAMP");
	drawings.text[2].setPosition(494, 100);

	drawings.midiSignal.setFillColor(sf::Color::Red);
	drawings.midiSignal.setRadius(5);
	drawings.midiSignal.setPosition(440, 13);

	drawings.midiText.setString("MIDI DISCONNECTED");
	drawings.midiText.setPosition(460, 10);
	drawings.midiText.setFont(font);
	drawings.midiText.setFillColor(sf::Color::White);
	drawings.midiText.setCharacterSize(12);

	//set the origin of each knob to the center so rotation can properly occur
	for (int i = 0; i < NUM_KNOBS; i++)
		drawings.knob[i].setOrigin(sf::Vector2f(drawings.knob[i].getLocalBounds().width / 2, drawings.knob[i].getLocalBounds().height / 2));
}