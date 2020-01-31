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

class ClientThread
{
private:
	CinemaDB *cinemaDB;
	int clientSocket;
	char requestBuffer[maxBufferSize + 1];
	int requestLength;
	int result;
	string stringOfRequest;
	string nickName;
	string answer;

	//func to check in data base nickname of new user and if nickname is free add to this class, but no to data base
	int checkUserNickName()
	{
		list<string> listOfUsers = this->cinemaDB->getListOfUsers();
		string nickName = getNickNameFromRequest(this->stringOfRequest);
		if (std::find(listOfUsers.begin(), listOfUsers.end(), nickName) == listOfUsers.end())
		{
			this->answer = "nickNameIsFree";
			this->result = send(clientSocket, answer.c_str(), answer.length(), 0);
			if (this->result < 0)
			{
				return -1;
			}
			else
			{
				this->nickName = nickName;
				return 0;
			}
			
		}
		else
		{
			this->answer = "nickNameIsBusy";
			this->result = send(clientSocket, answer.c_str(), answer.length(), 0);
			if (this->result < 0)
			{
				return -1;
			}
			else
			{
				return 1;
			}
		}
	}
	
	//func to add new user to data base, in the begin we know nickname add will get last and first name from text of request
	int addNewUserToDB()
	{
		string lastName = getLastNameFromRequest(this->stringOfRequest);
		string firstName = getFirstNameFromRequest(this->stringOfRequest);
		this->result = this->cinemaDB->insertUser(this->nickName.c_str(),lastName.c_str(),firstName.c_str());
		if (this->result < 0)
		{
			this->answer = "errorOfAddingToDB";
			this->result = send(clientSocket, answer.c_str(), answer.length(), 0);
			if (this->result < 0)
			{
				return -1;
			}
			else
			{
				return -2;

			}
			
		}
		else 
		{
			this->answer = "sucsesAddingToDB";
			this->result = send(clientSocket, answer.c_str(), answer.length(), 0);
			if (this->result < 0)
			{
				return -1;
			}
			else
			{
				return 0;

			}
		}

	}

	//func to send user list of films from data base
	int sendListOfFilms()
	{
		string listOfFilms = this->cinemaDB->getListStringNameOfFilm();
	}

	
public:
	ClientThread(int clientSocket, const char* conectPath)
	{
		this->nickName = "unknow";
		this->clientSocket = clientSocket;
		this->cinemaDB = new CinemaDB(conectPath);
	}

	~ClientThread()
	{
		delete this->cinemaDB;
		closesocket(this->clientSocket);
	}

	int workClient()
	{
		while (true)
		{
			this->requestLength = recv(this->clientSocket, this->requestBuffer, maxBufferSize, 0);

			//chek of reception error
			if (this->requestLength == SOCKET_ERROR)// ошибка получения данных
			{
				cout << "recv error" << endl;
				return -1;
				break;
			}
			
			//chek of empty request
			if (this->requestLength == 0)
			{
				cout << "empty reqv" << endl;
				continue;
			}

			this->requestBuffer[requestLength] = '\0';
			this->stringOfRequest = this->requestBuffer;
			
			//request type is check user nickname is busy or no
			if(stringOfRequest.find("checkUserNickName") != std::string::npos)
			{
				this->result = this->checkUserNickName();
				if (this->result < 0)
				{
					cout << "error of sending answer to client" << endl;
				}
				else
					if(this->result == 0)
					{
						cout << "nickName is free" << endl;
					}
					else
					{
						cout << "nickName is busy" << endl;
					}
			}
			
			//request to add new user to data base
			if (stringOfRequest.find("addNewUserToDB") != std::string::npos)
			{
				this->result = this->addNewUserToDB();
				if (this->result == 0)
				{
					cout << "sucsess adding new user to database" << endl;
				}
				else
					if (this->result == -1)
					{
						cout << "error of sending answer to client" << endl;
					}
					else
					{
						cout << "error of adding new user to database" << endl;
					}
			}
			
			//request to send to user list of films
			if (stringOfRequest.find("getListOfFilm") != std::string::npos)
			{
				this->result = this->sendListOfFilms();

			}
		}
	}

	
};