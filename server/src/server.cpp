#include "../include/server.h" 

#pragma warning(disable: 4996)    // ����� ����� 

using namespace ObjectModel;

namespace Net {

	Server::Server(int port, std::string ipAddress)
		:
		wsa{0}, 
		//recvLength(0),
		port(port),
		ipAddress(ipAddress),
		serverSocket(INVALID_SOCKET),
		info{0},
		infoLength(sizeof(info)) {
		memset(buffer, 0, SIZE);
		}
	//----------------------------------------------------------------------------------------------------
	 
	void Server::init() {
		info.sin_family = AF_INET;       // ipv4
		info.sin_port = htons(port);	
		info.sin_addr.s_addr = inet_addr(ipAddress.c_str());      // ip 

		
		printf("WSA init\n");
		assert(!(WSAStartup(514, &wsa)) && "Couldn`t init wsa!!!"); //MAKEWORD(2, 2)
		printf("WSA success\n");

		
		printf("Creating Socket\n");
		assert(!((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) && "Couldn`t create Socket!!!");
		//   TCP (SOCK_STREAM)  UDP (SOCK_DGRAM) // 0 - 
		printf("Success Socket\n");

		
		if (bind(serverSocket, (struct sockaddr*)&info, infoLength)) {
				std::cerr << "Errro bind socket!!!\n";
				__debugbreak(); 
			}
		printf("Socket binded\n");

		printf("Server started at:%s:%d\n", inet_ntoa(info.sin_addr), ntohs(info.sin_port)); 
		
	}
	//----------------------------------------------------------------------------------------------------
	void Server::start() {
		init();

		for (;;) {
			receive();   
			process();
			send();
		}
	} 
	//----------------------------------------------------------------------------------------------------
	void Server::stop() {
		if (serverSocket != INVALID_SOCKET) {
			closesocket(serverSocket);
			serverSocket = INVALID_SOCKET;
		}
		WSACleanup();                        
		printf("Server stopped!\n");
	}
	//----------------------------------------------------------------------------------------------------
	void Server::receive() {
	
		if ((recvLength = recvfrom(serverSocket, buffer, SIZE, 0, (struct sockaddr*)&info, &infoLength)) == SOCKET_ERROR) {
			printf("receive() failed...%d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}

	//----------------------------------------------------------------------------------------------------
	void Server::process() {
		
		printf("Packet from:%s:%d\n", inet_ntoa(info.sin_addr), ntohs(info.sin_port));

		if (buffer[0] == 0x1) {
			std::vector<int8_t> result;
			for (unsigned int i = 0; i < recvLength; i++) {
				result.push_back(buffer[i]);
			}
			int16_t it = 0;
			Primitive p = Primitive::unpack(&result, &it);
			primitives.insert(std::make_pair(p.getName(), p)); 
			current = p.getName();


			printf("Primitive:\n");
			printf("\t |Name: %s\n ", p.getName().c_str());
			printf("\t |Size: %d\n ", p.getSize());
			printf("\t |Data: ");
			
			for (auto i : p.getData()) {
				printf("[%d]", i);
			}

			printf("\n");

		}
		else if (recvLength == 4 && (strncmp(buffer, "stop", 4)) == 0) {
			printf("Client requested to stop the server.\n");
			stop();                  
			exit(EXIT_SUCCESS);         
		}
		else {
			printf("DATA: ");
			for (unsigned int i = 0; i < recvLength; i++) {
				printf("%c", buffer[i]);                        
			}
			printf("\n");
		}
		
	}
	//----------------------------------------------------------------------------------------------------
	void Server::send() {

		if (primitives.empty()) { 

			if ((sendto(serverSocket, buffer, recvLength, 0, (struct sockaddr*)&info, infoLength)) == SOCKET_ERROR) {
				printf("send() failed...%d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
		}
		else {
			
			int16_t it = 0;

			std::unique_ptr<Primitive> p = modify(current);
			std::vector<int8_t> result(p->getSize());

			p->pack(&result, &it);
			std::copy(result.begin(), result.end(), buffer);


			if ((sendto(serverSocket, buffer, p->getSize(), 0, (struct sockaddr*)&info, infoLength)) == SOCKET_ERROR) {
				printf("send() failed...%d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			primitives.erase(current);  
		}

	}
	//----------------------------------------------------------------------------------------------------
	std::unique_ptr<Primitive> Server::modify(std::string key) {
		
		primitives.erase(key);
		int16_t bar = 75;
		std::unique_ptr<Primitive> p(Primitive::create("int16_t", Type::I16, bar));
		primitives.insert(std::make_pair(p->getName(), *p)); 

		current = p->getName();
		return std::move(p);  
	}
	//----------------------------------------------------------------------------------------------------
	void Server::getIpAddrbyHost(const char* hostname, std::vector<std::string>& vec_ipstr) {
		// Инициал WinSock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			fprintf(stderr, "WSAStartup failed.\n");
			__debugbreak();
		}

		struct addrinfo hints, * res;
		int status;
		//char* ipstr[INET6_ADDRSTRLEN];
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;     //ipv4 and ipv6
		hints.ai_socktype = SOCK_STREAM;

		//getaddrinfo(hostname, NULL, &hints, &res);
		if ((status = getaddrinfo(hostname, "http", &hints, &res) != 0)) {
			fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
			WSACleanup();
			__debugbreak();
		}

		for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
			void* addr;  // потому что не понятно или ipv4 или ipv6 
			const char* ipversion;
			// получаем указаетль в зависимости от версии
			if (p->ai_family == AF_INET) {   //ipv4 
				// чтобы вытащить адрес приводим к типу sockaddr_in->sin_addr
				struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
				addr = &(ipv4->sin_addr);
				ipversion = "IPv4";
			}
			else { // IPv6
				struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
				addr = &(ipv6->sin6_addr);
				ipversion = "IPv6";
			}
			char ipstr[INET6_ADDRSTRLEN];
			// преобразуем в норм вид 
			inet_ntop(p->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
			vec_ipstr.push_back(ipstr);

			//printf("IP addresses for %s:\n", hostname);
			//printf("%s: %s\n\n", ipversion, ipstr);
		}
		// Освобождаем память
		freeaddrinfo(res);

		// Завершаем работу с Winsock
		WSACleanup();
	}
	//----------------------------------------------------------------------------------------------------
	Server::~Server() {
		WSACleanup();               
		closesocket(serverSocket);   
	}
	//----------------------------------------------------------------------------------------------------

} // namespace Net