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
- MSSQL ���� �׽�Ʈ
- Raw Query �׽�Ʈ
- SP �׽�Ʈ
*/

int main()
{

	try
	{
		HRESULT hr = CoInitialize(0);

		if (FAILED(hr)) 
		{
			std::cout << " COM �ʱ�ȭ �� �� �����ϴ�.\n";
			return -1;
		}

		//�����ͺ��̽� ����
		CDBPropSet	dbinit(DBPROPSET_DBINIT);
		dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
		dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, OLESTR("PROVIDER=SQLOLEDB.1;SRVER�������ּ� ��¼�� �н����� DB�̸�)"));

		dbinit.AddProperty(DBPROP_INIT_LCID,(LONG)1043);	//Local Identifer;

		hr = ds.Open(_T("SQLOLEDB"), &dbinit);

		if (FAILED(hr))
		{
			std::cout << "������ ���̽��� �� �� �����ϴ�.\n";
			return -1;
		}

		//������ ����
		hr = session.Open(ds);
		
		if (FAILED(hr)) 
		{
			std::cout << "SESSION�� ������ �� �����ϴ�.\n";
			ds.Close();
			return -1;
		}

		//����� �������� ���ڿ��� ����� ��
		char mySQL[] = "SELECT * FROM state";

		//�������� �����Ѵ�.
		hr = cust.Open(session, mySQL);

		if (FAILED(hr)) 
		{
			std::cout << "�������� �����Ѵ�.\n";
			session.Close();
			ds.Close();
			return -1;
		}

		//�о�� �����͸� ȭ�鿡 ����Ѵ�.
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
		std::cout << "Error �߻�\n";
		return -1;
	}
}


