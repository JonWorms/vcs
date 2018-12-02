#include "input_stream.hpp"

using namespace audio;

input_stream::input_stream(char *des, audio::format f, unsigned int sr, unsigned int c)
: stream(des, f, sr, c) {}


input_stream::~input_stream() {
	
}

void input_stream::open() {
	
}

void input_stream::close() {
	stream::open(SND_PCM_STREAM_CAPTURE);
	
}