#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "device.hpp"
#include "file.hpp"



namespace audio {

	static std::vector<audio::device*> devices() {
		std::vector<audio::device*> d;

		int card = -1;
		while(snd_card_next(&card) >= 0 && card >= 0) {
			audio::device *device = new audio::device(card);
			d.push_back(device);
		}
	
		return d;
	}

	static device* get_device_named(std::string name) {
		std::vector<audio::device*> d = audio::devices();
		audio::device *dn = NULL;
		while(d.size() > 0 && !dn) {
			audio::device *ad = d.back();
			d.pop_back();
			if(ad->name() == name) {
				dn = ad;
			} else {
				delete ad;
			}
		}

		while(d.size() > 0) {
			audio::device *ad = d.back();
			d.pop_back();
			delete ad;
		}
		
		return dn;
	}
	
}



#endif