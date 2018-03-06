// DelegateRealize.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<string>
#include "IDelegate.h"
using namespace std;

void Say()
{
	cout << "��ͨί�гɹ�" << endl;
}

class A
{
public:
	static A* getInstance(){
		if (m_instance==nullptr)
		{
			m_instance = new A();
		}
			return m_instance;
	}
	~A(){};
	void Say()
	{
		cout << "���Ա����ί�гɹ�" <<m_str<< endl;
		
	}
	void setStr(string str)
	{
		m_str = str;
	}
	A() :m_str("11"){ };
private:
	
	static A* m_instance;
	std::string m_str;
};

A* A::m_instance = nullptr;

class B
{
public:
	B(){
		event += newDelegate(Say);
		event += newDelegate(&A(), &A::Say);
		cout << &A() << endl;
		cout << A::getInstance() << endl;
		A().setStr("222");
		A().Say();
		A::getInstance()->Say();

		//event += newDelegate(A::getInstance(),&A::Say);
		
	};
	~B(){};
	void func(){
		cout <<"��ִ��B���¼�" << endl;
		cout << "��ʼִ��ί���¼�" << endl;
		event();
	}
private:
	CMultiDelegate event;
};


int _tmain(int argc, _TCHAR* argv[])
{
	B b;
	b.func();
	getchar();
	return 0;
}

