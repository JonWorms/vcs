#include "device.hpp"

using namespace audio;
using namespace std;

device::device(int id) : device_id(id) {
	snprintf(device_descriptor, sizeof(device_descriptor), "hw:%d", id);
	std::cout << "opening device ";
	std::cout << device_descriptor;
	std::cout << std::endl;
	snd_ctl_card_info_malloc(&info);
	
	if(!info) {
		fprintf(stderr, "could not laod snd_ctl_info\n");
	}
}

device::~device() {
	std::cout << "deleting device" << std::endl;
	device::close();
	snd_ctl_card_info_free(info);
}


bool device::open() {

	if(handle) {
		return true;
	}

	if(!snd_ctl_open(&handle, device_descriptor, 0)) {
		return true;
	}
	
	handle = NULL;
	return false;
}

bool device::close() {
	if(!handle) {
		return true;
	}

	if(!snd_ctl_close(handle)) {
		handle = NULL;
		return true;
	}

	return false;
}

string device::name() {

	if(device_name.length() > 0) {
		return device_name;
	}

	if(!device::open()) {
		return NULL;
	}

	if(!snd_ctl_card_info(handle, info)) {
		device_name.assign(snd_ctl_card_info_get_name(info)); // pretty sure this does not leak
	}

	device::close();

	return device_name;
	
}


void device::get_num_io() {
	device::open();
	void **hints;
	if(!snd_device_name_hint(device_id, "pcm", &hints)) {
		void **hint = hints;
		num_inputs = 0;
		num_outputs = 0;
		while (*hint) {
			char *name = snd_device_name_get_hint(*hint, "NAME");
			
			if(name) {
				char *io_str = snd_device_name_get_hint(*hint, "IOID");
				if(io_str) {
					num_inputs += (!strcmp(io_str, "Input"));
					num_outputs += (!strcmp(io_str, "Output"));
					free(io_str);
				} else {
					num_inputs += 1;
					num_outputs += 1;
				}

				free(name);
			}
			hint++;
		}		
	}
	device::close();
}


int device::get_num_inputs() {
	if(num_inputs < 0) {
		device::get_num_io();
	}

	return num_inputs;
}


int device::get_num_outputs() {
	if(num_outputs < 0) {
		device::get_num_io();
	}

	return num_outputs;
}




void device::play_file(audio::file *file) {
	output_stream os(device_descriptor, file->format(), file->sample_rate(), file->channels());
	short *fd = file->read_data();
	short *front = fd;
	unsigned int chnls = file->channels();
	snd_pcm_sframes_t frames_remaining = file->frames();
	os.open([&](snd_pcm_sframes_t played, snd_pcm_sframes_t *remaining) -> short* {
		frames_remaining -= played;

		if(frames_remaining > 0) {
			front += (played * chnls);
			*remaining = frames_remaining;
			return front;
		} else {
			*remaining = 0;
			free(fd);
			os.close();
			return NULL;
		}
	});
	
}




int device::count() {
	int card = -1;
	int count = 0;
	while(snd_card_next(&card) >= 0 && card >= 0) {
		count++;
	}
	return count;	
}

/*
vector<device*> audio::devices() {

	

	vector<device*> d;

	int card = -1;
	while(snd_card_next(&card) >= 0 && card >= 0) {
		d.push_back(new device(card));
	}
	 
	return d;
}
*/
