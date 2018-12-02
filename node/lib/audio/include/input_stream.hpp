#ifndef INPUT_STREAM_HPP
#define INPUT_STREAM_HPP

#include "stream.hpp"
#include <functional>

namespace audio {

	class input_stream: private stream {
		public:
		typedef std::function<short*(unsigned long, unsigned long*)> callback;

		input_stream(char *descriptor, audio::format format, unsigned int sample_rate, unsigned int channels);
		~input_stream();

		void open(callback cb);
		void close() override;

	};

	
}


#endif