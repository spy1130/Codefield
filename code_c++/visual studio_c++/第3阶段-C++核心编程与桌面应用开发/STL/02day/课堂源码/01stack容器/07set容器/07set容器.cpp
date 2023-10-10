#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<set>//multsetҲ�����ͷ�ļ�
#include<algorithm>
#include<string>

void test()
{
	set<int>::iterator it;
	it++;
	it--;
	//it + 2;err
	//˫�������
}

void printSet(set<int> &s)
{
	for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}
/*
���캯��
set<T> st;//setĬ�Ϲ��캯����
mulitset<T> mst; //multisetĬ�Ϲ��캯��:
set(const set &st);//�������캯��

��ֵ����
set&operator=(const set &st);//���صȺŲ�����
swap(st);//����������������
��С����
size();//����������Ԫ�ص���Ŀ
empty();//�ж������Ƿ�Ϊ��

�����ɾ������
insert(elem);//�������в���Ԫ�ء�
clear();//�������Ԫ��
erase(pos);//ɾ��pos��������ָ��Ԫ�أ�������һ��Ԫ�صĵ�������
erase(beg, end);//ɾ������[beg,end)������Ԫ�� ��������һ��Ԫ�صĵ�������
erase(elem);//ɾ��������ֵΪelem��Ԫ�ء�


*/

void test01()
{
	set<int> s;
	s.insert(4);
	s.insert(8);
	s.insert(2);
	s.insert(10);
	s.insert(7);
	//�������������򣬴�С����
	printSet(s);


}

struct myfunc
{
	bool operator()(int v1,int v2)
	{
		return v1 > v2;
	}
};
void printSet2(set<int, myfunc> &s)
{
	for (set<int, myfunc>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}
//�ı�set�����Ĺ��򣬱�Ϊ���򣨴Ӵ�С��
void test02()
{
	set<int, myfunc> s;
	s.insert(4);
	s.insert(8);
	s.insert(2);
	s.insert(10);
	s.insert(7);

	printSet2(s);
}


//set������������������ͬ��Ԫ��
void test03()
{

	set<int, myfunc> s;
	s.insert(4);
	s.insert(8);
	s.insert(2);
	s.insert(10);
	s.insert(7);

	printSet2(s);
	//ע�⣺set������������������ͬ��Ԫ�أ����ǲ�����ͬ��Ԫ�أ����벻�ᱨ������Ҳ������
	//ֻ�ǲ���������������
	s.insert(8);
	printSet2(s);

	pair<set<int,myfunc>::iterator, bool> ret=s.insert(11);

	if (ret.second)
	{
		cout << "����ɹ�" << endl;
	}
	else
	{
		cout << "����ʧ��" << endl;
	}

	s.erase(2);
	s.erase(s.begin());
	printSet2(s);
}
void print(int v)
{
	cout << v << " ";
}
//����ͨ���㷨����
void test04()
{
	multiset<int> s;
	s.insert(4);
	s.insert(8);
	s.insert(2);
	s.insert(2);
	s.insert(7);
	//�������������򣬴�С����
	//����ͨ���㷨�������������ʽ������Ԫ��
	//sort(s.begin(), s.end());

	for_each(s.begin(), s.end(), print);
	cout << endl;

}
/*
���Ҳ���
find(key);//���Ҽ�key�Ƿ����,�����ڣ����ظü���Ԫ�صĵ��������������ڣ�����set.end();
1.	count(key);//���Ҽ�key��Ԫ�ظ���
2.	lower_bound(keyElem);//���ص�һ��key>=keyElemԪ�صĵ�������
3.	upper_bound(keyElem);//���ص�һ��key>keyElemԪ�صĵ�������
equal_range(keyElem);//����������key��keyElem��ȵ������޵�������������

*/
void test05()
{
	set<int> s;
	s.insert(4);
	s.insert(8);
	s.insert(6);
	s.insert(9);
	s.insert(7);
	set<int>::iterator it=s.find(10);
	if (it == s.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ���" << *it << endl;
	}
	//���Ҵ��ڵ���2����С����
	it = s.lower_bound(2);
	if (it == s.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ���" << *it << endl;
	}
	//���Ҵ���2����С����
	it = s.upper_bound(2);
	if (it == s.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "���ҳɹ���" << *it << endl;
	}
	//���ش��ڵ���2��������С�����������2��ô�ͷ���2�ʹ���2����С��
	pair<set<int>::iterator,set<int>::iterator> ret=s.equal_range(2);
	cout << *(ret.first) << endl;
	cout << *(ret.second) << endl;

	multiset<int> s1;
	s1.insert(4);
	s1.insert(4);
	s1.insert(4);
	s1.insert(9);
	s1.insert(7);

	cout << s1.count(4) << endl;
}

//�洢����ʱ����Ҫ����set��������
class Maker
{
public:
	Maker(string name,int age)
	{
		this->name = name;
		this->age = age;
	}
public:
	string name;
	int age;
};

struct Makerfunc
{
	bool operator()(const Maker &m1,const Maker &m2)
	{
		return m1.age > m2.age;
	}
};
void test06()
{
	set<Maker, Makerfunc> s;
	s.insert(Maker("aaa", 18));
	s.insert(Maker("bbb", 19));
	s.insert(Maker("ccc", 20));
	s.insert(Maker("ddd", 21));
	s.insert(Maker("eee", 22));

	for (set<Maker, Makerfunc>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << "Name:" << it->name << " Age:" << it->age << endl;
	}
}
int main()
{
	test06();
	system("pause");
	return EXIT_SUCCESS;
}

