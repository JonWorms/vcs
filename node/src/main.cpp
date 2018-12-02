


//#include <stdlib.h>
//#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "file.hpp"
#include "audio.hpp"


int main(int argc, char *argv[]) {

	printf("number of audio devices: %d\n", audio::device::count());


	audio::device *playback_device = audio::get_device_named("USB Audio DAC");
	if(!playback_device) {
		std::cerr << "could not get device" << std::endl;
		exit(1);
	}



	

	if(argc == 2) {
		std::string argfile;
		argfile.assign(argv[1]);
		audio::file a_file(argfile);
		short *a_data = a_file.read_data();

		audio::file o_file("test.wav", a_file.format(), a_file.channels(), a_file.sample_rate());


		short *w_buf = a_data;
		unsigned long frames_written = 0;
		unsigned long write_len = 1024;
		while(frames_written < a_file.frames()) {
			unsigned long write = std::min(write_len, a_file.frames() - frames_written);
			write = o_file.write_data(w_buf, write);
			frames_written += write;
			w_buf += (write * a_file.channels());
		}
		o_file.flush();
		
		delete a_data;
	}
	
	

	

	
	delete playback_device;
	
	
	return 0;
}
