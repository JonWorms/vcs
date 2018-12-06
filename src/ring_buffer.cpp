#include "ring_buffer.hpp"


using namespace vcs;

ring_buffer::ring_buffer(size_t s) : buffer_size(s)  {
	data = (char*) malloc(s);
}

ring_buffer::~ring_buffer() {
	if(data != NULL) {
		free(data);
	}
}

size_t ring_buffer::length() {
	std::lock_guard<std::mutex> lock(idx);
	return data_length;
}



void ring_buffer::prep_rw(char **front, size_t &c_idx, size_t &n_idx, size_t &len, size_t &wrap_len) {
	*front = data + c_idx;
	n_idx = (c_idx + len) % buffer_size;
	if(n_idx < c_idx) { // wrap around
		len = buffer_size - c_idx;
		wrap_len = n_idx;
	} else {
		wrap_len = 0;
	}
}

void ring_buffer::perform_rw(char *front, size_t &length, size_t &wrap_length, rb_callback &op) {
	if(wrap_length > 0) {
		op(front, length);
		op(data, wrap_length);
	} else {
		op(front, length);
	}
}

void ring_buffer::read(size_t length, rb_callback read_op) {
	char *front;
	size_t next_read_idx;
	size_t wrap_length;
	
	idx.lock();
	if(data_length == 0) {
		idx.unlock();
		return;
	}
	prep_rw(&front, read_idx, next_read_idx, length, wrap_length);
	idx.unlock();

	perform_rw(front, length, wrap_length, read_op);

	idx.lock();
	data_length -= (length + wrap_length);
	read_idx = next_read_idx;
	idx.unlock();
	
}

void ring_buffer::write(size_t length, rb_callback write_op) {
	char *front;
	size_t next_write_idx;
	size_t wrap_length;
	
	idx.lock();
	if(read_idx == write_idx && data_length > 0) {
		data_length = 0;
		std::cout << "ring buffer overrun!" << std::endl;
	}
	prep_rw(&front, write_idx, next_write_idx, length, wrap_length);
	idx.unlock();

	perform_rw(front, length, wrap_length, write_op);

	idx.lock();
	data_length += (length + wrap_length);
	write_idx = next_write_idx;
	idx.unlock();
	

	
}