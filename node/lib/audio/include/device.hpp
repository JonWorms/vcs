#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "alsa/asoundlib.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "file.hpp"
#include "input_stream.hpp"
#include "output_stream.hpp"

namespace audio {

	class device {
	private:
	
		int device_id;
		char device_descriptor[12];
		snd_ctl_t *handle = NULL;
		snd_ctl_card_info_t *info = NULL;
		snd_ctl_elem_list_t *elements = NULL;


		int num_inputs = -1;
		int num_outputs = -1;

		std::string device_name;

		bool open();
		bool close();


		void get_num_io();


			

		
	public:
		static int count();

		
		device(int id);
		~device();

		std::string name();
		
		void play_file(file *f);
		int get_num_inputs();
		int get_num_outputs();


		

		
	};

	//static std::vector<device*> devices();

	
}

#endif
