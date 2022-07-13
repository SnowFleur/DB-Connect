#pragma once

/*
����
https://webnautes.tistory.com/702
*/

#include<atldbcli.h>
#include<iostream>

class CState
{
private:
public:
	//�����͸� ������ �ڷ���
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

//OLEDB ��ü ����
CDataSource ds;
CSession	session;
CCommand<CAccessor<CState>> cust;

/*
To do(����)
- MSSQL ���� �׽�Ʈ		--> OK
- Raw Query �׽�Ʈ		--> Curr
- SP �׽�Ʈ
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


	//SQL Server ����õ�
	hr = ds.Open(L"SQLOLEDB", &dbinit);

	if (FAILED(hr))
	{
		std::cout << "SQL Server�� ������ �����߽��ϴ�\n";
		return false;
	}

	//������ ����
	hr = session.Open(ds);

	if (FAILED(hr)) 
	{
		std::cout << "session.Open ����\n";
		return false;
	}


	std::cout << "SQL Server Connect!\n";
	return true;
}

int main()
{

}


