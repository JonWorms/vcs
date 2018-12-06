#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <functional>
#include <mutex>
#include <iostream>

namespace vcs {

	

	class ring_buffer {
		public:
			ring_buffer(size_t size);
			~ring_buffer();
	
			typedef std::function<void(char* buffer, size_t length)> rb_callback;
			size_t length();

			void read(size_t length, rb_callback read_op);
			void write(size_t length, rb_callback write_op);


			
		private:

			std::mutex idx;
			size_t data_length;
			const size_t buffer_size;

			size_t read_idx;			
			size_t write_idx;
			
			char *data = NULL;


			void prep_rw(char **front, size_t &c_idx, size_t &n_idx, size_t &len, size_t &wrap_len);
			void perform_rw(char *front, size_t &length, size_t &wrap_length, rb_callback &op);
	};
}

#endif