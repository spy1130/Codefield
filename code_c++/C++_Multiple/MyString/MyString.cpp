#include "MyString.h"

MyString::MyString()
{
	this->pM = new char[1];
	this->pM[0] = '\0';
	this->mSize = 0;
}

MyString::~MyString()
{
	if (this->pM != NULL)
	{
		delete[] this->pM;
		this->pM = NULL;
	}
}

MyString::MyString(int n, char c) // �û������趨��ʼ���ַ�����n��c��ɵ��ַ���
{
	this->pM = new char[n + 1];
	for (int i = 0; i < n; i++)
	{
		this->pM[i] = c;
	}
	this->pM[n] = '\0';
	this->mSize = n;
}
MyString::MyString(const MyString &str) // ��ǳ����
{
	this->pM = new char[strlen(str.pM) + 1];
	strcpy(this->pM, str.pM);
	this->mSize = str.mSize;
}
MyString &MyString::operator=(const MyString &str)
{
	// 1.�ͷ�ԭ���ռ�
	if (this->pM != NULL)
	{
		delete[] this->pM;
		this->pM = NULL;
	}
	// 2.����ռ�
	this->pM = new char[strlen(str.pM) + 1];
	// 3.��������
	strcpy(this->pM, str.pM);
	this->mSize = str.mSize;

	return *this;
}

MyString MyString::operator+(const MyString &str)
{
	// MyString s3=s1+s2;
	// this��s1,str��s2;
	// ��ȡs3Ҫ���ٵĿռ��С
	int newlen = this->mSize + str.mSize + 1;

	// 1.����һ����ʱ�������������޲ι���
	MyString tmp;
	// 2.�ͷ�ԭ���Ŀռ�
	if (tmp.pM != NULL)
	{
		delete[] tmp.pM;
		tmp.pM = NULL;
	}

	// 3.�����µĿռ�
	tmp.pM = new char[newlen];
	memset(tmp.pM, 0, newlen);
	tmp.mSize = this->mSize + str.mSize;

	// 4.׷���ַ����ռ���
	strcat(tmp.pM, this->pM);
	strcat(tmp.pM, str.pM);

	return tmp;
}
MyString MyString::operator+(const char *s)
{
	// MyString s3=s1+"hello";
	// this��s1,s��"hello";
	int newlen = this->mSize + strlen(s);
	// ���ٿռ�
	char *newspace = new char[newlen + 1];
	memset(newspace, 0, newlen + 1);

	// ׷�����ݵ��ռ�
	strcat(newspace, this->pM);
	strcat(newspace, s);
	//�ŵ������������ʽ��
	MyString tmp;
	if (tmp.pM != NULL)
	{
		delete[] tmp.pM;
		tmp.pM = NULL;
	}

	tmp.pM = newspace;

	tmp.mSize = newlen;

	return tmp;
}

MyString &MyString::operator+=(const MyString &str)
{
	/*
	s4+=s3;==>s4=s4+s3;

	this��s4,str��s3
	*/
	// 1.��ȡ�����ַ��������ַ�����
	int newlen = this->mSize + str.mSize;
	// 2.�����¿ռ�
	char *newspace = new char[newlen + 1];
	memset(newspace, 0, newlen + 1);

	// 3.׷������
	strcat(newspace, this->pM);
	strcat(newspace, str.pM);

	// 4.�ͷű���Ŀռ�
	if (this->pM != NULL)
	{
		delete[] this->pM;
		this->pM = NULL;
	}
	//�ŵ�����Ե�ַ��ʽ��
	this->pM = newspace;
	this->mSize = newlen;

	return *this;
}
MyString &MyString::operator+=(const char *s)
{
	// 1.��ȡ�����ַ��������ַ�����
	int newlen = this->mSize + strlen(s);
	// 2.�����¿ռ�
	char *newspace = new char[newlen + 1];
	memset(newspace, 0, newlen + 1);

	// 3.׷������
	strcat(newspace, this->pM);
	strcat(newspace, s);

	// 4.�ͷű���Ŀռ�
	if (this->pM != NULL)
	{
		delete[] this->pM;
		this->pM = NULL;
	}

	this->pM = newspace;
	this->mSize = newlen;

	return *this;
}

int MyString::Size()
{
	return this->mSize;
}

char &MyString::operator[](int index)
{
	return this->pM[index];
}
// ����������������ĳ�Ա���������Բ���Ҫ����������
ostream &operator<<(ostream &out, MyString &str)
{
	out << str.pM;
	return out;
}
istream &operator>>(istream &in, MyString &str)
{
	// cin>>s4;
	// �û�������ַ���Ҫ�洢��s4.pMָ��Ķ����ռ�
	// 1.������ʱ�ռ�
	char tmp[1024] = {0};
	// 2.��ȡ�û��������Ϣ
	in >> tmp;

	// 3.�ͷ�s4�Ŀռ�
	if (str.pM != NULL)
	{
		delete[] str.pM;
		str.pM = NULL;
	}

	// 4.�����µĿռ�
	str.pM = new char[strlen(tmp) + 1];
	memset(str.pM, 0, strlen(tmp) + 1);

	// 5.�����û��������Ϣ�������ռ�
	strcpy(str.pM, tmp);
	str.mSize = strlen(tmp);

	return in;
}