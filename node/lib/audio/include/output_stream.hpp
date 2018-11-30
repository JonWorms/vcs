#ifndef OUTPUT_STREAM_HPP
#define OUTPUT_STREAM_HPP

#include "alsa/asoundlib.h"
#include "audio_exception.hpp"
#include <string>
#include <functional>

namespace audio {

	
	
	class output_stream {
		public:

			// returns front of buffer
			// arg 0 is total frames played since last call
			// arg 1 is a pointer to be set indicating how my frames remain in data buffer 
			typedef std::function<short*(snd_pcm_sframes_t, snd_pcm_sframes_t*)> os_callback;
		
			output_stream(char *descriptor, snd_pcm_format_t f, unsigned int sr, unsigned int c);
			~output_stream();

			void open(os_callback callback);
			void close();

		private:
			std::string device_descriptor;
			snd_pcm_t *handle = NULL;
			snd_pcm_hw_params_t *hw_parameters = NULL;
			snd_pcm_sw_params_t *sw_parameters = NULL;
			snd_pcm_format_t format;
			unsigned int sample_rate;
			unsigned int channels;

			void open_pcm_device();
			void init_hw_params();
			void init_sw_params();

	};

	
}


#endif