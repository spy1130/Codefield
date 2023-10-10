#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<map>
#include<string>

void test()
{
	map<int, string>::iterator it;
	it--;
	it++;
	//it + 2;˫�������
}
/*
��������Ԫ�ز���
map.insert(...); //����������Ԫ�أ�����pair<iterator,bool>
map<int, string> mapStu;
// ��һ�� ͨ��pair�ķ�ʽ�������
mapStu.insert(pair<int, string>(3, "С��"));
// �ڶ��� ͨ��pair�ķ�ʽ�������
mapStu.inset(make_pair(-1, "У��"));
// ������ ͨ��value_type�ķ�ʽ�������
mapStu.insert(map<int, string>::value_type(1, "С��"));
// ������ ͨ������ķ�ʽ����ֵ
mapStu[3] = "С��";
mapStu[5] = "С��";

*/

template<class T>
void printMap(T &m)
{
	for (map<int, string>::iterator it = m.begin(); it != m.end(); ++it)
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

void test01()
{
	map<int, string> mymap;

	//1.
	mymap.insert(pair<int, string>(3, "aaa"));
	//2.
	mymap.insert(make_pair(6, "bbb"));
	//3.
	mymap.insert(map<int, string>::value_type(2, "ccc"));
	//4.
	mymap[4] = "ddd";

	printMap(mymap);

}
//�ı����
struct mapfunc
{
	bool operator()(int key1,int key2)
	{
		return key1 > key2;
	}
};
void test02()
{
	map<int, string, mapfunc> mymap;

	//1.
	mymap.insert(pair<int, string>(3, "aaa"));
	//2.
	mymap.insert(make_pair(6, "bbb"));
	//3.
	mymap.insert(map<int, string>::value_type(2, "ccc"));
	//4.
	mymap[4] = "ddd";

	printMap(mymap);

}

//ע�⣺[]��ʽ��������,���û��ʵֵ����ô��ֵҲ�Ǵ��ڵ�
void test03()
{
	map<int, string> mymap;

	//1.
	mymap.insert(pair<int, string>(3, "aaa"));
	//2.
	mymap.insert(make_pair(6, "bbb"));
	//3.
	mymap.insert(map<int, string>::value_type(2, "ccc"));
	//4.
	mymap[4] = "ddd";

	printMap(mymap);

	cout << "size:" << mymap.size() << endl;

	cout << mymap[100] << endl;//�����ֵ�����ص���ʵֵ
	cout << "size:" << mymap.size() << endl;
}
/*
����
find(key);//���Ҽ�key�Ƿ����,�����ڣ����ظü���Ԫ�صĵ�������/�������ڣ�����map.end();
count(keyElem);//����������keyΪkeyElem�Ķ����������map��˵��Ҫô��0��Ҫô��1����multimap��˵��ֵ���ܴ���1��
lower_bound(keyElem);//���ص�һ��key>=keyElemԪ�صĵ�������
upper_bound(keyElem);//���ص�һ��key>keyElemԪ�صĵ�������
equal_range(keyElem);//����������key��keyElem��ȵ������޵�������������

*/
void test04()
{
	map<int, string> mymap;
	mymap[1] = "aaa";
	mymap[2] = "bbb";
	mymap[3] = "ccc";
	mymap[4] = "ddd";
	mymap[5] = "eee";

	map<int, string>::iterator it = mymap.find(30);
	if (it == mymap.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
	//���Ҵ��ڵ���3����С����
	it = mymap.lower_bound(3);
	if (it == mymap.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
	//���Ҵ���3����С����
	it = mymap.upper_bound(3);
	if (it == mymap.end())
	{
		cout << "����ʧ��" << endl;
	}
	else
	{
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
	cout << "--------------" << endl;
	//���ش��ڵ���2��������С�����������2��ô�ͷ���2�ʹ���2����С��
	pair<map<int,string>::iterator,map<int,string>::iterator> ret=mymap.equal_range(3);

	if (ret.first != mymap.end())
	{
		cout << "key:" << ret.first->first << " Value:" << ret.first->second << endl;
	}

	if (ret.second != mymap.end())
	{
		cout << "key:" << ret.second->first << " Value:" << ret.second->second << endl;

	}
}
int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

