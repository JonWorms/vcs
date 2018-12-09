#include "ssl.hpp"
#include "server_https.hpp"
#include <string>

namespace web {

	using HttpsServer = SimpleWeb::Server<SimpleWeb::HTTPS>;
	
	class server {
		public:
			server(std::string cert, std::string key);
			~server();
		private:
			//SimpleWeb::Server<SimpleWeb::HTTPS> *svr;
			 HttpsServer https;
	};
	
}