


#include <stdlib.h>
#include <stdio.h>
#include "audio.h"
#include <vector>
#include <iostream>
//#include "alsa/asoundlib.h"

/*
void print_card_info(snd_ctl_card_info_t *info)
{
        printf("Card #%d: id=%s, driver=%s, name=%s, longname=%s, mixername=%s, components=%s\n",
                snd_ctl_card_info_get_card(info),
                snd_ctl_card_info_get_id(info),
                snd_ctl_card_info_get_driver(info),
                snd_ctl_card_info_get_name(info),
                snd_ctl_card_info_get_longname(info),
                snd_ctl_card_info_get_mixername(info),
                snd_ctl_card_info_get_components(info));
}

// for now, let's see if we can get ALSA to list inputs and outputs

typedef struct {
	int id;
	char name[6];
	
} sound_card;


int get_num_audio_devices() {
	int card = -1;
	int count = 0;	
	while(snd_card_next(&card) >= 0 && card >= 0) {
		count++;
	}
	return count;
}

void list_device_names() {
	int card = -1;
	char cardascii[6];
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;
	snd_ctl_elem_list_t *elements;
	
	if(snd_ctl_card_info_malloc(&info)) {
		printf("could not allocate card info\n");
		return;
	}

	if(snd_ctl_elem_list_malloc(&elements)) {
		printf("could not allocate element list\n");
		snd_ctl_card_info_free(info);
		return;
	}
	
	while(snd_card_next(&card) >= 0 && card >= 0) {

		snprintf(cardascii, sizeof(cardascii), "hw:%d", card);
		if(!snd_ctl_open(&handle, cardascii, 0)) {
			if(!snd_ctl_card_info(handle, info)) {
				print_card_info(info);
			}

			
			snd_ctl_close(handle);
		}

		printf("\n");
		
	}	

	snd_ctl_card_info_free(info);
	snd_ctl_elem_list_free(elements);
}

int snd_device_is_input(void *hint) {
	char *hint_str;
	int is_input = 0;
	if((hint_str = snd_device_name_get_hint(hint, "IOID"))) {
		is_input = !strcmp(hint_str, "Input");
		free(hint_str);
	} else {
		is_input = 1;
	}
	return is_input;
}

void list_input_devices() {
	int card = -1;

	while(snd_card_next(&card) >= 0 && card >= 0) {
		void **hints;
		if(!snd_device_name_hint(card, "pcm", &hints)) {
			void **hint = hints;
			while(*hint) {
				char *name = snd_device_name_get_hint(*hint, "NAME");
				if(name && snd_device_is_input(*hint)) {
					printf("%s\n", name);
					free(name);
					break;
				} 
				
				free(name);
				hint += 1;
			}
			
			snd_device_name_free_hint(hints);
		}		
	}	

}

int get_input_devices(int** input_devices) {
	int devices[get_num_audio_devices()];
	int idx = 0;

	int card = -1;
	while(snd_card_next(&card) >= 0 && card >= 0) {
		void **hints;
		if(!snd_device_name_hint(card, "pcm", &hints)) {
			void **hint = hints;
			while(*hint) {
				char *name = snd_device_name_get_hint(*hint, "NAME");
				if(name && snd_device_is_input(*hint)) {
					devices[idx] = card;
					idx++;
					free(name);
					break;
				} 
				free(name);
				hint += 1;
			}
			snd_device_name_free_hint(hints);
		}		
	}	
	
	if(idx > 0) {
		*input_devices = malloc(sizeof(int) * idx);
		for(card = 0; card < idx ; card++) {
			*input_devices[card] = devices[card];
		}
		return idx;
	}
	

	return 0;
}
*/
int main(int argc, char *argv[]) {

	printf("number of audio devices: %d\n", audio::device::count());

	std::vector<audio::device*> devices = audio::device::devices();


	while(devices.size() > 0) {
		audio::device *device = devices.back();
		std::cout << device->name();
		std::cout << " inputs: ";
		std::cout << device->get_num_inputs();
		std::cout << " outputs: ";
		std::cout << device->get_num_outputs();
		std::cout << std::endl;
		devices.pop_back();
		delete device;
	}
	
	/*
	int *input_devices = NULL;
	int num_inputs = get_input_devices(&input_devices);
	int d;
	for(d = 0 ; d < num_inputs ; d++) {
		printf("device %d has inputs\n", input_devices[d]);
	}

	if(input_devices) {
		free(input_devices);
	}
	*/
	
	return 0;
}
