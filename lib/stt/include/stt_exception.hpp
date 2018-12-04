#ifndef STT_EXCEPTION_HPP
#define STT_EXCEPTION_HPP

#include <exception>
#include <iostream>


namespace stt {
	class exception: public std::exception {
		public:
			exception(std::string msg) : what_message(msg) {}
			
		protected:
		
		virtual const char* what() const throw() {
			return what_message.c_str(); 
		}
		
		private:
			std::string what_message;
	};

}




#endif