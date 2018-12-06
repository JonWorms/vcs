#include "client.hpp"


using namespace vcs;
using namespace audio;


client::client() {
	
}
client::~client() {
	if(buffer != nullptr) {
		delete buffer;
		buffer = nullptr;
	}
}


void client::start() {
	if(running) {
		return;	
	}

	if(input_device == nullptr || buffer == nullptr) {
		std::cout << "failed to start" << std::endl;
		return;
	}

	
	

	clear_buffer();
	running = true;
	capture_thread = new std::thread(&client::capture_audio, this);
	process_thread = new std::thread(&client::process_audio, this);
	
}

void client::stop() {
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

	if(f != nullptr) {
		f->flush();
		delete f;
	}
}


void client::update_setting(std::function<void()> update) {
	bool was_running = client::running;
	client::stop();
	update();
	if(was_running) {
		client::start();
	}
}

void client::set_model(stt::language_model &m) {
	client::update_setting([this, &m](){
		if(parser != nullptr) {
			delete parser;
		}
		stt_engine.set_model(m);
		parser = new stt::hotword_parser(&stt_engine);
	});
}

void client::clear_buffer() {
	if(buffer != nullptr) {
		delete buffer;
	}
	buffer = new ring_buffer(buffer_size);	
}




void client::set_buffer_settings(audio::frames_t ss, audio::frames_t bs) {
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

void client::set_input_device(device *d) {
	client::update_setting([&](){
		input_device = d;
	});
}

void client::set_output_device(device *d) {
	client::update_setting([&](){
		output_device = d;
	});
}

void client::capture_audio() {
	//unsigned long frames_read;
	while(running) {
		buffer->write(sample_size, [&](char *to, size_t length_in_bytes){
			input_device->capture(length_in_bytes / sizeof(short), (short*) to);
		});
		read_cond.notify_all();
	}
}



void client::wait_for_read() {
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

void client::process_audio() {
	f = new file("test.wav", input_device->format, input_device->channels, input_device->sample_rate);

	wait_for_read();
	while(running) {
		buffer->read(sample_size, [&](char *from, size_t length_in_bytes){
			f->write_data((short*) from, length_in_bytes / sizeof(short));
		});
		wait_for_read();
	}


}
