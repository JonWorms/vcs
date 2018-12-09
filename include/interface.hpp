#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "audio.hpp"
#include "hotword_parser.hpp"
#include "ring_buffer.hpp"

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>

namespace vcs {



	class interface {
		public:
			interface();
			~interface();

			void start();
			void stop();
			
			void set_model(stt::language_model &model);
			void set_input_device(audio::device *d);
			void set_output_device(audio::device *d);
			void set_buffer_settings(audio::frames_t sample_size, audio::frames_t buffer_size);
			void set_hotword(std::string hotword);
			// temporary file for diagnostics
			//audio::file *f;

		protected:
			//virtual void capture_sample();
			virtual void process_sample(char *sample, size_t length);
			
		private:
			
			bool running = false;
			stt::engine stt_engine;
			stt::hotword_parser *parser;

			audio::device *input_device = nullptr;
			audio::device *output_device = nullptr;
			
			size_t sample_size = 0;
			size_t buffer_size = 0;
			
			std::mutex buffer_mutex;
			ring_buffer *buffer = nullptr;
			void clear_buffer();



			std::condition_variable read_cond;
			void wait_for_read();
			
			
			std::thread *capture_thread;
			std::thread *process_thread;

			void capture_audio();
			void process_audio();

			void update_setting(std::function<void()> update);

			
	};
}


#endif