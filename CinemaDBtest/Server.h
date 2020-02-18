#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include<fstream>
#include"ClientThread.h"
#include"Function.h"
#include<thread>
#include<winsock.h>



using std::cout;
using std::endl;
using std::string;


void serverWork(int clientSocket,string conectPath)
{
	ClientThread *clientThread = new ClientThread(clientSocket,conectPath.c_str());
	if (clientThread->workClient() < 0)
	{
		cout << "error of client thread working" << endl;
		delete clientThread;
		return;
	}
	else
	{
		cout << "end work of client thread" << endl;
		delete clientThread;
		return;
	}
	

}

class Server
{
private:
	string dbCinemaConectPath;
	WSADATA wsaData;
	struct addrinfo *address; 
	struct addrinfo hints;
	SOCKET serverSocket;
	SOCKET clientSocket;
	int result;

public:
	Server(const char* ipAddress, const char* port, const char* conectPath)
	{
		this->dbCinemaConectPath = conectPath;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);//старт использования библиотеки сокетов
		if (result != 0)
		{
			std::cout << "wsa start failed" << endl;
			system("pause");
		}
		ZeroMemory(&hints, sizeof(hints));// шаблон для реализации структуры адреса
		hints.ai_family = AF_INET; //сеть для работы с сокетом
		hints.ai_socktype = SOCK_STREAM; //потоковый сокет
		hints.ai_protocol = IPPROTO_TCP;// протокол tcp
		hints.ai_flags = AI_PASSIVE;// сокет биндится на адрес, для принятия входящего соединения
		result = getaddrinfo(ipAddress, port, &hints, &address);// инициализация структуры хранящий адрес,port - номер порта
		if (result != 0)
		{
			std::cout << "error getaddres info" << endl;
			system("pause");
		}
		serverSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);//создание сокета
		if (serverSocket == INVALID_SOCKET)
		{
			std::cout << "socket create error" << endl;
			system("pause");
		}
		result = bind(serverSocket, address->ai_addr, (int)address->ai_addrlen);
		if (result < 0)
		{
			cout << "socket bind error" << endl;
		}
		
		
		
	}
	
	~Server()
	{
		closesocket(serverSocket);
		freeaddrinfo(address);
		WSACleanup();
	}
	
	int setServerToListen()
	{
		result = listen(serverSocket, SOMAXCONN);
		if (result == SOCKET_ERROR) // инициализация слущающего сокета, SOMAXCONN - максимальное кол-во tcp соединений
		{
			std::cout << "listen socket error" << endl;
			system("pause");
			return -1;
		}
		
		sockaddr *addr = NULL;

		while (TRUE)
		{
			clientSocket = accept(serverSocket, addr, NULL);
			std::cout << "connection accept" << endl;
			if (clientSocket == INVALID_SOCKET)
			{
				std::cout << "client_socket error" << endl;
				system("pause");
				return -2;
			}
			else
			{
				cout << "new connect" << endl;
				std::thread clientThread(serverWork,clientSocket, this->dbCinemaConectPath);
				clientThread.detach();
			}

		}
	}
	
	
};