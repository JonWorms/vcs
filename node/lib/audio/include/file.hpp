#ifndef FILE_HPP
#define FILE_HPP

#include "sndfile.h"
#include <string>
#include <iostream>

#define AFM_READ 0
#define AFM_WRITE 1
#define AFM_RDWR 2

namespace audio {

	class file {
		public:
		file(std::string path, int mode);
		~file();

		private:		

		SNDFILE *snd_file = NULL;
		SF_INFO snd_file_info;

		
	};

	
}

#endif