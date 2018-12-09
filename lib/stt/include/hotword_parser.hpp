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
			hotword_parser(engine *eng);
			~hotword_parser();
			void set_hotword(std::string hotword);

			void process_data(short *buffer, size_t buffer_size);
			
		private:
			ps_decoder_t *decoder = NULL;
			bool in_speech = false;
			bool utt_started = false;
		
	};


	
}


#endif