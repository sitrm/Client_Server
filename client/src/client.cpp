#include "../include/client.h" 

#pragma warning(disable: 4996)    

using namespace ObjectModel;

namespace Net {

	Client::Client(int port, std::string ipAddress)
		:
		wsa{ 0 },
		port(port),
		ipAddress(ipAddress),
		clientSocket(INVALID_SOCKET),
		info{ 0 },
		infoLength(sizeof(info)) {
		memset(buffer, 0, SIZE); 
	}
	//----------------------------------------------------------------------------------------------------

	void Client::init() {
		info.sin_family = AF_INET;       // ipv4
		info.sin_port = htons(port);	
		info.sin_addr.s_addr = inet_addr(ipAddress.c_str());     

		
		printf("WSA init\n");
		assert(!(WSAStartup(MAKEWORD(2, 2), &wsa)) && "Couldn`t init wsa!!!");
		printf("WSA success\n");

	
		printf("Creating Socket\n");
		assert(!((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) && "Couldn`t create Socket!!!");
		//   TCP (SOCK_STREAM)  UDP (SOCK_DGRAM) // 0 
		printf("Success Socket\n");


	}
	//----------------------------------------------------------------------------------------------------
	void Client::connect() {
		init();

		for (;;) {
			printf("\n");

			send();
			receive();  
			process();
		}
	}
	//----------------------------------------------------------------------------------------------------
	void Client::send() {

		printf("Please enter a messege: ");
		std::getline(std::cin, messege);

		if (messege == "prim") {

			int32_t foo = 53;
			int16_t it = 0;
			Primitive* p = Primitive::create("int32", Type::I32, foo);
			std::vector<int8_t> result(p->getSize());
			p->pack(&result, &it);
			std::copy(result.begin(), result.end(), buffer);

			if ((sendto(clientSocket, buffer, p->getSize(), 0, (struct sockaddr*)&info, infoLength)) == SOCKET_ERROR) {
				printf("send() failed...%d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

		}
		else {
			
			if ((sendto(clientSocket, messege.c_str(), messege.size(), 0, (struct sockaddr*)&info, infoLength)) == SOCKET_ERROR) {
				printf("send() failed...%d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
		}

	}
	//----------------------------------------------------------------------------------------------------
	void Client::receive() {
		
		if ((recvLength = recvfrom(clientSocket, buffer, SIZE, 0, (struct sockaddr*)&info, &infoLength)) == SOCKET_ERROR) {
			printf("receive() failed...%d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}
	//----------------------------------------------------------------------------------------------------
	void Client::process() {
		
		printf("Packet from:%s:%d\n", inet_ntoa(info.sin_addr), ntohs(info.sin_port));
		
		if (buffer[0] == 0x1) {
			std::vector<int8_t> result;
			for (unsigned int i = 0; i < recvLength; i++) {
				result.push_back(buffer[i]);
			}
			int16_t   it = 0;
			Primitive p = Primitive::unpack(&result, &it);
			


			printf("Primitive:\n");
			printf("\t |Name:%s\n ", p.getName().c_str());
			printf("\t |Size:%d\n ", p.getSize());
			printf("\t |Data:");

			for (auto i : p.getData()) {
				printf("[%d]", i);
			}

			printf("\n");

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
	//----------------------------------------------------------------------------------------------------
	Client::~Client() {
		WSACleanup();               
		closesocket(clientSocket);   
	}
	//----------------------------------------------------------------------------------------------------



} // namespace Net