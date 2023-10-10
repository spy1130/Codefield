#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<list>
#include<vector>
#include<algorithm>

void printList(const list<int> &mylist)
{
	for (list<int>::const_iterator it = mylist.begin(); it != mylist.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}
/*
����
list<T> lstT;//list���ò���ģ����ʵ��,�����Ĭ�Ϲ�����ʽ��
list(beg,end);//���캯����[beg, end)�����е�Ԫ�ؿ���������
list(n,elem);//���캯����n��elem����������
list(const list &lst);//�������캯����

*/
void test01()
{
	list<int> mylist1(10, 6);
	list<int> mylist2(++mylist1.begin(), --mylist1.end());

	printList(mylist2);
}

/*
����Ԫ�ز����ɾ������
push_back(elem);//������β������һ��Ԫ��
pop_back();//ɾ�����������һ��Ԫ��
push_front(elem);//��������ͷ����һ��Ԫ��
pop_front();//��������ͷ�Ƴ���һ��Ԫ��
insert(pos,elem);//��posλ�ò�elemԪ�صĿ��������������ݵ�λ�á�
insert(pos,n,elem);//��posλ�ò���n��elem���ݣ��޷���ֵ��
insert(pos,beg,end);//��posλ�ò���[beg,end)��������ݣ��޷���ֵ��
clear();//�Ƴ���������������
erase(beg,end);//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ�á�
erase(pos);//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ�á�
remove(elem);//ɾ��������������elemֵƥ���Ԫ�ء�

*/

bool myfunc(int val)
{
	return val > 300;
}
void test02()
{
	list<int> mylist;
	mylist.push_back(10);
	mylist.push_back(20);
	mylist.push_back(30);
	mylist.push_back(40);
	mylist.push_back(50);
	mylist.push_front(100);
	mylist.push_front(200);
	mylist.push_front(300);
	mylist.push_front(400);

	vector<int> v;
	v.push_back(1000);
	v.push_back(2000);
	v.push_back(3000);

	mylist.insert(mylist.begin(), v.begin(), v.end());

	printList(mylist);

	mylist.remove(300);

	printList(mylist);

	//Ҫɾ������300������
	mylist.remove_if(myfunc);
	printList(mylist);
}
/*
��С����
size();//����������Ԫ�صĸ���
empty();//�ж������Ƿ�Ϊ��
resize(num);//����ָ�������ĳ���Ϊnum��
�������䳤������Ĭ��ֵ�����λ�á�
���������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
resize(num, elem);//����ָ�������ĳ���Ϊnum��
�������䳤������elemֵ�����λ�á�
���������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����

*/
void test03()
{
	list<int> mylist;
	for (int i = 0; i < 5; i++)
	{
		mylist.push_back(i + 1);
	}

	cout << "size:" << mylist.size() << endl;

	cout << mylist.empty() << endl;
	if (mylist.empty())
	{
		cout << "��" << endl;
	}
	else
	{
		cout << "��Ϊ��" << endl;
	}

	mylist.resize(3);

	printList(mylist);

}
/*
��ֵ����,���ݵĴ�ȡ
assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
list&operator=(const list &lst);//���صȺŲ�����
swap(lst);//��lst�뱾���Ԫ�ػ�����

front();//���ص�һ��Ԫ�ء�
back();//�������һ��Ԫ�ء�

*/
void test04()
{
	list<int> mylist;
	mylist.assign(10, 10);
	printList(mylist);

	cout << mylist.front() << endl;
	cout << mylist.back() << endl;

	list<int> mylist2;
	for (int i = 0; i < 5; i++)
	{
		mylist2.push_back(i);
	}

	mylist2.swap(mylist);
	printList(mylist2);
}
/*
��ת ����
reverse();//��ת��������lst����1,3,5Ԫ�أ����д˷�����lst�Ͱ���5,3,1Ԫ�ء�
sort(); //list����

*/

bool myfunc2(int v1, int v2)
{
	return v1 > v2;
}
void test05()
{
	list<int> mylist;
	for (int i = 0; i < 5; i++)
	{
		mylist.push_back(i + 10);
	}

	printList(mylist);

	mylist.reverse();

	printList(mylist);

	//ע�⣺list��������ʹ��sort�㷨
	//sort(mylist.begin(), mylist.end());
	mylist.sort();

	printList(mylist);

	mylist.sort(myfunc2);
	printList(mylist);
}

int main()
{
	test05();
	system("pause");
	return EXIT_SUCCESS;
}

