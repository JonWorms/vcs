#include "ssl_cert_location.h"
#include <string>
#include <iostream>

#include <experimental/filesystem>
namespace std {
	namespace filesystem = experimental::filesystem;
}


#define CERT_GEN_ERR_BN  1;
#define CERT_GEN_ERR_RSA (1 << 1);
#define CERT_GEN_ERR_EVP (1 << 2);

namespace web {
	class ssl {
		private:
			static bool ssl_file_exists(std::string filename);
			static bool ssl_directory_exists();
			static void generate_self_signed_cert();
			
		public:
			static void get_certificates(); //std::string &cert_file, std::string &private_key_file, std::string &verify_file

			
	};
}