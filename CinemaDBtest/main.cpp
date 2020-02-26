#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
#include<winsock.h>
#pragma comment(lib, "Ws2_32.lib")

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
#include"Server.h"
#include<list>

using std::cout;
using std::endl;
using std::string;
using std::exception;


int main()
{
	//Server* server = new Server("192.168.43.10","8080", "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;",0);
	//server->setServerToListen();
	CinemaDB *cinemaDB = new CinemaDB("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;");
	cinemaDB->insertFilm("Old man", "USA", "12.12.2012");
	

	system("pause");
	


}