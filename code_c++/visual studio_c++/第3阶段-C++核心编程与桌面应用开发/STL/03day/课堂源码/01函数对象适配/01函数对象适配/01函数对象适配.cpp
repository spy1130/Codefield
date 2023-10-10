#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<algorithm>
#include<functional>
#include<vector>
#include<string>


//��һ��:�̳�binary_function<����1������2����������>
struct Myfunc:public binary_function<int,int,void>
{
	void operator()(int v1,int v2)const //�ڶ���������const��Ϊ��������������2��
	{
		cout << "v1=" << v1 << endl;
		cout << "v2=" << v2 << endl;
		//cout << v1 << " " << endl;
		cout << v1 + v2 << endl;//��������ʵ�ֺ�����
	}

};
//���󣺴�ӡʱ��ÿ��Ԫ�ؼ�100Ȼ���ӡ����
void test()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	//for_each(v.begin(), v.end(), Myfunc());
	//���Ĳ�����bind2nd���󶨺�������
	for_each(v.begin(), v.end(), bind2nd(Myfunc(),100));
}
//2.bind1st��bind2nd������
//bind1st��100�󶨸���һ������
//bind2nd��100�󶨸��ڶ�������

//��һ�����̳�
struct MyNotfunc:public unary_function<int,bool>
{
	bool operator()(int v)const //�ڶ�������Ϊ������
	{
		return v >= 20;
	}
	 
};
//not1 ��not2 
void myPrint(int val)
{
	cout << val << " ";
}

void test02()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(50);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	//find_if()

	/*
	_InIt _Find_if(_InIt _First, _InIt _Last, _Pr _Pred)
	{	// find first satisfying _Pred
		for (; _First != _Last; ++_First)
		if (_Pred(*_First))
			break;
		return (_First);
	}
	*/
	//������������
	vector<int>::iterator it=find_if(v.begin(), v.end(), not1(MyNotfunc()));
	if (it == v.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�=" << *it << endl;
	}

	//not2��ʹ��
	//less ��Ԫ��������
	vector<int> v2;
	v2.push_back(10);
	v2.push_back(50);
	v2.push_back(30);
	v2.push_back(40);
	v2.push_back(50);
	//releaseģʽ�¿���
	sort(v2.begin(), v2.end(),not2(less<int>()));
	for_each(v2.begin(), v2.end(), myPrint);
}

//��ͨ������������ ptr_fun
//��һ��:��һ��������Ϊ��������
void myprint2(int val,int val2)
{
	cout << val+val2<< " ";
}
void test03()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	//�ڶ���������ͨ������Ϊ�������� ptr_fun
	for_each(v.begin(), v.end(), bind2nd(ptr_fun(myprint2),100));
}

class Maker
{
public:
	Maker(string name,int age)
	{
		this->name = name;
		this->age = age;
	}
	//��Ա����
	void MyprintMaker()
	{
		cout << "Name:" << this->name << " Age:" << this->age << endl;
	}
public:
	string name;
	int age;
};
//void MyprintMaker(Maker m)
//{
//	cout << "Name:" << m.name << " Age:" << m.age << endl;
//}
void test04()
{
	vector<Maker> v;
	v.push_back(Maker("aaa", 10));
	v.push_back(Maker("bbb", 20));
	v.push_back(Maker("ccc", 30));
	//�������洢���Ƕ�����mem_fun_ref�������ĳ�Ա����
	for_each(v.begin(), v.end(), mem_fun_ref(&Maker::MyprintMaker));
	cout << "-------------" << endl;
	vector<Maker*> vp;
	vp.push_back(new Maker("aaa", 10));
	vp.push_back(new Maker("bbb", 20));
	vp.push_back(new Maker("ccc", 30));
	//�������洢���Ƕ���ָ�룬��mem_fun�������ĳ�Ա����
	for_each(vp.begin(), vp.end(), mem_fun(&Maker::MyprintMaker));
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

