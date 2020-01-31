#include"comutil.h"
#include <oledb.h>  
#include<iostream>
#include<list>
#include<string>


using namespace std;

string bstr_to_str(BSTR source);

int checkUser(const char* nickname,list<string> listOfUsers);

list<int> getFreePlacesList(list<int> listOfBusyPlaces, int sizeOfHall);

string getStringOfFreePlaces(list<int> freePlaces);

string getNickNameFromRequest(string request);

string getLastNameFromRequest(string request);

string getFirstNameFromRequest(string request);

