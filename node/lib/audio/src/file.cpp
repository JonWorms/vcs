#include "file.hpp"

using namespace audio;






file::file(std::string path) : rw_mode(AUDIO_FILE_MODE_READ) {
	memset(&snd_file_info, 0, sizeof(snd_file_info));

	if(!(snd_file = sf_open(path.c_str(), SFM_READ,  &snd_file_info))) {
		throw audio_exception("could not open sound file");
	}	

	file::a_format = sndfile_format(snd_file_info.format);
	
}

file::file(std::string path, audio::format f, unsigned int channels, unsigned int sample_rate) : rw_mode(AUDIO_FILE_MODE_WRITE), a_format(f) {
	memset(&snd_file_info, 0, sizeof(snd_file_info));
	memset(&ctx, 0, sizeof(ctx));

	snd_file_info.format = (SF_FORMAT_WAV | format_sndfile(f));
	snd_file_info.channels = channels;
	snd_file_info.samplerate = sample_rate;

	if(!(snd_file = sf_open(path.c_str(), SFM_WRITE,  &snd_file_info))) {
		throw audio_exception("could not open sound file");
	}	
	
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

unsigned int file::frames() {
	return snd_file_info.frames;
} 

audio::format file::format() {
	return file::a_format;
}

short* file::read_data() {
	
	short *file_data = (short*) malloc(snd_file_info.channels * snd_file_info.frames * sizeof(short));
	sf_readf_short(snd_file, file_data, snd_file_info.frames);
	return file_data;

}

unsigned long file::write_data(short* data, unsigned long frames) {
	return sf_writef_short(snd_file, data, frames);
}

void file::flush() {
	sf_write_sync(snd_file);
}