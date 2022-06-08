#include "Gui.h"

//Load images and text for synthesizer gui
void Gui::LoadAssetsFromFile()
{
	drawings.texture[0].loadFromFile("Resources/Images/background.jpg", sf::IntRect(0, 0, 645, 450));
	drawings.texture[1].loadFromFile("Resources/Images/chainsawlogo.png", sf::IntRect(0, 0, 267, 65));
	drawings.texture[2].loadFromFile("Resources/Images/AudioKnobLarge.png", sf::IntRect(0, 0, 65, 65));
	drawings.texture[3].loadFromFile("Resources/Images/chainsawText.png", sf::IntRect(0, 0, 212, 26));
	drawings.texture[4].loadFromFile("Resources/Images/sineWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.texture[5].loadFromFile("Resources/Images/sawWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.texture[6].loadFromFile("Resources/Images/triangleWave.png", sf::IntRect(0, 0, 60, 60));
	drawings.texture[7].loadFromFile("Resources/Images/squareWave.png", sf::IntRect(0, 0, 42, 36));
	drawings.texture[8].loadFromFile("Resources/Images/AudioKnobSmall.png", sf::IntRect(0, 0, 50, 50));
	drawings.texture[9].loadFromFile("Resources/Images/whtieNoiseWave.png", sf::IntRect(0, 0, 43, 40));
	drawings.font.loadFromFile("Resources/Fonts/Arial.ttf");	
}

//Set the inital font, position, size, texture, and/or, color of each gui element
void Gui::InitalizeGui()
{
	//chainsaw logo
	drawings.chainsawLogo.setTexture(drawings.texture[1]);
	drawings.chainsawLogo.setPosition(160, 100);

	//main background
	drawings.backgroundImage.setTexture(drawings.texture[0]);

	//chainsaw text background
	drawings.chainsawTextBackground.setPosition(0, 170);
	drawings.chainsawTextBackground.setSize(sf::Vector2f(590.0f, 30.0f));
	drawings.chainsawTextBackground.setFillColor(sf::Color(100, 100, 100));

	//wave image background
	drawings.waveImageBackground.setPosition(170, 10);
	drawings.waveImageBackground.setOutlineColor(sf::Color(222, 116, 44));
	drawings.waveImageBackground.setOutlineThickness(1);
	drawings.waveImageBackground.setSize(sf::Vector2f(255.0f, 75.0f));
	drawings.waveImageBackground.setFillColor(sf::Color::Black);

	//wave image backgrounds
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

	//audio knobs
	drawings.knob[0].setTexture(drawings.texture[8]);
	drawings.knob[0].setPosition(80, 125);
	drawings.knob[0].setRotation(-125.0f);
	drawings.knob[1].setTexture(drawings.texture[8]);
	drawings.knob[1].setPosition(80, 40);
	drawings.knob[2].setTexture(drawings.texture[2]);
	drawings.knob[2].setPosition(510, 70);
	drawings.knob[2].setRotation(-125.0f);

	//chainsaw logo text
	drawings.chainsawText.setTexture(drawings.texture[3]);
	drawings.chainsawText.setPosition(190, 172);

	//Wave images
	drawings.waveImage[0].setTexture(drawings.texture[4]);
	drawings.waveImage[0].setPosition(170, 15);
	drawings.waveImage[1].setTexture(drawings.texture[5]);
	drawings.waveImage[1].setPosition(220, 2);
	drawings.waveImage[2].setTexture(drawings.texture[6]);
	drawings.waveImage[2].setPosition(270, 12);
	drawings.waveImage[3].setTexture(drawings.texture[7]);
	drawings.waveImage[3].setPosition(328, 17);
	drawings.waveImage[4].setTexture(drawings.texture[9]);
	drawings.waveImage[4].setPosition(375, 15);

	for (int i = 1; i < 5; i++)
		drawings.waveImage[i].setColor(sf::Color::Black);

	
	//Octave tick values
	for (int i = 0; i < 6; i++)
	{
		drawings.octaveTickValue[i].setFillColor(sf::Color::White);
		drawings.octaveTickValue[i].setRadius(2.0f);
	}

	drawings.octaveTickValue[0].setPosition(52, 140);
	drawings.octaveTickValue[1].setPosition(48, 117);
	drawings.octaveTickValue[2].setPosition(65, 96);
	drawings.octaveTickValue[3].setPosition(91, 96);
	drawings.octaveTickValue[4].setPosition(106, 118);
	drawings.octaveTickValue[5].setPosition(101, 139);

	//gain tick values
	for (int i = 0; i < 11; i++)
	{
		drawings.gainTickValue[i].setFillColor(sf::Color::White);
		drawings.gainTickValue[i].setRadius(2.0f);
	}

	drawings.gainTickValue[0].setPosition(53, 55);
	drawings.gainTickValue[1].setPosition(48, 42);
	drawings.gainTickValue[2].setPosition(49, 30);
	drawings.gainTickValue[3].setPosition(53, 20);
	drawings.gainTickValue[4].setPosition(63, 12);
	drawings.gainTickValue[5].setPosition(76, 9);
	drawings.gainTickValue[6].setPosition(90, 12);
	drawings.gainTickValue[7].setPosition(101, 20);
	drawings.gainTickValue[8].setPosition(107, 30);
	drawings.gainTickValue[9].setPosition(106, 42);
	drawings.gainTickValue[10].setPosition(102, 56);

	//ramp tick value
	for (int i = 0; i < 6; i++)
	{
		drawings.rampTickValue[i].setFillColor(sf::Color::White);
		drawings.rampTickValue[i].setRadius(2.0f);
	}

	drawings.rampTickValue[0].setPosition(476, 87);
	drawings.rampTickValue[1].setPosition(472, 58);
	drawings.rampTickValue[2].setPosition(489, 35);
	drawings.rampTickValue[3].setPosition(526, 35);
	drawings.rampTickValue[4].setPosition(544, 58);
	drawings.rampTickValue[5].setPosition(538, 87);

	//envelope switch text
	drawings.envelopeText.setFont(drawings.font);
	drawings.envelopeText.setFillColor(sf::Color::White);
	drawings.envelopeText.setCharacterSize(12);
	drawings.envelopeText.setString("ENVELOPE");
	drawings.envelopeText.setPosition(480, 150);

	//envelope switch status text
	drawings.envelopeStatusText.setFont(drawings.font);
	drawings.envelopeStatusText.setFillColor(sf::Color::White);
	drawings.envelopeStatusText.setCharacterSize(9);
	drawings.envelopeStatusText.setString("OFF");
	drawings.envelopeStatusText.setPosition(515, 133);

	//envelope switch inner circle
	drawings.envelopeStatusCircle.setRadius(10.0f);
	drawings.envelopeStatusCircle.setFillColor(sf::Color::White);
	drawings.envelopeStatusCircle.setPosition(490, 128);

	//envelope switch button circle
	for (int i = 0; i < 2; i++)
	{
		drawings.envelopeSwitchCircle[i].setFillColor(sf::Color::Black);
		drawings.envelopeSwitchCircle[i].setRadius(12.5f);
	}

	drawings.envelopeSwitchCircle[0].setPosition(485, 125);
	drawings.envelopeSwitchCircle[1].setPosition(525, 125);

	//envelope switch button square
	drawings.envelopeSwitchSquare.setPosition(495, 125);
	drawings.envelopeSwitchSquare.setSize(sf::Vector2f(40, 25));
	drawings.envelopeSwitchSquare.setFillColor(sf::Color::Black);

	//waveform text
	drawings.waveText.setString("WAVEFORM");
	drawings.waveText.setPosition(260, 65);
	drawings.waveText.setFont(drawings.font);
	drawings.waveText.setFillColor(sf::Color::White);
	drawings.waveText.setCharacterSize(12);

	//C key text
	for (int i = 0; i < 4; i++)
	{
		drawings.cKeyText[i].setFont(drawings.font);
		drawings.cKeyText[i].setFillColor(sf::Color::Black);
		drawings.cKeyText[i].setCharacterSize(12);
	}

	drawings.cKeyText[0].setString("C1");
	drawings.cKeyText[0].setPosition(1, 305);
	drawings.cKeyText[1].setString("C2");
	drawings.cKeyText[1].setPosition(195, 305);
	drawings.cKeyText[2].setString("C3");
	drawings.cKeyText[2].setPosition(382, 305);
	drawings.cKeyText[3].setString("C4");
	drawings.cKeyText[3].setPosition(572, 305);

	//Synthesizer knob text
	for (int i = 0; i < NUM_KNOBS; i++)
	{
		drawings.knobText[i].setFont(drawings.font);
		drawings.knobText[i].setFillColor(sf::Color::White);
		drawings.knobText[i].setCharacterSize(12);
	}

	drawings.knobText[0].setString("OCTAVE");
	drawings.knobText[0].setPosition(55, 150);
	drawings.knobText[1].setString("GAIN");
	drawings.knobText[1].setPosition(65, 66);
	drawings.knobText[2].setString("RAMP");
	drawings.knobText[2].setPosition(494, 100);

	//midi status s
	drawings.midiStatusSignal.setFillColor(sf::Color::Red);
	drawings.midiStatusSignal.setRadius(5);
	drawings.midiStatusSignal.setPosition(440, 13);

	//midi signal text
	drawings.midiSignalText.setString("NO MIDI");
	drawings.midiSignalText.setPosition(460, 10);
	drawings.midiSignalText.setFont(drawings.font);
	drawings.midiSignalText.setFillColor(sf::Color::White);
	drawings.midiSignalText.setCharacterSize(12);

	//set the origin of each knob to the center so rotation can properly occur
	for (int i = 0; i < NUM_KNOBS; i++)
		drawings.knob[i].setOrigin(sf::Vector2f(drawings.knob[i].getLocalBounds().width / 2, drawings.knob[i].getLocalBounds().height / 2));
}


//Draw the gui to the window
void Gui::Draw(sf::RenderWindow& window)
{
	window.draw(drawings.backgroundImage);
	window.draw(drawings.chainsawLogo);
	window.draw(drawings.chainsawTextBackground);
	window.draw(drawings.chainsawText);
	window.draw(drawings.midiStatusSignal);
	window.draw(drawings.midiSignalText);
	window.draw(drawings.waveImageBackground);
	window.draw(drawings.waveText);
	window.draw(drawings.envelopeText);

	for (int i = 0; i < 6; i++)
		window.draw(drawings.rampTickValue[i]);
	for (int i = 0; i < 11; i++)
		window.draw(drawings.gainTickValue[i]);
	for (int i = 0; i < 6; i++)
		window.draw(drawings.octaveTickValue[i]);
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.knob[i]);
	for (int i = 0; i < NUM_KNOBS; i++)
		window.draw(drawings.knobText[i]);
	for (int i = 0; i < 5; i++)
		window.draw(drawings.waveBackground[i]);
	for (int i = 0; i < 5; i++)
		window.draw(drawings.waveImage[i]);
	for (int i = 0; i < NUM_KEYS; i++)
		window.draw(drawings.pianoKey[i]);
	for (int i = 0; i < 4; i++)
		window.draw(drawings.cKeyText[i]);
	for (int i = 0; i < 2; i++)
		window.draw(drawings.envelopeSwitchCircle[i]);

	window.draw(drawings.envelopeSwitchSquare);
	window.draw(drawings.envelopeStatusCircle);
	window.draw(drawings.envelopeStatusText);
}

//Generate the location of each key
void Gui::GenerateKeyboard(sf::RenderWindow& window)
{
	sf::Vector2u windowSize = window.getSize();
	float wX = windowSize.x;
	float wY = windowSize.y;
	float x = 1;

	sf::RectangleShape whiteKey(sf::Vector2f(25.0f, 120.0f));
	sf::RectangleShape BlackKey(sf::Vector2f(13.0f, 80));
	std::vector<sf::RectangleShape> white;
	std::vector<sf::RectangleShape> black;
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
		drawings.pianoKey.push_back(white[i]);
	for (int i = 0; i < 15; i++)
		drawings.pianoKey.push_back(black[i]);
}
