#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include<fstream>
#include"Function.h"

using std::cout;
using std::endl;
using std::string;


class AdminThread
{
private:
	CinemaDB *cinemaDB;
	int clientSocket;
	char requestBuffer[maxBufferSize + 1];
	int requestLength;
	int result;
	string stringOfRequest;
	string answer;

	int sendListOfUsers()
	{
		string stringListOfUsers = getStringListOfUsers(this->cinemaDB->getListOfUsers());
		stringListOfUsers += '\0';
		this->result = send(clientSocket, stringListOfUsers.c_str(), stringListOfUsers.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	int sendListOfFilms()
	{
		string listOfFilms = this->cinemaDB->getListOfFilms();
		listOfFilms += '\0';
		this->result = send(clientSocket, listOfFilms.c_str(), listOfFilms.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	int addNewUserToDB()
	{
		string nickName = getParameterByNumber(this->stringOfRequest, 2);
		string lastName = getParameterByNumber(this->stringOfRequest, 3);
		string firstName = getParameterByNumber(this->stringOfRequest, 4);
		this->result = this->cinemaDB->insertUser(nickName.c_str(), lastName.c_str(), firstName.c_str());
		if (this->result < 0)
		{
			this->answer = "errorOfAddingToDB|\0";
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
			this->answer = "successfullyAddingToDB|\0";
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

	int checkUserNickName()
	{
		list<string> listOfUsers = this->cinemaDB->getListOfUsers();
		string nickName = getParameterByNumber(this->stringOfRequest, 2);
		cout << "nick = " << nickName << endl;
		if (std::find(listOfUsers.begin(), listOfUsers.end(), nickName) == listOfUsers.end())
		{
			this->answer = "nickNameIsFree|\0";
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
		else
		{
			this->answer = "nickNameIsBusy|\0";
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

	int addNewFilmToDB()
	{
		string filmTitle = getParameterByNumber(this->stringOfRequest, 2);
		string country = getParameterByNumber(this->stringOfRequest, 3);
		string date = getParameterByNumber(this->stringOfRequest, 4);
		this->result = this->cinemaDB->insertFilm(filmTitle.c_str(), country.c_str(), date.c_str());
		if (this->result < 0)
		{
			this->answer = "errorOfAddingToDB|\0";
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
			this->answer = "successfullyAddingToDB|\0";
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

	int addNewSessionToDB()
	{
		string filmTitle = getParameterByNumber(this->stringOfRequest, 2);
		string date = getParameterByNumber(this->stringOfRequest, 3);
		string time = getParameterByNumber(this->stringOfRequest, 4);
		string hall = getParameterByNumber(this->stringOfRequest, 5);
		this->result = this->cinemaDB->insertSession(filmTitle.c_str(), date.c_str(), time.c_str(), std::stoi(hall));
		if (this->result < 0)
		{
			this->answer = "errorOfAddingToDB|\0";
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
			this->answer = "successfullyAddingToDB|\0";
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

	int addNewUserSessionToDB()
	{
		string nickName = getParameterByNumber(this->stringOfRequest, 2);
		string sessionString = getParameterByNumber(this->stringOfRequest, 3);
		string placeString = getParameterByNumber(this->stringOfRequest, 4);
		int numOfSession = std::stoi(sessionString);
		int place = std::stoi(placeString);
		this->result = this->cinemaDB->insertClient(nickName.c_str(), numOfSession, place);
		if (this->result < 0)
		{
			this->answer = "errorOfAddingToDB|\n";
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
	}

public:
	AdminThread(int clientSocket, const char* conectPath)
	{
		this->clientSocket = clientSocket;
		this->cinemaDB = new CinemaDB(conectPath);
	}

	~AdminThread()
	{
		delete this->cinemaDB;
		closesocket(this->clientSocket);
	}

	int workAdmin()
	{
		while (true)
		{
			memset(this->requestBuffer, 0, maxBufferSize + 1);
			this->requestLength = recv(this->clientSocket, this->requestBuffer, maxBufferSize, 0);

			//chek of reception error
			if (this->requestLength == SOCKET_ERROR)// ������ ��������� ������
			{
				cout << "recv error" << endl;
				return -1;
				break;
			}

			//chek of empty request
			if (this->requestLength == 0)
			{
				cout << "empty reqv" << endl;
				return -1;
				break;
			}

			this->requestBuffer[requestLength] = '\0';
			this->stringOfRequest = this->requestBuffer;

			cout << this->stringOfRequest << endl;

			if (stringOfRequest.find("getListOfUser") != std::string::npos)
			{
				this->result = this->sendListOfUsers();
				if (this->result < 0)
				{
					cout << "error of sending answer to admin" << endl;
				}
				else
				{
					cout << "list of user was send to admin" << endl;
				}
					
			}

			if (stringOfRequest.find("getListOfFilm") != std::string::npos)
			{
				this->result = this->sendListOfFilms();
				if (this->result < 0)
				{
					cout << "error of sending answer to admin" << endl;
				}
				else
				{
					cout << "list of film was send to admin" << endl;
				}
			}

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
						cout << "error of sending answer to admin" << endl;
					}
					else
					{
						cout << "error of adding new user to database" << endl;
					}
			}

			if (stringOfRequest.find("checkUserNickName") != std::string::npos)
			{
				this->result = this->checkUserNickName();
				if (this->result < 0)
				{
					cout << "error of sending answer to admin" << endl;
				}
				else
					if (this->result == 0)
					{
						cout << "nickName is free" << endl;
					}
					else
					{
						cout << "nickName is busy" << endl;
					}
			}

			if (stringOfRequest.find("addNewUserSession") != std::string::npos)
			{
				this->result = this->addNewUserSessionToDB();
				if (this->result == 0)
				{
					cout << "sucsess adding new user session to database" << endl;
				}
				else
					if (this->result == -1)
					{
						cout << "error of sending answer to admin" << endl;
					}
					else
					{
						cout << "error of adding new user session to database" << endl;
					}
			}

			if (stringOfRequest.find("addNewFilmToDataBase") != std::string::npos)
			{
				this->result = this->addNewFilmToDB();
				if (this->result == 0)
				{
					cout << "sucsess adding new film to database" << endl;
				}
				else
					if (this->result == -1)
					{
						cout << "error of sending answer to admin" << endl;
					}
					else
					{
						cout << "error of adding new film to database" << endl;
					}
			}

			if (stringOfRequest.find("addNewSessionToDataBase") != std::string::npos)
			{
				this->result = this->addNewSessionToDB();
				if (this->result == 0)
				{
					cout << "sucsess adding new session to database" << endl;
				}
				else
					if (this->result == -1)
					{
						cout << "error of sending answer to admin" << endl;
					}
					else
					{
						cout << "error of adding new session to database" << endl;
					}
			}

		}
	}
};
