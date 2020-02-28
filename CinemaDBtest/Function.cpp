#include"Function.h"


string bstr_to_str(BSTR source)
{
	_bstr_t wrapped_bstr = _bstr_t(source);
	int length = wrapped_bstr.length();
	char* char_array = new char[length];
	strcpy_s(char_array, length + 1, wrapped_bstr);
	return char_array;
}

int checkUser(const char* nickname, list<string> listOfUsers)
{

	std::list<string>::iterator iterator = std::find(listOfUsers.begin(), listOfUsers.end(), string(nickname));
	if (iterator == listOfUsers.end())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

list<int> getFreePlacesList(list<int> listOfBusyPlaces, int sizeOfHall)
{
	list<int> allPlacesList;
	int flag = 0;
	for (int i = 1; i < sizeOfHall + 1; i++)
	{
		if (std::find(listOfBusyPlaces.begin(), listOfBusyPlaces.end(), i) != listOfBusyPlaces.end())
		{
			continue;
		}
		else
		{
			allPlacesList.push_back(i);
		}
	}
	return allPlacesList;
    
}

string getStringOfFreePlaces(list<int> freePlaces)
{
	string freePlaceString;
	for (int i : freePlaces)
	{
		freePlaceString += std::to_string(i) + "|";
	}
	freePlaceString += "\n";
	return freePlaceString;
}

string getNameFromRequest(string request)
{
	string Name;
	int beginOfName = request.find("|") + 1;
	Name = request.substr(beginOfName,request.size()-beginOfName-1);
	return Name;
}

string getFirstParametrFromRequest(string request)
{
	string FirstParametr;
	int beginOfFirstParametr = request.find("|") + 1;
	string parametrs= request.substr(beginOfFirstParametr, request.size() - beginOfFirstParametr);
	int endOfFirstParametr = parametrs.find("|");
	FirstParametr = parametrs.substr(0, endOfFirstParametr);
	return FirstParametr;
}

string getSecondParametrFromRequest(string request)
{
	string secondParametr;
	int beginOfFirstParametr = request.find("|") + 1;
	string parametrs = request.substr(beginOfFirstParametr, request.size() - beginOfFirstParametr);
	int beginOfLastParametr = parametrs.find("|") + 1;
	secondParametr = parametrs.substr(beginOfLastParametr, parametrs.size() - beginOfLastParametr - 1);
	return secondParametr;
}

string getParameterByNumber(string request, int number)
{
	while (true)
	{
		number--;
		int endOfFirstParam = request.find("|");
		if (number == 0)
		{
			return request.substr(0, endOfFirstParam);
		}
		else
		{
			return getParameterByNumber(request.substr(endOfFirstParam + 1,request.size() - endOfFirstParam),number);
		}
	}
}

string getStringListOfUsers(list<string> listOfUsers)
{
	string stringListOfUsers = "";
	for (string user : listOfUsers)
	{
		stringListOfUsers += user + "|";
	}
	return stringListOfUsers;
}

list<string> getListOfParams(string answer)
{

	string temp;
	list<string> listOfParams;
	for (int i = 0; i < answer.size(); i++)
	{
		if (answer[i] == '|')
		{
			listOfParams.push_back(temp);
			temp = "";
			continue;
		}
		temp += answer[i];
	}
	return listOfParams;
}

Film getFilmFromString(string answer)
{
	int fl = 0;
	string temp;
	string title;
	string country;
	string date;

	for (int i = 0; i < answer.size(); i++)
	{
		if (answer[i] == '/')
		{
			switch (fl)
			{
			case 0:
			{
				title = temp.c_str();
				temp = "";
				fl++;
				i++;
				break;
			}
			case 1:
			{
				country = temp.c_str();
				temp = "";
				fl++;
				i++;
				break;
			}
			case 2:
			{
				date = temp.c_str();
				temp = "";
				fl++;
				continue;
			}
			}
		}
		temp += answer[i];
		Film film = Film(title.c_str(), country.c_str(), date.c_str());
		return film;
	}
}