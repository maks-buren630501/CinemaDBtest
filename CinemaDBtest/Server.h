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

#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

DWORD WINAPI serverWork(CONST LPVOID lpParam)
{

	ExitThread(0);
}


class Server
{
private:
	WSADATA wsaData;
	struct addrinfo *address; 
	struct addrinfo hints;
	SOCKET serverSocket;
	SOCKET clientSocket;
	int result;
public:
	Server(const char* ipAddress, const char* port)
	{
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
				HANDLE handleThread = CreateThread(NULL,0,&serverWork,(LPVOID)clientSocket,0,NULL);
				if (NULL == handleThread)
				{
					cout << "error create thread" << endl;
					return -3;
				}
			}

		}
	}
	
	
	


};