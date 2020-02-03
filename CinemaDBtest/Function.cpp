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
		for (int j : listOfBusyPlaces)
		{
			if (i == j)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			flag = 0;

			continue;
		}
		allPlacesList.push_back(i);
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
	Name += '\0';
	return Name;
}

string getFirstParametrFromRequest(string request)
{
	string FirstParametr;
	int beginOfFirstParametr = request.find("|") + 1;
	string parametrs= request.substr(beginOfFirstParametr, request.size() - beginOfFirstParametr);
	int endOfFirstParametr = parametrs.find("|");
	FirstParametr = parametrs.substr(0, endOfFirstParametr);
	FirstParametr += '\0';
	return FirstParametr;
}

string getSecondParametrFromRequest(string request)
{
	string secondParametr;
	int beginOfFirstParametr = request.find("|") + 1;
	string parametrs = request.substr(beginOfFirstParametr, request.size() - beginOfFirstParametr);
	int beginOfLastParametr = parametrs.find("|") + 1;
	secondParametr = parametrs.substr(beginOfLastParametr, parametrs.size() - beginOfLastParametr - 1);
	secondParametr += '\0';
	return secondParametr;
}