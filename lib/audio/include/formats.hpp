#ifndef FORMATS_HPP
#define FORMATS_HPP

#include "asoundlib.h"
#include "sndfile.h"
#include "audio_exception.hpp"

namespace audio {

	enum format {
		pcm_s8,
		pcm_s16_le,
		pcm_s24_le,
		pcm_s32_le,
	};

	static snd_pcm_format_t format_alsa(audio::format fmt) {
		switch(fmt) {
			case pcm_s8:
				return SND_PCM_FORMAT_S8;
			case pcm_s16_le:
				return SND_PCM_FORMAT_S16_LE;
			case pcm_s24_le:
				return SND_PCM_FORMAT_S24_LE;
			case pcm_s32_le:
				return SND_PCM_FORMAT_S32_LE;
		}
	}

	static int format_sndfile(audio::format fmt) {
		switch(fmt) {
			case pcm_s8:
				return SF_FORMAT_PCM_S8;
			case pcm_s16_le:
				return SF_FORMAT_PCM_16;
			case pcm_s24_le:
				return SF_FORMAT_PCM_24;
			case pcm_s32_le:
				return SF_FORMAT_PCM_32;
		}
	}

	static format sndfile_format(int fmt) {
		if(fmt & SF_FORMAT_PCM_S8) {
			return pcm_s8;
		}

		if(fmt & SF_FORMAT_PCM_16) {
			return pcm_s16_le; 
		}

		if(fmt & SF_FORMAT_PCM_24) {
			return pcm_s24_le; 
		}

		if(fmt & SF_FORMAT_PCM_32) {
			return pcm_s32_le; 
		}

		throw audio_exception("unknown audio format");
	}
}


#endif