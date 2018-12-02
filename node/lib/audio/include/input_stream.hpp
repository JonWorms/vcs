#ifndef INPUT_STREAM_HPP
#define INPUT_STREAM_HPP

#include "stream.hpp"

namespace audio {

	class input_stream: private stream {
		public:

		input_stream(char *descriptor, audio::format f, unsigned int sr, unsigned int c);
		~input_stream();

		void open();
		void close();

	};

	
}


#endif