#pragma once

/*
����
- https://webnautes.tistory.com/702
- https://www.codeproject.com/Articles/10236/Database-Development-using-Visual-C-and-OLE-DB-Est
*/

#include<atldbcli.h>
#include<iostream>

class CUserAcount {
public:
	char FLD_ID[10];
	char FLD_PW[10];
public:
	BEGIN_COLUMN_MAP(CUserAcount)
		COLUMN_ENTRY(1, FLD_ID)
		COLUMN_ENTRY(1, FLD_PW)
	END_COLUMN_MAP()
};

//OLEDB ��ü ����
CDataSource ds;
CSession	session;
CCommand<CAccessor<CUserAcount>> cust;

/*
To do(����)
- MSSQL ���� �׽�Ʈ		--> 0k
- Raw Query �׽�Ʈ		--> Ok
- SP �׽�Ʈ				--> Curr
*/

namespace SnowOELDB {}

namespace SnowRedis {}



bool SQLServerConnect(const WCHAR* pDataBase, const WCHAR* pSQLServer, const WCHAR* pAuthUserID, const WCHAR* pAuthUserPW) {
	HRESULT hr = CoInitialize(0);

	WCHAR arrSqlCoonect[1024];
	ZeroMemory(arrSqlCoonect, sizeof(arrSqlCoonect));

	CDBPropSet	dbinit(DBPROPSET_DBINIT);

	//SQL Server
	if (dbinit.AddProperty(DBPROP_INIT_DATASOURCE, pSQLServer) == false) return false;

	//DB
	if (dbinit.AddProperty(DBPROP_INIT_CATALOG, pDataBase) == false) return false;

	//AuthUSerID
	if (dbinit.AddProperty(DBPROP_AUTH_USERID, pAuthUserID) == false) return false;

	//AuthUserPW
	if (dbinit.AddProperty(DBPROP_AUTH_PASSWORD, pAuthUserPW) == false) return false;


	//SQL Server ����õ�
	hr = ds.Open(L"SQLOLEDB", &dbinit);

	if (FAILED(hr)) {
		std::cout << "SQL Server�� ������ �����߽��ϴ�\n";
		return false;
	}

	//������ ����
	hr = session.Open(ds);

	if (FAILED(hr)) {
		std::cout << "session.Open ����\n";
		return false;
	}

	std::cout << "SQL Server Connect!\n";
	return true;
}


void SendRawQuery() {

	TCHAR myQuery[] = L"SELECT * FROM DBO.UserAccount";
	HRESULT hr = CoInitialize(0);

	//Open DB
	hr = cust.Open(session, myQuery);

	if (FAILED(hr)) {
		std::cout << " ���̺��� �� �� �����ϴ�.\n";

	}
	else {

		//read all data
		while (cust.MoveNext() == S_OK) {
			std::cout << "ID:" << cust.FLD_ID << " ," << "PW:" << cust.FLD_PW << "\n";
		}
	}

	cust.Close();
	session.Close();
	ds.Close();

}

int main() {

}


