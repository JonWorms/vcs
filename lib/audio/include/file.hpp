#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include "sndfile.h"
#include "audio_exception.hpp"
#include "alsa/asoundlib.h"
#include "formats.hpp"


#define AUDIO_FILE_MODE_READ 1
#define AUDIO_FILE_MODE_WRITE 2

namespace audio {

	typedef struct {
		unsigned int num_channels;

		
		sf_count_t size; // in bytes
		sf_count_t offset; // in bytes;
		unsigned char data[1024];
	} file_context;



	class file {
		public:

		

		
		file(std::string path);
		file(std::string path, audio::format f, unsigned int channels, unsigned int sample_rate);
		~file();

		
		audio::format format();
		unsigned int channels();
		unsigned int  frames();
		unsigned int sample_rate();

		// reads the whole file, returns a pointer to the data, does not retain ownership of returned data
		short* read_data();
		unsigned long write_data(short* data, unsigned long frames);

		void flush();
		
		private:		
		file_context ctx;
		int rw_mode;
		audio::format a_format;
		
		SNDFILE *snd_file = NULL;
		SF_INFO snd_file_info;
		
		//static snd_pcm_format_t format_file_pcm(int pcm_format);

		//static void read_file_info(const char *path, SF_INFO *snd_file_info);
		
	};

	
}

#endif