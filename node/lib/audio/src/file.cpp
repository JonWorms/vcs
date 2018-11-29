#include "file.hpp"

using namespace audio;

file::file(std::string path, int mode) {
	if(!(snd_file = sf_open(path.c_str(), mode,  &snd_file_info))) {
		//TODO: Throw exception
	}

	printf("opened file: %s\n", path.c_str());
	printf("sample rate: %d\n", snd_file_info.samplerate);
	printf("channels: %d\n", snd_file_info.channels);
	printf("format: %d\n", snd_file_info.format);
	printf("sections: %d\n", snd_file_info.sections);
	printf("seekable: %d\n", snd_file_info.seekable);
	
	
}

file::~file() {
	if(snd_file) {
		sf_close(snd_file);
	}
}