#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "interface.hpp"
#include <iostream>

namespace vcs {
	class client : public interface {
		public:
			client();
			~client();
		protected:
			void capture_sample();
			void process_sample(char *sample, size_t length);
		private:
			
	};
}


#endif