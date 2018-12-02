#include "input_stream.hpp"

using namespace audio;

input_stream::input_stream(char *des, audio::format f, unsigned int sr, unsigned int c)
: stream(des, f, sr, c) {}


input_stream::~input_stream() {}

void input_stream::open(callback cb) {
	stream::open(SND_PCM_STREAM_CAPTURE);

	int err;
	short *buf;
	unsigned long should_read = 0;
	unsigned long actually_read = 0;
	
	while((buf = cb(actually_read, &should_read)) != NULL) {
		if((err = snd_pcm_readi(handle, buf, should_read)) < 0) {
			throw audio_exception("input stream read error", err);
		}	
		actually_read = err;
	}
}

void input_stream::close() {
	stream::close();
}

