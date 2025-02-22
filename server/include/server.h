#pragma once
#include <serialization.h>
#include <iostream>
#include <cassert>
#include <thread>
#include <unordered_map>
#include <vector>
#include <WinSock2.h>

#include <ws2tcpip.h>
//-----------------------------------------------------------------------------------------------------------

#pragma comment(lib, "Ws2_32.lib") 

using namespace ObjectModel;

#define SIZE 1024

namespace Net{
	class Server {

	private:
		WSADATA wsa;                   
		SOCKET  serverSocket;

		std::string ipAddress;
		int         port;

		char        buffer[SIZE];
		std::string messege;

		struct sockaddr_in info;
		int    infoLength;
		int    recvLength; 

		std::unordered_map<std::string, Primitive> primitives;
		std::string current;

	public:
		Server(int, std::string);
		~Server();

	public:
		void start();
		void stop();

	private:
		void init();
		void receive(); 
		void process();
		void send();

		std::unique_ptr<Primitive> modify(std::string key);

	private:
		static void getIpAddrbyHost(const char* hostname, std::vector<std::string>& vec_ipstr);

	}; // classs Server
} // namespace Net