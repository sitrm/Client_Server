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

	std::unique_ptr<Primitive> Server::modify(std::string key) {
		
		primitives.erase(key);
		int16_t bar = 75;
		std::unique_ptr<Primitive> p(Primitive::create("int16_t", Type::I16, bar));
		primitives.insert(std::make_pair(p->getName(), *p)); 

		current = p->getName();
		return std::move(p);  
	}

	//----------------------------------------------------------------------------------------------------
	Server::~Server() {
		WSACleanup();               
		closesocket(serverSocket);   
	}
	//----------------------------------------------------------------------------------------------------

} // namespace Net