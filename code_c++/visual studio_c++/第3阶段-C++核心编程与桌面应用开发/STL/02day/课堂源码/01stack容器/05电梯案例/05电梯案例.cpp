#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<list>
#include<vector>
#include<queue>
#include<string>
#include<ctime>

//������Ա
class Student
{
public:
	string name;
};

//��ӡ��Ա
void printVector(vector<Student> &vec)
{
	for (vector<Student>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		cout << (*it).name << endl;
	}
}

//������Ա
void CreateStudent(queue<Student> &que,int num)
{
	string setName = "ABCDEFGHIJKLMN";
	int sum = rand() % 10;
	for (int i = 0; i < sum; i++)
	{
		Student stu;

		char buf[64] = { 0 };
		sprintf(buf, "%d", num);
		string s(buf);

		stu.name = "��";
		stu.name += s;
		stu.name += "��";
		stu.name += setName[i];

		que.push(stu);//ÿ�����Ա�����������
	}
}
//mylist�ǵ���,que��������,pushV�ǿ��������ݵ���Ա
int PushList(list<Student> &mylist,queue<Student> &que,vector<Student> &pushV)
{
	int tmppush = 0;//��ʱ��������¼��������Ա��
	
	while (!que.empty())
	{
		if (mylist.size() >= 15)//��������
		{
			break;
		}

		Student s = que.front();

		//������vector
		pushV.push_back(s);

		//������
		mylist.push_back(s);

		//�����ж�ͷԪ�س�����
		que.pop();

		tmppush++;
	}

	return tmppush;
}
//mylist�ǵ���,popV��¼��������Ա��num����
int PopList(list<Student> &mylist, vector<Student> &popV,int num)
{
	int tmppop = 0;

	if (num == 17)//������17�㣬��ô���е��˶�Ҫ������
	{
		while (!mylist.empty())
		{
			Student s=mylist.front();
			//�ѳ����ݵ���Ա������popV��
			popV.push_back(s);

			mylist.pop_front();//�Ƴ����ݵ���
			tmppop++;
		}
	}

	int n = rand() % 5;//�������������
	if (n == 0)
	{
		return tmppop;
	}
	//���������ˣ����������ڵ�����������ݵ����������˳�����
	if (mylist.size() > 0 && mylist.size() >= n)
	{
		for (int i = 0; i < n; i++)
		{
			Student s = mylist.front();

			//�ѳ����ݵ���Ա������popV��
			popV.push_back(s);
			mylist.pop_front();//�Ƴ����ݵ���
			tmppop++;
		}
	}

	return tmppop;
}
void test()
{
	srand((unsigned int)time(NULL));
	list<Student> mylist;//��������

	int Pushnum = 0;//��¼�����ݵ�������
	int Popnum = 0;//��¼�����ݵ�������

	vector<Student> pushV;//��¼�����ݵ���Ա
	vector<Student> popV;//��¼�����ݵ���Ա

	//��������
	for (int i = 1; i < 18; i++)
	{
		//������Ա
		queue<Student> que;
		//������Ա����
		CreateStudent(que, i);

		//�ж��Ƿ��ܽ�����
		if (mylist.size() <= 15)
		{
			//17�㲻Ҫ���ˣ���17���¼�����
			if (i < 17)
			{
				//������
				Pushnum += PushList(mylist, que, pushV);
			}
		}

		//�жϳ���������
		if (mylist.size()>0)//����Ҫ���˲��ܳ�
		{
			if (i > 1)//1��ʱ�������ǿյ�
			{
				//������
				Popnum += PopList(mylist, popV, i);
			}
		}
	}

	//��ӡ�����ݵ���Ա
	printVector(pushV);
	cout << "����������:" << Pushnum << endl;
	//��ӡ�����ݵ���Ա
	printVector(popV);
	cout << "����������:" << Popnum << endl;

}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

