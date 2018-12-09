#include "interface.hpp"


using namespace vcs;
using namespace audio;


interface::interface() {
	
}
interface::~interface() {
	if(buffer != nullptr) {
		delete buffer;
		buffer = nullptr;
	}
}


void interface::start() {
	if(running) {
		return;	
	}

	if(input_device == nullptr || buffer == nullptr) {
		std::cout << "failed to start" << std::endl;
		return;
	}

	/*
	if(f == nullptr) {
		f = new file("test.wav", input_device->format, input_device->channels, input_device->sample_rate);
	}
	*/

	clear_buffer();
	running = true;
	capture_thread = new std::thread(&interface::capture_audio, this);
	process_thread = new std::thread(&interface::process_audio, this);
	
}

void interface::stop() {
	if(!running) {
		return;
	} 

	running = false;
	
	capture_thread->join();
	delete capture_thread;
	capture_thread = nullptr;


	read_cond.notify_all();
	
	process_thread->join();
	delete process_thread;
	process_thread = nullptr;

	/*
	if(f != nullptr) {
		f->flush();
		delete f;
	}*/
}


void interface::update_setting(std::function<void()> update) {
	bool was_running = interface::running;
	interface::stop();
	update();
	if(was_running) {
		interface::start();
	}
}

void interface::set_model(stt::language_model &m) {
	interface::update_setting([this, &m](){
		if(parser != nullptr) {
			delete parser;
		}
		stt_engine.set_model(m);
		parser = new stt::hotword_parser(&stt_engine);
	});
}

void interface::clear_buffer() {
	if(buffer != nullptr) {
		delete buffer;
	}
	buffer = new ring_buffer(buffer_size);	
}




void interface::set_buffer_settings(audio::frames_t ss, audio::frames_t bs) {
	if(bs % ss != 0) {
		throw exception();
	}
	update_setting([&](){
		if(buffer != nullptr) {
			delete buffer;
		}
		buffer_size = sizeof(short) * bs;
		sample_size = sizeof(short) * ss;
		buffer = new ring_buffer(buffer_size);
	});
}

void set_device(device **d, std::string &name) {
	device *device = *d;
	if(device != nullptr) {
		delete device;
	}
	device = get_device_named(name);
}

void interface::set_input_device(device *d) {
	interface::update_setting([&](){
		input_device = d;
	});
}

void interface::set_output_device(device *d) {
	interface::update_setting([&](){
		output_device = d;
	});
}

void interface::set_hotword(string hotword) {
	interface::update_setting([&](){
		parser->set_hotword(hotword);
	});
}

void interface::capture_audio() {
	//unsigned long frames_read;
	while(running) {
		buffer->write(sample_size, [&](char *to, size_t length_in_bytes){
			input_device->capture(length_in_bytes / sizeof(short), (short*) to);
		});
		read_cond.notify_all();
	}
}

/*
void interface::capture_sample() {
	
}
*/

void interface::wait_for_read() {
	bool wait = false;
	
	buffer_mutex.lock();
	wait = (buffer->length() == 0);
	buffer_mutex.unlock();

	if(!wait || !running) {
		return;
	}

	// TODO: timeout?
	std::unique_lock<std::mutex> mlock(buffer_mutex);
	read_cond.wait(mlock,[&](){
		return (buffer->length() > 0) || !running;
	});
}

void interface::process_audio() {

	wait_for_read();
	while(running) {
		buffer->read(sample_size, [&](char *from, size_t length_in_bytes){
			process_sample(from, length_in_bytes);
		});
		wait_for_read();
	}


}

void interface::process_sample(char *sample, size_t length) {
	//f->write_data((short*) sample, length / sizeof(short));
	parser->process_data((short*) sample, length / sizeof(short));
}
