#include "MyArray.h"


MyArray::MyArray()
{
	this->mCapacity = 20;
	this->mSize = 0;

	this->pArray = new int[this->mCapacity];
	for (int i = 0; i < this->mCapacity; i++)
	{
		this->pArray[i] = 0;
	}
}


MyArray::~MyArray()
{
	if (this->pArray != NULL)
	{
		delete[] this->pArray;
		this->pArray = NULL;
	}
}

MyArray::MyArray(const MyArray &arr)
{
	this->mCapacity = arr.mCapacity;
	this->mSize = arr.mSize;
	//1.����ռ�
	this->pArray = new int[arr.mCapacity];
	//2.��������
	for (int i = 0; i < this->mSize; i++)
	{
		this->pArray[i] = arr.pArray[i];
	}
}
MyArray::MyArray(int capacity, int val)
{
	this->mCapacity = capacity;
	this->mSize = capacity;
	this->pArray = new int[capacity];
	for (int i = 0; i < this->mSize; i++)
	{
		this->pArray[i] = val;
	}
}

//ͷ��
void MyArray::PushFront(int val)
{
	//�ж������Ƿ���
	if (this->mSize == this->mCapacity)
	{
		return;
	}

	for (int i = this->mSize - 1; i >= 0; i--)
	{
		this->pArray[i + 1] = this->pArray[i];
	}
	//�ճ���0��λ��
	this->pArray[0] = val;

	//ά��Ԫ�ظ���
	this->mSize++;
}
//β��
void MyArray::PushBack(int val)
{
	//�ж������Ƿ���
	if (this->mSize == this->mCapacity)
	{
		return;
	}

	this->pArray[this->mSize] = val;
	this->mSize++;
}
//ͷɾ
void MyArray::PopFront()
{
	//�ж��Ƿ���Ԫ��
	if (this->mSize == 0)
	{
		return;
	}
	//���������ǰ���ƶ������ǵ�һ��Ԫ��
	for (int i = 0; i < this->mSize - 1; i++)
	{
		this->pArray[i] = this->pArray[i + 1];
	}

	this->mSize--;
}
//βɾ
void MyArray::PopBack()
{

	//�ж��Ƿ���Ԫ��
	if (this->mSize == 0)
	{
		return;
	}

	this->mSize--;

}
//��ȡ����Ԫ�ظ���
int MyArray::Size()
{
	return this->mSize;
}
//��ȡ��������
int MyArray::Capacity()
{
	return this->mCapacity;
}
//ָ��λ�ò���Ԫ�� 
void MyArray::Insert(int pos, int val)
{
	//�ж������Ƿ���
	if (this->mSize == this->mCapacity)
	{
		return;
	}

	//���λ�ò��ԣ��Ͳ��뵽β��
	if (pos<0 || pos>this->mSize - 1)
	{
		pos = this->mSize;
	}

	for (int i = this->mSize - 1; i >= pos; i--)
	{
		this->pArray[i + 1] = this->pArray[i];
	}
	//pos��λ�ÿճ���
	this->pArray[pos] = val;

	this->mSize++;
}
//��ȡָ��λ�õ�ֵ
int &MyArray::Get(int pos)
{
	return this->pArray[pos];
}
//��ָ��λ���޸�ֵ
void MyArray::Set(int pos, int val)
{
	if (pos<0 || pos>this->mCapacity - 1)
	{
		return;
	}

	this->pArray[pos] = val;
}


//��д��ֵ��������غ���
MyArray&MyArray::operator=(const MyArray &m)
{
	cout << "��ֵ����" << endl;
	//1.�ͷ�ԭ���Ŀռ�
	if (this->pArray != NULL)
	{
		delete[] this->pArray;
		this->pArray = NULL;
	}
	this->mCapacity = m.mCapacity;
	this->mSize = m.mSize;
	//2.����ռ䣬��С��m����
	this->pArray = new int[m.mCapacity];
	//3.��������
	cout << "this->mSize:"<<this->mSize << endl;
	for (int i = 0; i < this->mCapacity; i++)
	{
		this->pArray[i] = m.pArray[i];
	}

	return *this;
}

//Ҫ�ܵ�����ֵ
int &MyArray::operator[](int index)
{
	/*
	for (int i = 0; i < 20; i++)
	{
		arr[i] = i + 10;
	}

	for (int i = 0; i < 20; i++)
	{
		cout << arr[i] << " ";
	}
	*/
	//��ֵʱ���Ӽ�
	
	
	if (this->mSize <=index)
	{
		this->mSize++;
	}
	
	return this->pArray[index];
}