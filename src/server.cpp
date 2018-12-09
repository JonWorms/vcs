#include "server.hpp"

using namespace web;
using namespace std;

server::server(string cert, string key) : https(cert, key) {
	
}

server::~server() {
	
}