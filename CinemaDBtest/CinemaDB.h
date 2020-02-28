#pragma once

#include<iostream>
#include <typeinfo>
#include<sql.h>
#include<sqlext.h>
#include <oledb.h>  
#include <stdio.h>  
#include <conio.h>  
#include "icrsint.h"  
#include <string>
#include"Function.h"
#include<list>

using std::exception;


inline void TESTHR(HRESULT _hr)
{
	if FAILED(_hr)
	_com_issue_error(_hr);
}


class CinemaDB
{
private:
	ADODB::_ConnectionPtr pConnection = NULL;
	ADODB::_RecordsetPtr pRecordset = NULL;
	ADODB::_CommandPtr pComand = NULL;
	HRESULT hr;
public:
	CinemaDB(const char* conectPath)
	{
		CoInitialize(NULL);
		_bstr_t bstrConnect = conectPath;
		TESTHR(pConnection.CreateInstance(__uuidof(ADODB::Connection)));
		hr = pConnection->Open(bstrConnect, "", "", ADODB::adConnectUnspecified);
		if (SUCCEEDED(hr)) {
			cout << "Successfully connected to database." << endl;
		}
		else
		{
			cout << "Conection to data base error" << endl;
		}
	}

	~CinemaDB()
	{
		pConnection->Close();
	}

	string getListStringNameOfFilm()
	{
		string listOfFilms;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		_bstr_t query = "SELECT title FROM Films;";
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfFilms += temp + "|";
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfFilms += "\n";
		return listOfFilms;
	}

	string getListOfFilms()
	{
		string listOfFilms;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT * FROM Films;";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0" << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfFilms += temp + "/";
				}
				listOfFilms += "|";
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfFilms += "\0";
		return listOfFilms;
	}

	list<string> getListOfUsers()
	{
		list<string> listOfUsers;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		_bstr_t query = "SELECT Nickname FROM Users;";
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfUsers.push_back(temp);
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		return listOfUsers;
	}
	
	string getListOfSessionByFileTitle(const char* title)
	{
		string listOfSession;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT Num,Film,Date_of_session,Time_of_session FROM Sessions WHERE Film=\'" + string(title) + "\';";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfSession += temp + "/";
				}
				listOfSession += "|";
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfSession += "\n";
		return listOfSession;
	}

	int getHallByNumOfSession(int numOfSession)
	{
		string numOfHall;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT Hall FROM Sessions WHERE Num = " + std::to_string(numOfSession) + ";";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					numOfHall += temp;
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		numOfHall += "\0";
		return std::stoi(numOfHall);
		
	}

	int getHallSizeByNumOfSession(int numOfSession)  
	{
		string sizeOfHall;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT hallSize FROM Halls WHERE Num = (SELECT Hall FROM Sessions WHERE Num = " + std::to_string(numOfSession) + ");";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					sizeOfHall += temp;
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		sizeOfHall += "\0";
		return std::stoi(sizeOfHall);
	}

	list<int> getBusyPlacesByNumOfSession(int numOfSession)
	{
		list<int> listOfBusyPlace;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT Place FROM Clients WHERE numOfSession = " + std::to_string(numOfSession) + ";";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfBusyPlace.push_back(std::stoi(temp));
				}
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfBusyPlace.sort();
		return listOfBusyPlace;
	}

	string getSessionInformationByClient(const char* nickname)
	{
		string listOfSession;
		TESTHR(pRecordset.CreateInstance(__uuidof(ADODB::Recordset)));
		string request = "SELECT S.Film, S.Date_of_session, S.Time_of_session, S.Hall, C.Place FROM Sessions S, Clients C WHERE S.Num IN (SELECT numOfSession FROM Clients WHERE Nickname = \'" + string(nickname) + "\') and C.Nickname = \'" + string(nickname) + "\' and C.numOfSession = S.Num;";
		_bstr_t query = request.c_str();
		hr = pRecordset->Open(query, _variant_t((IDispatch *)pConnection, true), ADODB::adOpenUnspecified, ADODB::adLockUnspecified, ADODB::adCmdText);
		if (SUCCEEDED(hr))
		{
			ADODB::Fields* pFields = NULL;
			hr = pRecordset->get_Fields(&pFields);
			if (pFields && pFields->GetCount() <= 0)
			{
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}
			while (!pRecordset->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {

					string temp = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					listOfSession += temp + "/";
				}
				listOfSession += "|";
				pRecordset->MoveNext();
			}
		}
		else
		{
			cout << "error" << endl;
		}
		pRecordset->Close();
		listOfSession += "\n";
		return listOfSession;
	}

	int insertUser(const char* nickname, const char* lastname, const char* firstname)
	{
		try
		{
			hr = pComand.CreateInstance(__uuidof(ADODB::Command));
			pComand->ActiveConnection = pConnection;
			string temp = "INSERT INTO Users (Nickname,Last_name,First_name) VALUES (\'" + string(nickname) + "\',\'" + string(lastname) + "\',\'" + string(firstname) + "\');";
			_bstr_t query = temp.c_str();
			pComand->CommandText = query;
			pComand->Execute(NULL, NULL, ADODB::adCmdText);
			return 0;
		}
		catch (exception e)
		{
			return -1;
		}
	}

	int insertClient(const char* nickname, int numOfSession, int place)
	{
		try
		{
			hr = pComand.CreateInstance(__uuidof(ADODB::Command));
			pComand->ActiveConnection = pConnection;
			string temp = "INSERT INTO Clients (Nickname,numOfSession,Place) VALUES (\'" + string(nickname) + "\'," + std::to_string(numOfSession) + "," + std::to_string(place) + ");";
			_bstr_t query = temp.c_str();
			pComand->CommandText = query;
			pComand->Execute(NULL, NULL, ADODB::adCmdText);
			return 0;
		}
		catch (exception e)
		{
			return -1;
		}
	}

	int insertFilm(const char* title, const char* country, const char* date)
	{
		try
		{
			hr = pComand.CreateInstance(__uuidof(ADODB::Command));
			pComand->ActiveConnection = pConnection;
			string temp = "INSERT INTO Films (Title,Country,[Year]) VALUES (\'" + string(title) + "\',\'" + string(country) + "\',\'" + string(date) + "\');";
			_bstr_t query = temp.c_str();
			pComand->CommandText = query;
			pComand->Execute(NULL, NULL, ADODB::adCmdText);
			return 0;
		}
		catch (exception e)
		{
			cout << "error" << endl;
			return -1;
		}
	}
	
	int insertSession(const char* film, const char* date, const char* time, int hall)
	{
		try
		{
			hr = pComand.CreateInstance(__uuidof(ADODB::Command));
			pComand->ActiveConnection = pConnection;
			string temp = "INSERT INTO Sessions (Film,[Date_of_session],[Time_of_session],Hall) VALUES (\'" + string(film) + "\',\'" + string(date) + "\',\'" + string(time) + "\'," +std::to_string(hall)+");";
			_bstr_t query = temp.c_str();
			pComand->CommandText = query;
			pComand->Execute(NULL, NULL, ADODB::adCmdText);
			return 0;
		}
		catch (exception e)
		{
			return -1;
		}
	}

};


