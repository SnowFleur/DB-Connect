#pragma once

/*
참고
https://webnautes.tistory.com/702
*/

#include<atldbcli.h>
#include<iostream>

class CState
{
private:
public:
	//데이터를 저장할 자료형
	char ip[20];
	char data[20];
	char time[20];

public:
	BEGIN_COLUMN_MAP(CState)
		COLUMN_ENTRY(1,ip)
		COLUMN_ENTRY(1,data)
		COLUMN_ENTRY(1,time)
	END_COLUMN_MAP()

};

//OLEDB 객체 선언
CDataSource ds;
CSession	session;
CCommand<CAccessor<CState>> cust;

/*
To do(순서)
- MSSQL 연동 테스트		--> OK
- Raw Query 테스트		--> Curr
- SP 테스트
*/

namespace SnowOELDB{}

namespace SnowRedis {}



bool SQLServerConnect( const WCHAR* pDataBase, const WCHAR* pSQLServer, const WCHAR* pAuthUserID, const WCHAR* pAuthUserPW )
{
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

	if (FAILED(hr))
	{
		std::cout << "SQL Server와 연결을 실패했습니다\n";
		return false;
	}

	//세션을 시작
	hr = session.Open(ds);

	if (FAILED(hr)) 
	{
		std::cout << "session.Open 에러\n";
		return false;
	}


	std::cout << "SQL Server Connect!\n";
	return true;
}

int main()
{

}


