#pragma once

/*
참고
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

//OLEDB 객체 선언
CDataSource ds;
CSession	session;
CCommand<CAccessor<CUserAcount>> cust;

/*
To do(순서)
- MSSQL 연동 테스트		--> 0k
- Raw Query 테스트		--> Ok
- SP 테스트				--> Curr
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


	//SQL Server 연결시도
	hr = ds.Open(L"SQLOLEDB", &dbinit);

	if (FAILED(hr)) {
		std::cout << "SQL Server와 연결을 실패했습니다\n";
		return false;
	}

	//세션을 시작
	hr = session.Open(ds);

	if (FAILED(hr)) {
		std::cout << "session.Open 에러\n";
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
		std::cout << " 테이블을 열 수 없습니다.\n";

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


