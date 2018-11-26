CXX = g++ -v
CXXFLAGS = -std=c++11 -Wall -pthread


INC=-I/usr/local/include/ -I/usr/local/include/pocketsphinx/ -I/usr/local/include/sphinxbase/
LIB=-L/usr/local/lib/

SRC=./VCS/

LIB+=-lportaudio -lpocketsphinx -lsphinxbase -lsndfile -lsamplerate

COMPILE=${CXX} ${CXXFLAGS} ${INC}

all: voice_interface audio_stream
	${COMPILE} -o ./build/vcs ${SRC}main.cpp ./build/*.o ${LIB}
voice_interface: builddir
	${COMPILE} -c ${SRC}voice_interface.cpp -o ./build/voice_interface.o ${LIB}

audio_stream: builddir audio_file
	${COMPILE} -c ${SRC}audio_stream.cpp -o ./build/audio_stream.o
	
audio_file: builddir
	cp computerbeep_75.wav build/ || :
	
builddir:
	mkdir -p build
	cp mllr_matrix build/ || :
clean:
	rm -rdf ./build
