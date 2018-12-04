


//#include <stdlib.h>
//#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "audio.hpp"
#include "stt.hpp"

stt::engine stt_engine;


int main(int argc, char *argv[]) {
	audio::device *playback_device = audio::get_device_named("USB Audio DAC");
	audio::device *capture_device = audio::get_device_named("USB audio CODEC");

	
	stt::language_model lm = stt::language_model::languages().front();
	stt_engine.set_model(lm);

	
	if(!playback_device) {
		std::cerr << "could not get playback device" << std::endl;
		exit(1);
	}

	if(!capture_device) {
		std::cerr << "could not get capture device" << std::endl;
		exit(1);
	}

	if(argc == 2) {
		std::string path(argv[1]);
		audio::file a_file(path);
		playback_device->play_file(&a_file);
	}
	


	audio::format fmt = audio::format::pcm_s16_le;
	unsigned long sample_rate = 48000;
	unsigned long channels = 1;
	unsigned long capture_seconds = 5;
	unsigned long frames_to_capture = sample_rate * capture_seconds;
	short buffer[channels * frames_to_capture];

	// configure devices
	playback_device->configure(fmt, sample_rate, channels);
	capture_device->configure(fmt, sample_rate, channels);
		

	// capture 5 seconds of audio
	std::cout << "starting capture" << std::endl;
	unsigned long capture_size = 128; // frames
	unsigned long pos = 0;
	capture_device->capture([&](unsigned long frames_read, unsigned long *read_frames)->short*{
		pos += (frames_read * channels);
		if(pos >= frames_to_capture || pos + capture_size >= frames_to_capture) {
			std::cout << "read complete" << std::endl;
			return NULL;
		}

		*read_frames = capture_size;
		return buffer + ((pos + capture_size) * channels);
	
			
	}); 

	std::cout << "starting playback" << std::endl;
	short *buf = buffer;
	unsigned long frames_remaining = frames_to_capture;
	
	playback_device->play([&](unsigned long played, unsigned long *remaining) -> short* {
		frames_remaining -= played;
		std::cout << frames_remaining << std::endl;
		if(frames_remaining > 0) {
			buf += (played * channels);
			*remaining = frames_remaining;
			return buf;
		} else {
			*remaining = 0;
			return NULL;
		}
		
	});

	
	
	

	

	
	delete playback_device;
	delete capture_device;
	
	return 0;
}
