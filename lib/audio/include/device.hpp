#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "asoundlib.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "file.hpp"
#include "input_stream.hpp"
#include "output_stream.hpp"
#include "formats.hpp"

namespace audio {

	class device {

	public:
		static int count();

		
		device(int id);
		~device();

		std::string name();
		
		
		int get_num_inputs();
		int get_num_outputs();

		void configure(audio::format f, unsigned int sample_rate, unsigned int channels);
		//void capture(audio::input_stream::callback cb);
		void capture(frames_t frames, short *to_buffer);

		//audio::output_stream* create_output_stream();
		//audio::input_stream* create_input_stream();
		
		void play(audio::output_stream::callback cb);
		void play_data(short *data, unsigned long total_frames);
		void play_file(file *f);

				// audio format
		audio::format format;
		unsigned int channels;
		unsigned int sample_rate;

	private:
	
		int device_id;
		char device_descriptor[12];
		snd_ctl_t *handle = NULL;
		snd_ctl_card_info_t *info = NULL;
		snd_ctl_elem_list_t *elements = NULL;


		// audio format
		//audio::format format;
		//unsigned int channels;
		//unsigned int sample_rate;

		int num_inputs = -1;
		int num_outputs = -1;

		std::string device_name;

		bool open();
		bool close();


		void get_num_io();

		audio::input_stream *capture_stream = nullptr;
		audio::output_stream *playback_stream = nullptr;

			

		
			

		
	};

	//static std::vector<device*> devices();

	
}

#endif
