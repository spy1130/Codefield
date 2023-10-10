#pragma once

template<class T>
class MyArray
{
public:
	MyArray(int capacity)
	{
		this->mCapacity = capacity;
		this->mSize = 0;
		//T�����Maker,����Ҫ����ʲô���캯����Ҫ�����޲ι���
		p = new T[this->mCapacity];

	}

	//��������
	MyArray(const MyArray &arr)
	{
		this->mCapacity = arr.mCapacity;
		this->mSize = arr.mSize;
		p = new T[arr.mCapacity];

		for (int i = 0; i < this->mSize; i++)
		{
			p[i] = arr.p[i];
		}
	}
	//��ֵ����
	MyArray &operator=(const MyArray &arr)
	{
		if (this->p != NULL)
		{
			delete[] this->p;
			this->p = NULL;
		}

		p = new T[arr.mCapacity];
		this->mSize = arr.mSize;
		this->mCapacity = arr.mCapacity;
		for (int i = 0; i < this->mSize; i++)
		{
			p[i] = arr.p[i];

		}

		return *this;
	}

	//����[]
	T &operator[](int index)
	{
		return this->p[index];
	}
	//β��
	void Push_Back(const T &val)
	{
		if (this->mSize == this->mCapacity)
		{
			return;
		}

		this->p[this->mSize] = val;
		this->mSize++;
	}

	//βɾ
	void Pop_Back()
	{
		if (this->mSize == 0)
		{
			return;
		}

		this->mSize--;
	}

	~MyArray()
	{
		if (this->p != NULL)
		{
			delete[] p;
			p = NULL;
		}
	}

	int getSize()
	{
		return this->mSize;
	}
private:
	T *p;
	int mCapacity;
	int mSize;
};
