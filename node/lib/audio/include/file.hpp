#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <iostream>
#include "sndfile.h"
#include "audio_exception.hpp"
#include "alsa/asoundlib.h"


namespace audio {

	class file {
		public:
		file(std::string path, int mode);
		~file();

		unsigned int channels();
		snd_pcm_format_t format();
		snd_pcm_sframes_t frames();
		unsigned int sample_rate();

		short* load();
	
		
		private:		

		SNDFILE *snd_file = NULL;
		SF_INFO snd_file_info;
		
		
	};

	
}

#endif