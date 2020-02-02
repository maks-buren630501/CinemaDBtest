#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>
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

using namespace std;

int main()
{
	//CinemaDB *cinemaDB = new CinemaDB("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;");
	Server* server = new Server("127.0.0.1","8080", "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;");

	system("pause");
	


}