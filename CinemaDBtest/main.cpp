#import <C:\\Program Files\\Common Files\\system\\ado\\msado15.dll> rename( "EOF", "AdoNSEOF" )

#include<iostream>
#include <typeinfo>
#include<sql.h>
#include<sqlext.h>
#include <oledb.h>  
#include <stdio.h>  
#include <conio.h>  
#include "icrsint.h"  
#include <string>
#include"CinemaDB.h"
#include<list>

using namespace std;

int main()
{
	CinemaDB *cinemaDB = new CinemaDB("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;");
	//cout << cinemaDB->getListStringNameOfFilm() << endl;
	//cinemaDB->InsertUser("Clotch", "Sergeev", "Artem");
	list<string> listOfUsers= cinemaDB->getListOfUsers();
	/*for(string var : listOfUsers)
	{
		cout << var << endl;
	}
	int a = checkUser("Zero", listOfUsers);
	cout << a << endl;*/
	//cout << cinemaDB->getListOfSessionByFileTitle("Staliningrad");
	cout << cinemaDB->getHallSizeByNumOfHall(cinemaDB->getHallByNumOfSession(2))<<endl;
	list<int> list =  cinemaDB->getBusyPlacesByNumOfSession(2);

	for (int a : list)
	{
		cout << a << endl;
	}

	system("pause");


}