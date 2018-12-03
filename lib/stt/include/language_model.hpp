#ifndef LANGUAGE_MODEL_HPP
#define LANGUAGE_MODEL_HPP
#include "stt.hpp"
#include <vector>
#include <string>
#include <iostream>

// using posix method to iterate directories until raspberry pi comes preloaded with c++17 support
#include <sys/types.h>
#include <dirent.h>


namespace stt {

	class language_model {
		public:
			static std::vector<std::string> languages();
			
		protected:
		
	};

	
}


#endif