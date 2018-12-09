#include "client.hpp"

using namespace vcs;

client::client() {
	
}

client::~client() {
	
}

void client::capture_sample() {
	std::cout << "client capture called" << std::endl;
	//interface::capture_sample();
}

void client::process_sample(char *sample, size_t length) {
	interface::process_sample(sample, length);
}