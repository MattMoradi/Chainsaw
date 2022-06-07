# ![eclipsenet](https://raw.githubusercontent.com/matthew17754/chainsaw/main/chainsaw/Resources/Images/fullChainsaw.PNG)

# What Is Chainsaw?
Chainsaw is an open source standalone synthesizer based on SFML that can make cool sounds. It has a built-in basic oscillator for generating samples which can be modified using various modifier knobs in the graphical user interface. Does is have as many functionalities as the popular proprietary professional grade synths? No. Does it have VST support? No. Is it called Chainsaw? Yes.

# Dependencies
- SFML: https://www.sfml-dev.org/
- RtMidi: http://www.music.mcgill.ca/~gary/rtmidi/index.html

# Building
## Windows (Visual Studio):
1. [Install SFML](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)
2. [Install RtMidi](http://beatinginfinity.blogspot.com/2013/05/use-rtmidi-in-windows-in-microsoft.html)
3. Build & Run Solution

## Linux (Debian):
1. Install SFML: `sudo apt-get install libsfml-dev`
2. Download [RtMidi](http://www.music.mcgill.ca/~gary/rtmidi/index.html) and extract `RtMidi.h` and `RtMidi.cpp` to the `chainsaw/` folder in this repo
3. Run `make`
4. Run `./Chainsaw`
