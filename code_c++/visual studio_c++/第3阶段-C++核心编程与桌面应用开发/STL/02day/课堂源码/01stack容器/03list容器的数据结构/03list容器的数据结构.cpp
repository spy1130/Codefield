#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<list>

void test()
{
	/*list<int>::iterator it;
	it--;
	it++;*/
	//it + 2;err
	//˫�������

	list<int> mylist;
	for (int i = 0; i < 10; i++)
	{
		mylist.push_back(i);
	}

	//2015��2017vs :_Myhead==>_Myhead(),_Mysize==>_Mysize()
	list<int>::_Nodeptr node = mylist._Myhead->_Next;//ͷ�ڵ����һ���ڵ�

	for (int i = 0; i < mylist._Mysize * 2; i++)
	{
		cout << "Node:" << node->_Myval << endl;//��ȡ�ڵ�����
		node = node->_Next;
		if (node == mylist._Myhead)
		{
			node = node->_Next;
		}
	}

}

int main()
{
	test();

	system("pause");
	return EXIT_SUCCESS;
}

