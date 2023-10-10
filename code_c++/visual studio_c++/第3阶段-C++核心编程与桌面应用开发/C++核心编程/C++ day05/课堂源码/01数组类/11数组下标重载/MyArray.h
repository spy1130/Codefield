#pragma once
#include<iostream>
using namespace std;
class MyArray
{
public:
	MyArray();
	//��������
	MyArray(const MyArray &arr);
	MyArray(int capacity, int val = 0);

	//��д��ֵ��������غ���
	MyArray&operator=(const MyArray &m);

	//Ҫ�ܵ�����ֵ
	int &operator[](int index);

	~MyArray();

	//ͷ��
	void PushFront(int val);
	//β��
	void PushBack(int val);
	//ͷɾ
	void PopFront();
	//βɾ
	void PopBack();
	//��ȡ����Ԫ�ظ���
	int Size();
	//��ȡ��������
	int Capacity();
	//ָ��λ�ò���Ԫ��
	void Insert(int pos, int val);
	//��ȡָ��λ�õ�ֵ
	int &Get(int pos);
	//��ָ��λ���޸�ֵ
	void Set(int pos, int val);

private:
	int *pArray;//ָ������ռ䣬�洢����
	int mSize;//Ԫ�ظ���
	int mCapacity;//����
};

