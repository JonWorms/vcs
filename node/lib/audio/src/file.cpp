#include "file.hpp"

using namespace audio;

file::file(std::string path, int mode) {
	snd_file_info.format = 0;

	if(!(snd_file = sf_open(path.c_str(), mode,  &snd_file_info))) {
		throw audio_exception("could not open sound file");
	}

	printf("opened file: %s\n", path.c_str());
	printf("sample rate: %d\n", snd_file_info.samplerate);
	printf("channels: %d\n", snd_file_info.channels);
	printf("format: %d\n", snd_file_info.format);
	printf("sections: %d\n", snd_file_info.sections);
	printf("seekable: %d\n", snd_file_info.seekable);
	
	
}

file::~file() {
	sf_close(snd_file);
}

unsigned int file::channels() {
	return (unsigned int)snd_file_info.channels;
}

unsigned int file::sample_rate() {
	return (unsigned int)snd_file_info.samplerate;
}

snd_pcm_sframes_t file::frames() {
	return snd_file_info.frames;
} 

snd_pcm_format_t file::format() {
	int fmt = snd_file_info.format;
	
	if(fmt & SF_FORMAT_PCM_S8) {
		return SND_PCM_FORMAT_S8;
	}

	if(fmt & SF_FORMAT_PCM_16) {

		return SND_PCM_FORMAT_S16_LE;
	}

	if(fmt & SF_FORMAT_PCM_24) {
		return SND_PCM_FORMAT_S24_LE;
	}

	
	if(fmt & SF_FORMAT_PCM_32) {
		return SND_PCM_FORMAT_S32_LE;
	}

	throw audio_exception("unknown file format");
	
}

short* file::load() {
	short *file_data = (short*)malloc(sizeof(short*) * snd_file_info.channels * snd_file_info.frames);
	sf_read_short(snd_file, file_data, snd_file_info.frames);
	return file_data;
}