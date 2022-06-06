#include "Synth.h"

int main()
{
    while (1)
    {
        Synth::listener();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            Synth::sine(440, false);
    }
}