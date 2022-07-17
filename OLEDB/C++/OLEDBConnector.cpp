#pragma once

#define _CRT_SECURE_NO_WARNINGS

/*
참고
- https://webnautes.tistory.com/702
- https://www.codeproject.com/Articles/10236/Database-Development-using-Visual-C-and-OLE-DB-Est
- https://stackoverflow.com/questions/8827297/unable-to-execute-stored-procedures-through-oledb
*/

#include<atldbcli.h>
#include<iostream>
#include <atlstr.h>


class CUserAcount {
	//DEFINE_COMMAND_EX(CUserAcount, _T("{call dbo.sp_AddUserAccount (?,?) }"))	//작동을 안하네 ㅡㅡ
public:
	WCHAR FLD_ID[10];
	WCHAR FLD_PW[10];
public:
	BEGIN_PARAM_MAP(CUserAcount)
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(1, FLD_ID)		// First ?
		SET_PARAM_TYPE(DBPARAMIO_INPUT)
		COLUMN_ENTRY(2, FLD_PW)		// Second ?
	END_PARAM_MAP()
};

class SP_AddUserAccount : public CCommand<CAccessor<CUserAcount>> {
public:
	void SetValue(const WCHAR* pID, const WCHAR* pPW)
	{
		if (pID == nullptr) return;
		if (pPW == nullptr) return;

		wmemcpy_s(FLD_ID, 10, pID, 10);
		wmemcpy_s(FLD_PW, 10, pPW, 10);
	}
};

class CUserAcount2 {	//Raw 쿼리
public:
	char FLD_ID[10];
	char FLD_PW[10];
public:
	BEGIN_COLUMN_MAP(CUserAcount2)
		COLUMN_ENTRY(1, FLD_ID)
		COLUMN_ENTRY(2, FLD_PW)
	END_COLUMN_MAP()

};


//OLEDB 객체 선언
CDataSource ds;
CSession	session;
HRESULT hr = CoInitialize(0);


/*
To do(순서)
- MSSQL 연동 테스트		--> OK
- Raw Query 테스트		--> OK
- SP 테스트				--> OK
*/


bool SQLServerConnect(const WCHAR* pDataBase, const WCHAR* pSQLServer, const WCHAR* pAuthUserID, const WCHAR* pAuthUserPW) {

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
	//hr = ds.Open(L"SQLOLEDB", &dbinit);
	hr = ds.Open(L"MSOLEDBSQL", &dbinit);


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

void SendSP(const WCHAR* pID, const WCHAR* pPW) {
		
	SP_AddUserAccount accountUserSP;

	accountUserSP.SetValue(pID, pPW);
	//주말 내내 ㅠ
	hr = accountUserSP.Open(session, _T("{call dbo.sp_AddUserAccount (?,?) }"));

	if (FAILED(hr)) {
		std::cout << "SP 실패\n";
	}

	else {
		std::cout << "SP 성공\n";
	}
	accountUserSP.ReleaseCommand();
	accountUserSP.Close();
}

void SendRawQuery() {

	TCHAR myQuery[] = L"SELECT * FROM dbo.UserAccount";
	CCommand<CAccessor<CUserAcount2>> cust;

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


