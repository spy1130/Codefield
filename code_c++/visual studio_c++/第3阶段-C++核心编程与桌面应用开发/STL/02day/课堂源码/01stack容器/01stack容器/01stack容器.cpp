#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<stack>
#include<string>

//ջ�������Ƚ����
//�洢������������
void test01()
{
	stack<int> s;
	s.push(10);
	s.push(20);
	s.push(30);
	s.push(40);
	s.push(50);

	//���ջ��Ԫ��
	while (!s.empty())
	{
		//���ջ��Ԫ��
		cout << s.top() << " ";
		//����ջ��Ԫ��
		s.pop();
	}
	cout << "size:" << s.size() << endl;
}

class Maker
{
public:
	Maker(string name, int age)
	{
		this->name = name;
		this->age = age;
	}
public:
	string name;
	int age;
};
//�洢����
void test02()
{
	stack<Maker> s;
	s.push(Maker("aaa", 18));
	s.push(Maker("bbb", 19));
	s.push(Maker("ccc", 20));

	while (!s.empty())
	{
		cout << "Name:" << s.top().name << " Age:" << s.top().age << endl;
		s.pop();
	}
	
}

int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

