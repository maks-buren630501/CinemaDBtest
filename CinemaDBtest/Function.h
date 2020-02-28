#include"comutil.h"
#include <oledb.h>  
#include<iostream>
#include<list>
#include<string>

using std::cout;
using std::endl;
using std::string;
using std::list;

//need to add
class Film
{
private:
	string title;
	string country;
	string date;
public:
	Film(const char* title, const char* country, const char* date)
	{
		this->title = title;
		this->country = country;
		this->date = date;
	}

	Film(Film &film)
	{

	}

	string getTitle()
	{
		return title;
	}

	string getCountry()
	{
		return country;
	}

	string getDate()
	{
		return date;
	}
};


string bstr_to_str(BSTR source);

int checkUser(const char* nickname,list<string> listOfUsers);

list<int> getFreePlacesList(list<int> listOfBusyPlaces, int sizeOfHall);

string getStringOfFreePlaces(list<int> freePlaces);

string getNameFromRequest(string request);

string getFirstParametrFromRequest(string request);

string getSecondParametrFromRequest(string request);

string getParameterByNumber(string request,int number);

string getStringListOfUsers(list<string> listOfUsers);

list<string> getListOfParams(string answer);

Film getFilmFromString(string answer);