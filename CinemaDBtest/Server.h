#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include<fstream>
#include"ClientThread.h"
#include"Function.h"
#include<thread>



using namespace std;

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
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);//����� ������������� ���������� �������
		if (result != 0)
		{
			cout << "wsa start failed" << endl;
			system("pause");
		}
		ZeroMemory(&hints, sizeof(hints));// ������ ��� ���������� ��������� ������
		hints.ai_family = AF_INET; //���� ��� ������ � �������
		hints.ai_socktype = SOCK_STREAM; //��������� �����
		hints.ai_protocol = IPPROTO_TCP;// �������� tcp
		hints.ai_flags = AI_PASSIVE;// ����� �������� �� �����, ��� �������� ��������� ����������
		result = getaddrinfo(ipAddress, port, &hints, &address);// ������������� ��������� �������� �����,port - ����� �����
		if (result != 0)
		{
			cout << "error getaddres info" << endl;
			system("pause");
		}
		serverSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);//�������� ������
		if (serverSocket == INVALID_SOCKET)
		{
			cout << "socket create error" << endl;
			system("pause");
		}
		bind(serverSocket, address->ai_addr, (int)address->ai_addrlen);//����������� ����� � ip 
		
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
		if (result == SOCKET_ERROR) // ������������� ���������� ������, SOMAXCONN - ������������ ���-�� tcp ����������
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
				std::thread clientThread(serverWork,clientSocket, this->dbCinemaConectPath);
				clientThread.detach();
			}

		}
	}
	
	
};