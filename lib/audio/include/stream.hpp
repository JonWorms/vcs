#ifndef STREAM_HPP
#define STREAM_HPP

#include "asoundlib.h"
#include "audio_exception.hpp"
#include "formats.hpp"

#include <string>

namespace audio {

	typedef snd_pcm_sframes_t frames_t;

	class stream {
		protected:
			snd_pcm_t *handle = NULL;
		
			stream(char *descriptor, audio::format format, unsigned int sample_rate, unsigned int channels);
			~stream();
			
			void open(snd_pcm_stream_t type);
			virtual void close();


			bool is_open();
			
		private:
			std::string device_descriptor;
			snd_pcm_hw_params_t *hw_parameters = NULL;
			snd_pcm_sw_params_t *sw_parameters = NULL;
			snd_pcm_format_t format;
			unsigned int sample_rate;
			unsigned int channels;

			bool stream_open;

			void open_pcm_device(snd_pcm_stream_t type);
			void init_hw_params();

				
	};


	
}


#endif