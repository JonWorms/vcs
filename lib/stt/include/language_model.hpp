#ifndef LANGUAGE_MODEL_HPP
#define LANGUAGE_MODEL_HPP
#include <vector>
#include <string>
#include <iostream>

#include "stt_config.hpp"
#include "stt_exception.hpp"

// TODO: shim for c++17
#include <experimental/filesystem>
namespace std {
	namespace filesystem = experimental::filesystem;
}



namespace stt {
	
	class language_model {
		friend class engine;
		public:
			
			static std::vector<language_model> languages();
			language_model(std::string model_path);
			
		protected:
			const char* hmm();
			const char* lm();
			const char* dict();
			const char* mllr();
		private:
			std::string hmm_path;
			std::string lm_path;
			std::string dict_path;
			std::string mllr_path;
			std::string name;
	};

	
}


#endif