#pragma once

#include <serialization.h>
#include <iostream>
#include <cassert>
#include <thread>
#include <unordered_map>
#include <vector>
#include <string>
#include <WinSock2.h>
//-----------------------------------------------------------------------------------------------------------

#pragma comment(lib, "Ws2_32.lib") 

#define SIZE 1024

namespace Net {
	class Client {

	private:
		WSADATA wsa;                  
		SOCKET  clientSocket;

		std::string ipAddress;
		int         port;

		char        buffer[SIZE];
		std::string messege;

		struct sockaddr_in info;
		int    infoLength;
		int    recvLength; 

	public:
		Client(int, std::string);
		~Client();

	public:
		void connect();
		

	private:
		void init();
		void receive();
		void process();
		void send();


	}; // classs Server
} // namespace Net