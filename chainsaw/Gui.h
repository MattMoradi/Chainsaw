#include<SFML/Graphics.hpp>
const int NUM_KNOBS = 3;
const int NUM_TEXTURES = 10;
const int NUM_KEYS = 37;

class Gui
{
	public:
		void LoadAssetsFromFile();
		void InitalizeGui();
		void Draw(sf::RenderWindow& window);
		void GenerateKeyboard(sf::RenderWindow& window);

		struct Drawings
		{
			sf::Font font;
			sf::Texture texture[NUM_TEXTURES];
			sf::Sprite chainsawLogo;
			sf::Sprite backgroundImage;
			sf::Sprite knob[NUM_KNOBS];
			sf::Text knobText[NUM_KNOBS];
			sf::RectangleShape chainsawTextBackground;
			sf::Sprite chainsawText;
			sf::Sprite waveImage[5];
			sf::RectangleShape waveBackground[5];
			sf::Text cKeyText[4];
			sf::RectangleShape waveImageBackground;
			sf::Text waveText;
			sf::Text midiSignalText;
			sf::CircleShape midiStatusSignal;
			sf::CircleShape octaveTickValue[6];
			sf::CircleShape gainTickValue[11];
			sf::CircleShape rampTickValue[6];
			sf::Text envelopeText;
			sf::CircleShape envelopeSwitchCircle[2];
			sf::RectangleShape envelopeSwitchSquare;
			sf::CircleShape envelopeStatusCircle;
			sf::Text envelopeStatusText;
			std::vector<sf::RectangleShape> pianoKey;
		}drawings;
};

