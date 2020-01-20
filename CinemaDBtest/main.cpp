#import <C:\\Program Files\\Common Files\\system\\ado\\msado15.dll> rename( "EOF", "AdoNSEOF" )

#include<iostream>
#include<sql.h>
#include<sqlext.h>
#include <oledb.h>  
#include <stdio.h>  
#include <conio.h>  
#include "icrsint.h"  


using namespace std;

inline void TESTHR(HRESULT _hr) {
	if FAILED(_hr)
		_com_issue_error(_hr);
}

int main() 
{
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
			cout << ": Retrieve schema info for the given result set: " << endl;
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

			cout << ": Fetch the actual data: " << endl;
			int rowCount = 0;
			while (!pRS->AdoNSEOF) {
				for (long nIndex = 0; nIndex < pFields->GetCount(); nIndex++) {
					cout << " | " << _bstr_t(pFields->GetItem(nIndex)->GetValue());
				}
				cout << endl;
				pRS->MoveNext();
				rowCount++;
			}
			cout << ": Total Row Count: " << rowCount << endl;
		}

		pRS->Close();
		pCn->Close();
	
	system("pause");
	CoUninitialize();
}