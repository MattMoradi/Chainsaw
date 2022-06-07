LIBS= -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: Chainsaw

run: clean Chainsaw
	./Chainsaw

clean: 
	rm -f Chainsaw *.o

Chainsaw:
	g++ -o Chainsaw chainsaw/chainsaw.cpp chainsaw/Synth.cpp chainsaw/RtMidi.cpp  $(LIBS)

