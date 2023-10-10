#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//��������
template<class T>
class MyArray
{
public:

	//����ԭ��ָ�룬��ԭ��ָ��ȡ����
	typedef T* iterator;
	MyArray()
	{
		mCapacity = 10;
		mSize = 10;
		p = new T[mCapacity];
		for (int i = 0; i < mCapacity; i++)
		{
			p[i] = i + 1;
		}
	}

	//�ṩ����������ʼλ�õĵ�����
	T* begin()
	{
		return p;
	}

	//���ؽ���λ�õĵ�����
	T* end()
	{
		return p + mSize;
	}


public:
	T* p;
	int mCapacity;
	int mSize;
};

//�㷨
template<class T>
void printArray(T begin,T end)
{
	for (; begin != end; ++begin)
	{
		cout << *begin << " ";
	}
	
}
void test01()
{
	MyArray<int> arr;

	//��ȡ�����ṩ�Ŀ�ʼλ�õ�����
	MyArray<int>::iterator begin=arr.begin();
	//��ȡ�����ṩ�Ľ���λ�õ�����
	MyArray<int>::iterator end = arr.end();

	printArray(begin, end);
}
int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

