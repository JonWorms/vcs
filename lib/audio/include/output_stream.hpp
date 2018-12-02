#ifndef OUTPUT_STREAM_HPP
#define OUTPUT_STREAM_HPP

#include "audio_exception.hpp"
#include "stream.hpp"
#include <string>
#include <functional>
#include "formats.hpp"

namespace audio {

	
	
	class output_stream: private stream {
		public:

			// returns front of buffer
			// arg 0 is total frames played since last call
			// arg 1 is a pointer to be set indicating how my frames remain in data buffer 
			typedef std::function<short*(unsigned long, unsigned long*)> callback;

			output_stream(char *descriptor, audio::format f, unsigned int sr, unsigned int c);
			~output_stream();

			void open(callback cb);
			void close() override;



	};

	
}


#endif