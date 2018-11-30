#include "output_stream.hpp"

using namespace audio;

output_stream::output_stream(char *descriptor, snd_pcm_format_t f, unsigned int sr, unsigned int c)
: device_descriptor(descriptor), format(f), sample_rate(sr), channels(c) {

}

output_stream::~output_stream() {
	snd_pcm_hw_params_free(hw_parameters);
	snd_pcm_sw_params_free(sw_parameters);
}

void output_stream::open_pcm_device() {
	int err = 0;

	// open pcm device
	if((err = snd_pcm_open(&handle, device_descriptor.c_str(), SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC))) {
		throw audio_exception("could not open pcm device", err);
	}


	// allocate hardware parameters
	if((err = snd_pcm_hw_params_malloc(&hw_parameters)) < 0) {
		throw audio_exception("could not allocate hardware parameters", err);
	}

	// initialize hardware parameters
	try {
		output_stream::init_hw_params();
	} catch (const audio_exception& e) {
		snd_pcm_hw_params_free(hw_parameters);
		throw e;
	}

	// allocate software parameters
	if ((err = snd_pcm_sw_params_malloc(&sw_parameters)) < 0) {
		throw audio_exception("could not allocate software parameters", err);
	}

	// initialize software parameters
	try {
		output_stream::init_sw_params();
	} catch (const audio_exception& e) {
		snd_pcm_sw_params_free(sw_parameters);
		throw e;
	}


	
	
}

void output_stream::init_hw_params() {
	int err = 0;	

	// initialize hw parameters
	if((err = snd_pcm_hw_params_any(handle, hw_parameters)) < 0) {
		throw audio_exception("could not initialize hardware parameters", err);
	}

	// dump hw params
	
	

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

void output_stream::init_sw_params() {
	int err = 0;	

	if ((err = snd_pcm_sw_params_current(handle, sw_parameters)) < 0) {
		throw audio_exception("could not initialize software parameters", err);
	}

	if ((err = snd_pcm_sw_params_set_avail_min(handle, sw_parameters, 4096)) < 0) {
		throw audio_exception("could not set minimum avalable count", err);
	}

	if ((err = snd_pcm_sw_params_set_start_threshold(handle, sw_parameters, 0)) < 0) {
		throw audio_exception("could not set start mode", err);
	}

	if ((err = snd_pcm_sw_params(handle, sw_parameters)) < 0) {
		throw audio_exception("could not set software parameters on dvice", err);
	}
}



void output_stream::open(os_callback callback) {
	int err = 0;
	snd_pcm_sframes_t frames = 0;
	snd_pcm_sframes_t frames_in_buffer = 0;
	
	output_stream::open_pcm_device();
	if ((err = snd_pcm_prepare(handle)) < 0) {
		throw audio_exception("could not prepare device", err);
	}

	
	short *buf = callback(0, &frames_in_buffer);
	while(frames_in_buffer > 0) {
		std::cout << "frames in buffer: ";
		std::cout << frames_in_buffer << std::endl;
		if ((err = snd_pcm_wait(handle, 10000)) < 0) {
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

		buf = callback(frames, &frames_in_buffer);
		
	} 

}

void output_stream::close() {
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
}

