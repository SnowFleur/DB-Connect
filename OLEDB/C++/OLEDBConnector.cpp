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
- MSSQL 연동 테스트
- Raw Query 테스트
- SP 테스트
*/

int main()
{

	try
	{
		HRESULT hr = CoInitialize(0);

		if (FAILED(hr)) 
		{
			std::cout << " COM 초기화 할 수 없습니다.\n";
			return -1;
		}

		//데이터베이스 연결
		CDBPropSet	dbinit(DBPROPSET_DBINIT);
		dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
		dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, OLESTR("PROVIDER=SQLOLEDB.1;SRVER아이피주소 어쩌구 패스워드 DB이름)"));

		dbinit.AddProperty(DBPROP_INIT_LCID,(LONG)1043);	//Local Identifer;

		hr = ds.Open(_T("SQLOLEDB"), &dbinit);

		if (FAILED(hr))
		{
			std::cout << "데이터 베이스를 열 수 없습니다.\n";
			return -1;
		}

		//세션을 시작
		hr = session.Open(ds);
		
		if (FAILED(hr)) 
		{
			std::cout << "SESSION을 시작할 수 없습니다.\n";
			ds.Close();
			return -1;
		}

		//사용할 쿼리문을 문자열로 만들어 둠
		char mySQL[] = "SELECT * FROM state";

		//쿼리문을 실행한다.
		hr = cust.Open(session, mySQL);

		if (FAILED(hr)) 
		{
			std::cout << "쿼리문을 실행한다.\n";
			session.Close();
			ds.Close();
			return -1;
		}

		//읽어온 데이터를 화면에 출력한다.
		while (cust.MoveNext()==S_OK)
		{
			std::cout << cust.ip << ", " << cust.data << " " << cust.time << "\n";
		}
		cust.Close();
		session.Close();
		ds.Close();

		return 1;

	}
	catch (...)
	{
		std::cout << "Error 발생\n";
		return -1;
	}
}


