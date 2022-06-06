#include "header.h"

int main()
{
    Synth::listener();

    std::cout << "test" << std::endl;
    while (1)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            Synth::sine(440);
    }

        /*
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            Synth::triangle(440);
        */
}