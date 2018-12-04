#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "pocketsphinx.h"
#include "language_model.hpp"
#include "stt_exception.hpp"

namespace stt {
	class engine {
		friend class hotword_parser;
		public:
			engine();
			void set_model(language_model model);
		protected:
			ps_decoder_t* create_decoder();
		private:
			cmd_ln_t *configuration;
	};
}


#endif

