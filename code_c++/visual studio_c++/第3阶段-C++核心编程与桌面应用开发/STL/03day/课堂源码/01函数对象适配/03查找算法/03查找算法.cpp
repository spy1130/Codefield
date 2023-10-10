#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
#include<string>
using namespace std;

bool Myprint(int v)
{
	return v > 30;
}
void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	/*
	_InIt _Find(_InIt _First, _InIt _Last, const _Ty& _Val, false_type)
	{	
		for (; _First != _Last; ++_First)
			if (*_First == _Val)
				break;
		return (_First);
	}
	*/
	vector<int>::iterator it=find(v.begin(), v.end(), 20);
	if (it == v.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�=" << *it << endl;
	}
	
	/*
	_InIt _Find_if(_InIt _First, _InIt _Last, _Pr _Pred)
	{	// find first satisfying _Pred
		for (; _First != _Last; ++_First)
		if (_Pred(*_First))
		break;
		return (_First);
	}
	*/
	it=find_if(v.begin(), v.end(), Myprint);
	if (it == v.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�=" << *it << endl;
	}
}
//���Ҷ���
class Maker
{
public:
	Maker(string name, int age)
	{
		this->name = name;
		this->age = age;
	}

	//����==
	bool operator==(const Maker& m)
	{
		return this->name == m.name && this->age == m.age;
	}
	
public:
	string name;
	int age;
};

struct MyFind:public binary_function<Maker,Maker,bool>
{
	bool operator()(Maker m,Maker m2)const
	{
		return m.name == m2.name && m.age == m2.age;
	}
};

void test02()
{
	vector<Maker> v;
	v.push_back(Maker("aaa1", 18));
	v.push_back(Maker("aaa2", 20));
	v.push_back(Maker("aaa3", 21));
	v.push_back(Maker("aaa4", 22));
	v.push_back(Maker("aaa5", 23));

	vector<Maker>::iterator it = find(v.begin(), v.end(), Maker("aaa2", 20));
	if (it == v.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�=" << it->name<<" "<<it->age<< endl;
	}

	it = find_if(v.begin(), v.end(), bind2nd(MyFind(),Maker("aaa3",21)));
	if (it == v.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�=" << it->name << " " << it->age << endl;
	}
}

/*
adjacent_find�㷨 ���������ظ�Ԫ��
@param beg ������ʼ������
@param end ��������������
@param  _callback �ص���������ν��(����bool���͵ĺ�������)
@return ��������Ԫ�صĵ�һ��λ�õĵ�����

adjacent_find(iterator beg, iterator end, _callback);
*/

class Maker2
{
public:
	Maker2(string name, int age)
	{
		this->name = name;
		this->age = age;
	}

	////����==
	//bool operator==(const Maker& m)
	//{
	//	return this->name == m.name && this->age == m.age;
	//}

public:
	string name;
	int age;
};

bool myadjacent_find(Maker2 &m1,Maker2 &m2)
{
	return m1.name == m2.name &&m1.age == m2.age;
}
void test03()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(40);
	v.push_back(40);
	v.push_back(40);
	v.push_back(50);
	v.push_back(60);
	/*
	_FwdIt adjacent_find(_FwdIt _First, _FwdIt _Last)
	{	// find first matching successor
	return (_STD adjacent_find(_First, _Last, equal_to<>()));
	}

	*/
	vector<int>::iterator it=adjacent_find(v.begin(), v.end());
	if (it == v.end())
	{
		cout << "�������ڵ��ظ�Ԫ��ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�:" << *it << endl;
	}



	/*
	_FwdIt _Adjacent_find(_FwdIt _First, _FwdIt _Last, _Pr _Pred)
	{
	if (_First != _Last)
	for (_FwdIt _Firstb; (_Firstb = _First), ++_First != _Last; )
	if (_Pred(*_Firstb, *_First))
	return (_Firstb);
	return (_Last);
	}
	*/

	vector<Maker2> v2;
	v2.push_back(Maker2("aaa1", 10));
	v2.push_back(Maker2("aaa4", 40));
	v2.push_back(Maker2("aaa4", 30));
	v2.push_back(Maker2("aaa4", 40));
	v2.push_back(Maker2("aaa5", 50));
	vector<Maker2>::iterator it2 = adjacent_find(v2.begin(), v2.end(), myadjacent_find);
	if (it2 == v2.end())
	{
		cout << "�������ڵ��ظ�Ԫ��ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ�:" << it2->name<<" "<<it2->age << endl;
	}




}

/*
binary_search�㷨 ���ֲ��ҷ�
ע��: �����������в�����
@param beg ������ʼ������
@param end ��������������
@param value ���ҵ�Ԫ��
@return bool ���ҷ���true ����false

bool binary_search(iterator beg, iterator end, value);
*/

class Student
{
public:
	Student(string name, int age)
	{
		this->name = name;
		this->age = age;
	}

	bool operator>(const Student &stu)const
	{
		return this->age >stu.age;
	}
	//less
	bool operator<(const Student &stu)const
	{
		return this->age <stu.age;
	}
public:
	string name;
	int age;
};
void test04()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);
	v.push_back(60);

	bool flg = binary_search(v.begin(), v.end(), 30);
	if (flg)
	{
		cout << "�ҵ�" << endl;
	}
	else
	{
		cout << "û���ҵ�" << endl;
	}

	vector<Student> vs;
	vs.push_back(Student("a1", 1));
	vs.push_back(Student("a2", 2));
	vs.push_back(Student("a3", 3));
	vs.push_back(Student("a4", 4));
	vs.push_back(Student("a5", 5));
	/*
	bool binary_search(_FwdIt _First, _FwdIt _Last,const _Ty& _Val, _Pr _Pred)
	{	
		_First = _STD lower_bound(_First, _Last, _Val, _Pred);
		return (_First != _Last && !_Pred(_Val, *_First));
	}
	*/
	//�洢�������ʹ��less����ô����Ҫ������,����Ҫ����<��greater����Ҫ�����ǽ��򣬲���Ҫ����>
	bool flg2 = binary_search(vs.begin(), vs.end(), Student("a4", 4), greater<Student>());
	if (flg2)
	{
		cout << "�ҵ�" << endl;
	}
	else
	{
		cout << "û���ҵ�" << endl;
	}
}
void test05()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(1);
	v.push_back(2);
	v.push_back(1);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);

	//��ѯ1�ж��ٸ�Ԫ��
	int n=count(v.begin(), v.end(), 1);
	cout << n << endl;
	//����2��Ԫ���ж��ٸ�
	n = count_if(v.begin(), v.end(), [](int val)->bool{return val > 2; });
	cout << n << endl;
}

int main()
{
	test05();
	system("pause");
	return EXIT_SUCCESS;
}

