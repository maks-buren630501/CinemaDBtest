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

using namespace std;

inline void TESTHR(HRESULT _hr) {
	if FAILED(_hr)
		_com_issue_error(_hr);
}


string bstr_to_str(BSTR source) {
	//source = L"lol2inside";
	_bstr_t wrapped_bstr = _bstr_t(source);
	int length = wrapped_bstr.length();
	char* char_array = new char[length];
	strcpy_s(char_array, length + 1, wrapped_bstr);
	return char_array;
}

class CinemaDB
{
private:
	ADODB::_ConnectionPtr pConnection = NULL;
	ADODB::_RecordsetPtr pRecordset = NULL;
	ADODB::_CommandPtr pComand = NULL;
	_bstr_t bstrConnect;
	HRESULT hr;
public:
	CinemaDB(const char* conectPath)
	{
		CoInitialize(NULL);
		_bstr_t bstrConnect = conectPath;
		TESTHR(pConnection.CreateInstance(__uuidof(ADODB::Connection)));
		hr = pConnection->Open(bstrConnect, "", "", ADODB::adConnectUnspecified);
		if (SUCCEEDED(hr)) {
			cout << ": Successfully connected to database." << endl;
		}
		else
		{
			cout << ": Conection to data base error" << endl;
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
		return listOfFilms;
	}

	int InsertUser(const char* nickname, const char* lastname, const char* firstname)
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
		catch(exception e)
		{
			return -11;
		}
	}

};


int main() 
{
	CinemaDB *cinemaDB = new CinemaDB("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;");
	cout << cinemaDB->getListStringNameOfFilm() << endl;
	cinemaDB->InsertUser("Clotch","Sergeev","Artem");
	system("pause");

	/*
	_bstr_t bstrConnect = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=F:\\AccesDB\\CinemaDB.accdb;";
	CoInitialize(NULL);
	
		ADODB::_ConnectionPtr pCn("ADODB.Connection");
		_variant_t vtTableName("authors"), vtCriteria;
		long ix[1];
		SAFEARRAY *pSa = NULL;

		pCn->Provider = "sqloledb";
		HRESULT hr = pCn->Open(bstrConnect, "", "", ADODB::adConnectUnspecified);
		if (SUCCEEDED(hr)) {
			cout << ": Successfully connected to database. Data source name:\n  " << pCn->GetConnectionString() << endl;
		}

		_bstr_t query = "SELECT * FROM Films;";
		cout << ": SQL query:\n  " << query << endl;

		ADODB::_RecordsetPtr pRS("ADODB.Recordset");
		hr = pRS->Open(query,
			_variant_t((IDispatch *)pCn, true),
			ADODB::adOpenUnspecified,
			ADODB::adLockUnspecified,
			ADODB::adCmdText);
		if (SUCCEEDED(hr)) {
			//cout << ": Retrieve schema info for the given result set: " << endl;
			ADODB::Fields* pFields = NULL;
			hr = pRS->get_Fields(&pFields);
			if (SUCCEEDED(hr) && pFields && pFields->GetCount() > 0) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {
					cout << " | " << _bstr_t(pFields->GetItem(nIndex)->GetName());
				}
				cout << endl;
			}
			else {
				cout << ": Error: Number of fields in the result set is 0." << endl;
			}

			//cout << ": Fetch the actual data: " << endl;
			int rowCount = 0;
			while (!pRS->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {
					//cout << " | " << _bstr_t(pFields->GetItem(nIndex)->GetValue());
					string s = bstr_to_str(_bstr_t(pFields->GetItem(nIndex)->GetValue()));
					//cout << ws<<" ";
					cout << s << endl;
				}
				cout << endl;
				pRS->MoveNext();
				rowCount++;
			}
			//cout << ": Total Row Count: " << rowCount << endl;
		}

		pRS->Close();
		pCn->Close();
	
	system("pause");
	CoUninitialize();
	*/

}


