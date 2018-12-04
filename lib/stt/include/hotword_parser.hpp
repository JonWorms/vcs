#ifndef HOTWORD_PARSER_HPP
#define HOTWORD_PARSER_HPP

#include <string>
#include <vector>

#include "pocketsphinx.h"
#include "stt_exception.hpp"
#include "engine.hpp"

namespace stt {

	class hotword_parser {
		public:
			hotword_parser(std::vector<std::string> hotwords, engine *eng);
			~hotword_parser();
			void add_hotword(std::string hotword);
			
		private:
			ps_decoder_t *decoder;
			
		
	};


	
}


#endif