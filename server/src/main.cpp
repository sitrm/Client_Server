//#include <serialization.h>
#include "../include/server.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {

	if(argc != 3){
		std::cout << "By default: Port: 8888   IP: 127.0.0.1\n";
		Net::Server server(8888, "127.0.0.1");
		server.start();

	} else{

		std::string ip = argv[1];
		int port = std::stoi(argv[2]);\

		if (port < 0 || port > 65535) {
        	std::cerr << "Неверный порт. Порт должен быть в диапазоне 0-65535.\n";
        	return 1;
		} else {
			Net::Server server(port, ip);
			server.start();
 	   }
	}
	return 0;
}