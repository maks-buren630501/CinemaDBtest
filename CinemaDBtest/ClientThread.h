#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <chrono>
#include<fstream>
#include"Function.h"

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
		string nickName = getNameFromRequest(this->stringOfRequest);
		if (std::find(listOfUsers.begin(), listOfUsers.end(), nickName) == listOfUsers.end())
		{
			this->answer = "nickNameIsFree|\n";
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
			this->answer = "nickNameIsBusy|\n";
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
		string lastName = getFirstParametrFromRequest(this->stringOfRequest);
		string firstName = getSecondParametrFromRequest(this->stringOfRequest);
		this->result = this->cinemaDB->insertUser(this->nickName.c_str(),lastName.c_str(),firstName.c_str());
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
		else 
		{
			this->answer = "successfullyAddingToDB|\n";
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

	//func to send list of session by film title
	int sendListOfSessionByFilmTittle()
	{
		string filmTitle = getNameFromRequest(this->stringOfRequest);
		string listOfSession = this->cinemaDB->getListOfSessionByFileTitle(filmTitle.c_str());
		this->result = send(clientSocket, listOfSession.c_str(), listOfSession.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	//func to send hall of session by session num
	int sendHallByNumOfSession()
	{
		int numOfSession = std::stoi(getNameFromRequest(this->stringOfRequest));
		int numOfHall = this->cinemaDB->getHallByNumOfSession(numOfSession);
		string hall = std::to_string(numOfHall);
		hall += "|\n";
		this->result = send(clientSocket, hall.c_str(), hall.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	//func to send free places by session num
	int sendFreePlacesByNumOfSession()
	{
		int numOfSession = std::stoi(getNameFromRequest(this->stringOfRequest));
		int sizeOfHall = this->cinemaDB->getHallSizeByNumOfSession(numOfSession);
		list<int> busyPlaces = this->cinemaDB->getBusyPlacesByNumOfSession(numOfSession);
		list<int> freePlacesList = getFreePlacesList(busyPlaces, sizeOfHall);
		string freePlacesString = getStringOfFreePlaces(freePlacesList);
		this->result = send(clientSocket, freePlacesString.c_str(), freePlacesString.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	//func to add new user session to data base
	int addNewUserSessionToDB()
	{
		string hallString = getFirstParametrFromRequest(this->stringOfRequest);
		string placeString = getSecondParametrFromRequest(this->stringOfRequest);
		int numOfHall = std::stoi(hallString);
		int place = std::stoi(placeString);
		this->result = this->cinemaDB->insertClient(this->nickName.c_str(),numOfHall,place);
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
		else
		{
			this->answer = "successfullyAddingToDB|\n";
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

	//func to send information about all client sessions
	int sendSessionsInformation()
	{
		string sessionsInformation = this->cinemaDB->getSessionInformationByClient(this->nickName.c_str());
		this->result = send(clientSocket, sessionsInformation.c_str(), sessionsInformation.length(), 0);
		if (this->result < 0)
		{
			return -1;
		}
		else
		{
			return 0;
		}
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
			if(stringOfRequest.find("addNewUserToDB") != std::string::npos)
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
			if(stringOfRequest.find("getListOfFilm") != std::string::npos)
			{
				this->result = this->sendListOfFilms();
				if (this->result < 0)
				{
					cout << "error of sending answer to user" << endl;
				}
				else
				{
					cout << "list of film was send to user" << endl;
				}

			}

			//request to send list of session by film title
			if(stringOfRequest.find("getSessionByFilmTittle") != std::string::npos)
			{
				this->result = this->sendListOfSessionByFilmTittle();
				if (this->result < 0)
				{
					cout << "error of sending answer to user" << endl;
				}
				else
				{
					cout << "list of session was send to user" << endl;
				}
			}

			//requset to send num of hall of session by num of session
			if(stringOfRequest.find("getHallByNumOfSession") != std::string::npos)
			{
				this->result = this->sendHallByNumOfSession();
				if (this->result < 0)
				{
					cout << "error of sending answer to user" << endl;
				}
				else
				{
					cout << "num of hall was send to user" << endl;
				}
			}

			//requset to free places by num of session
			if(stringOfRequest.find("getFreePlacesByNumOfSession") != std::string::npos)
			{
				this->result = this->sendFreePlacesByNumOfSession();
				if (this->result < 0)
				{
					cout << "error of sending answer to user" << endl;
				}
				else
				{
					cout << "list of free places was send to user" << endl;
				}
			}

			//request to add new user session to data base
			if (stringOfRequest.find("addUserSessionToDB") != std::string::npos)
			{
				this->result = this->addNewUserSessionToDB();
				if (this->result == 0)
				{
					cout << "sucsess adding new user session to database" << endl;
				}
				else
					if (this->result == -1)
					{
						cout << "error of sending answer to client" << endl;
					}
					else
					{
						cout << "error of adding new user session to database" << endl;
					}
			}

			//request to get information about all user session by nickname
			if (stringOfRequest.find("getMySessionsInformation") != std::string::npos)
			{
				this->result = this->sendSessionsInformation();
				if (this->result < 0)
				{
					cout << "error of sending answer to user" << endl;
				}
				else
				{
					cout << "sessions information was send to user" << endl;
				}
			}
		}
	}

	
};