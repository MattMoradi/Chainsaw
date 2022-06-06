#include "Synth.h"
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>

using namespace std;
const int NUM_KNOBS = 3;
const int NUM_TEXTURES = 9;
enum waveEnum { sine, saw, triangle, square };
struct Drawings
{
	sf::Texture texture[NUM_TEXTURES];
	sf::Sprite logo;
	sf::Sprite background;
	sf::Sprite knob[NUM_KNOBS];
	sf::Text text[NUM_KNOBS];
	sf::RectangleShape nameBar;
	sf::Sprite chainsawLogo;
	sf::Sprite wave[4];
	sf::RectangleShape waveBackground[4];
	sf::Text cKey[4];
	sf::RectangleShape cKeyBoarder;
	sf::Text cKeyBoarderText;
	sf::Text midiText;
	sf::CircleShape midiSignal;
	sf::RectangleShape midiBoarder;
	sf::Text midiBoarderText;
	sf::Text octaveValues[6];
	sf::Text volumeValues[11];
	sf::Text rampValues[6];
};

void MapMidiKeysToRect();
void IsMidiConnected(bool connected);
bool ClickedKey(vector<sf::RectangleShape>& rectangles, sf::Event& event);
bool ClickedKnob(sf::Sprite knob[], sf::Event& event, Midi& midi);
bool ClickedWaveType(sf::RectangleShape wave[], sf::Event& event);
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
waveEnum waveType;

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
			//rectangles[i].setFillColor(sf::Color(255, 165, 0));

			//HighlightKey(midi.midiRectValues.at(i), true);
			float freq = 440 * static_cast<float>(pow(2.0, ((static_cast<float>(midi.midiRectValues.at(i) - 69) / 12))));
			HighlightKey(midi.midiRectValues.at(i), true);
		/*
			switch (waveType)
			{
				sf::sleep(sf::milliseconds(10));
				case sine:
				{
					Synth::sine(freq, false);
					break;
				}
				case saw:
				{
					Synth::saw(freq, false);
					break;
				}
				case triangle:
				{
					Synth::triangle(freq, false);
					break;
				}
				case square:
				{
					Synth::square(freq, false);
					break;
				}
				default:
					break;
			}*/
			//HighlightKey(midi.midiRectValues.at(i), true);
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
				cout << "shits fucked fam\n";
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
	for (int i = 0; i < 4; i++)
	{
		if (wave[i].getGlobalBounds().contains(mousePos))
		{
			cout << "Clicked wave " << i << endl;
				waveType = waveEnum(i);
			if (waveType == 0)
				cout << "Changed to sinewave\n";
			else if (waveType == 1)
				cout << "Changed to saw wave\n";
			else if (waveType == 2)
				cout << "changed to triangle wave\n";
			else if (waveType == 3)
				cout << "changed to squre wave\n";
			else
				cout << "ERROR: did not properly set wave type\n";
			wave[i].setFillColor(sf::Color(222, 116, 44));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for (int j = 0; j < 4; j++)
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
	
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.knob[i]);
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.text[i]);
	for (int i = 0; i < 4; i++)
		window.draw(drawings.waveBackground[i]);
	for (int i = 0; i < 4; i++)
		window.draw(drawings.wave[i]);
	for (int i = 0; i < NUM_KEYS; i++)
		window.draw(rectangles[i]);
	for (int i = 0; i < 4; i++)
		window.draw(drawings.cKey[i]);
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

	drawings.texture[2].loadFromFile("Resources/Images/AudioKnob.png", sf::IntRect(0, 0, 59, 65));
	drawings.texture[8].loadFromFile("Resources/Images/AudioKnob1.png", sf::IntRect(0, 0, 44, 49));
	drawings.knob[0].setTexture(drawings.texture[8]);
	drawings.knob[0].setPosition(80, 120);
	drawings.knob[0].setRotation(-125.0f);
	drawings.knob[1].setTexture(drawings.texture[8]);
	drawings.knob[1].setPosition(80, 40);
	drawings.knob[2].setTexture(drawings.texture[2]);
	drawings.knob[2].setPosition(510, 80);
	drawings.knob[2].setRotation(-125.0f);

	drawings.texture[3].loadFromFile("Resources/Images/chainsawText.png", sf::IntRect(0, 0, 265, 37));
	drawings.chainsawLogo.setTexture(drawings.texture[3]);
	drawings.chainsawLogo.setPosition(195, 170);

	drawings.texture[4].loadFromFile("Resources/Images/sineWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[0].setTexture(drawings.texture[4]);
	drawings.wave[0].setPosition(195, 15);

	drawings.texture[5].loadFromFile("Resources/Images/sawWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[1].setTexture(drawings.texture[5]);
	drawings.wave[1].setPosition(245, 2);

	drawings.texture[6].loadFromFile("Resources/Images/triangleWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[2].setTexture(drawings.texture[6]);
	drawings.wave[2].setPosition(295, 12);

	drawings.texture[7].loadFromFile("Resources/Images/squareWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.wave[3].setTexture(drawings.texture[7]);
	drawings.wave[3].setPosition(345, 15);

	
	for (int i = 1; i < 4; i++)
		drawings.wave[i].setColor(sf::Color::Black);

	drawings.waveBackground[0].setPosition(200, 15);
	drawings.waveBackground[0].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[0].setFillColor(sf::Color(222, 116, 44));

	drawings.waveBackground[1].setPosition(250, 15);
	drawings.waveBackground[1].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[1].setFillColor(sf::Color::White);

	drawings.waveBackground[2].setPosition(300, 15);
	drawings.waveBackground[2].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[2].setFillColor(sf::Color::White);

	drawings.waveBackground[3].setPosition(350, 15);
	drawings.waveBackground[3].setSize(sf::Vector2f(45, 40));
	drawings.waveBackground[3].setFillColor(sf::Color::White);

	drawings.cKeyBoarder.setPosition(195, 10);
	drawings.cKeyBoarder.setOutlineColor(sf::Color(222, 116, 44));
	drawings.cKeyBoarder.setOutlineThickness(1);
	drawings.cKeyBoarder.setSize(sf::Vector2f(205.0f, 75.0f));
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
	drawings.text[1].setPosition(65, 70);

	drawings.text[2].setString("RAMP");
	drawings.text[2].setPosition(495, 120);
	
	drawings.midiSignal.setFillColor(sf::Color::Red);
	drawings.midiSignal.setRadius(5);
	drawings.midiSignal.setPosition(430, 13);

	drawings.midiText.setString("MIDI DISCONNECTED");
	drawings.midiText.setPosition(450, 10);
	drawings.midiText.setFont(font);
	drawings.midiText.setFillColor(sf::Color::White);
	drawings.midiText.setCharacterSize(12);

	//590 320
	/*
	drawings.midiBoarder.setPosition(445, 5);
	drawings.midiBoarder.setOutlineColor(sf::Color(222, 116, 44));
	drawings.midiBoarder.setOutlineThickness(1);
	drawings.midiBoarder.setSize(sf::Vector2f(205.0f, 45.0f));
	drawings.midiBoarder.setFillColor(sf::Color::Black);

	drawings.midiBoarderText.setString("MIDI");
	drawings.midiBoarderText.setPosition(260, 65);
	drawings.midiBoarderText.setFont(font);
	drawings.midiBoarderText.setFillColor(sf::Color::White);
	drawings.midiBoarderText.setCharacterSize(12);
	*/

	//set the origin of each knob to the center so rotation can properly occur
	for (int i = 0; i < NUM_KNOBS; i++)
		drawings.knob[i].setOrigin(sf::Vector2f(drawings.knob[i].getLocalBounds().width / 2, drawings.knob[i].getLocalBounds().height / 2));
}