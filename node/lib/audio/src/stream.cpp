#include "stream.hpp"

using namespace audio;


stream::stream(char *des, audio::format f, unsigned int sr, unsigned int c)
: device_descriptor(des), format(format_alsa(f)), sample_rate(sr), channels(c) {}

stream::~stream() {
	snd_pcm_hw_params_free(hw_parameters);
}



void stream::open_pcm_device(snd_pcm_stream_t type) {
	int err = 0;

	// open pcm device
	if((err = snd_pcm_open(&handle, device_descriptor.c_str(), type, SND_PCM_ASYNC))) {
		throw audio_exception("could not open pcm device", err);
	}

	// allocate hardware parameters
	if((err = snd_pcm_hw_params_malloc(&hw_parameters)) < 0) {
		throw audio_exception("could not allocate hardware parameters", err);
	}

	// initialize hardware parameters
	try {
		stream::init_hw_params();
	} catch (const audio_exception& e) {
		snd_pcm_hw_params_free(hw_parameters);
		throw e;
	}

	// allocate software parameters
	if ((err = snd_pcm_sw_params_malloc(&sw_parameters)) < 0) {
		throw audio_exception("could not allocate software parameters", err);
	}

}



void stream::init_hw_params() {
	int err = 0;	

	// initialize hw parameters
	if((err = snd_pcm_hw_params_any(handle, hw_parameters)) < 0) {
		throw audio_exception("could not initialize hardware parameters", err);
	}

	// restrict configuration space to only one access type
	if((err = snd_pcm_hw_params_set_access(handle, hw_parameters, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		throw audio_exception("could not configure access type", err);
	}

	// set format
	if((err = snd_pcm_hw_params_set_format(handle, hw_parameters, format)) < 0) {
		throw audio_exception("could not set format", err);
	}

	// set sample rate
	if((err = snd_pcm_hw_params_set_rate_near(handle, hw_parameters, &sample_rate, 0)) < 0) {
		throw audio_exception("could not set sample rate", err);
	}

	// set number of channels
	if ((err = snd_pcm_hw_params_set_channels (handle, hw_parameters, channels)) < 0) {
		throw audio_exception("could not set channel count", err);
	}

	// finally configure device with the fully initialized hardware parameters
	if ((err = snd_pcm_hw_params(handle, hw_parameters)) < 0) {
		throw audio_exception("could not set hardware parameters on device", err);
	}
	
}



void stream::open(snd_pcm_stream_t type) {
	int err = 0;
	stream::open_pcm_device(type);
	if ((err = snd_pcm_prepare(handle)) < 0) {
		throw audio_exception("could not prepare device", err);
	}
}