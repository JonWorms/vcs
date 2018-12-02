#include "output_stream.hpp"

using namespace audio;

output_stream::output_stream(char *descriptor, audio::format f, unsigned int sr, unsigned int c)
:stream(descriptor, f, sr, c) {}

output_stream::~output_stream() {}



void output_stream::open(callback cb) {
	stream::open(SND_PCM_STREAM_PLAYBACK);
	snd_pcm_sframes_t frames = 0;
	snd_pcm_sframes_t frames_in_buffer = 0;
	int err = 0;


	
	short *buf = cb(0, &frames_in_buffer);
	while(frames_in_buffer > 0) {
		std::cout << "frames in buffer: ";
		std::cout << frames_in_buffer << std::endl;
		if ((err = snd_pcm_wait(handle, 1000)) < 0) {
			throw audio_exception("device timed out", err);		
		}

		if ((frames = snd_pcm_avail_update(handle)) < 0) {
			if(frames == -EPIPE) {
				throw audio_exception("an overrun has occured");
			} else {
				throw audio_exception("unknown ALSA avail update return value: " + std::to_string(frames));
			}
		}

		if(frames > frames_in_buffer) {
			frames = frames_in_buffer;
		}

		if((frames = snd_pcm_writei(handle, buf, frames)) < 0) {
			throw audio_exception("playback error", err);
		}

		buf = cb(frames, &frames_in_buffer);
		
	} 


}


void output_stream::close() {
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
}

