#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include<fstream>
#include"CinemaDB.h"
#include"Function.h"
#include <windows.h>
#include<thread>
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

const int maxBufferSize = 1024;

using namespace std;

void serverWork(int clientSocket,CinemaDB *cinemaDB)
{
	while (true)
	{
		char requestBuffer[maxBufferSize+1];
		int requestLength;
		string stringOfRequest;
		requestLength = recv(clientSocket,requestBuffer, maxBufferSize,0);
		if (requestLength == SOCKET_ERROR)// ошибка получения данных
		{
			cout << "recv error" << endl;
			break;
		}
		if (requestLength == 0)
		{
			cout << "empty reqv" << endl;
			continue;
		}
		requestBuffer[requestLength] = '\0';
		stringOfRequest = requestBuffer;
		if (stringOfRequest.find("checkUserNickName") != std::string::npos)
		{
			workWithUserAddRequest(cinemaDB,clientSocket,stringOfRequest);
		}


	}
}


int workWithUserAddRequest(CinemaDB *cinemaDB, int clientSocket, string request)
{
	string answer;
	list<string> listOfUsers = cinemaDB->getListOfUsers();
	string nickName = getNickNameFromRequest(request);
	if (std::find(listOfUsers.begin(), listOfUsers.end(), nickName) == listOfUsers.end())
	{
		answer = "nickNameIsFree";
		send(clientSocket,answer.c_str(),answer.length(),0);
		return 0;
	}
	else
	{
		answer = "nickNameIsBusy";
		send(clientSocket, answer.c_str(), answer.length(), 0);
		return 1;
	}
}

class Server
{
private:
	CinemaDB *cinemaDB;
	WSADATA wsaData;
	struct addrinfo *address; 
	struct addrinfo hints;
	SOCKET serverSocket;
	SOCKET clientSocket;
	int result;

public:
	Server(const char* ipAddress, const char* port, const char* conectPath)
	{
		cinemaDB = new CinemaDB(conectPath);
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);//старт использования библиотеки сокетов
		if (result != 0)
		{
			cout << "wsa start failed" << endl;
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
			cout << "error getaddres info" << endl;
			system("pause");
		}
		serverSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);//создание сокета
		if (serverSocket == INVALID_SOCKET)
		{
			cout << "socket create error" << endl;
			system("pause");
		}
		result = bind(serverSocket, address->ai_addr, (int)address->ai_addrlen);//привязываем сокет к ip 
		if (result == SOCKET_ERROR)
		{
			cout << "socket addres error" << endl;
			system("pause");
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
			cout << "listen socket error" << endl;
			system("pause");
			return -1;
		}
		
		sockaddr *addr = NULL;

		while (TRUE)
		{
			clientSocket = accept(serverSocket, addr, NULL);
			if (clientSocket == INVALID_SOCKET)
			{
				cout << "client_socket error" << endl;
				system("pause");
				return -2;
			}
			else
			{
				std::thread clientThread(serverWork,clientSocket,std::ref(this->cinemaDB));
				clientThread.detach();
			}

		}
	}
	
	
};