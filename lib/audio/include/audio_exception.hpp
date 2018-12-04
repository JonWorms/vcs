#ifndef AUDIO_EXCEPTION_HPP
#define AUDIO_EXCEPTION_HPP

#include <iostream>
#include <string>
#include "asoundlib.h"

namespace audio {

	using namespace std;

	class audio_exception : public exception {
		public:
		audio_exception(const string& msg, int err) : m_msg(msg) {
			string err_message(snd_strerror(err));
			m_msg += (" (" + std::to_string(err) + ":");
			m_msg += " " + err_message + ")";
		}

		audio_exception(const string& msg) : m_msg(msg) {};

		~audio_exception() {}

		virtual const char* what() const throw() {
			return m_msg.c_str();
		}

		private:
		string m_msg;
		
	};

	
}


#endif