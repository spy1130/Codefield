#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>//�㷨ͷ�ļ�
#include<ctime>
using namespace std;
//Ŀ�ģ�5��ѧ����10����ί��10����ί�ķ���ȥ����ߺ���ͷ֣�ȡƽ���־���ѧ���ķ���
//����ѧ��
class Student
{
public:
	string name;
	int mScore;
};


//1.����ѧ��
void CreateStudent(vector<Student> &vstu)
{
	string setName = "ABCDE";
	for (int i = 0; i < 5; i++)
	{
		//����ѧ��
		Student stu;
		stu.name = "ѧ��";
		stu.name += setName[i];
		stu.mScore = 0;
		vstu.push_back(stu);//��ѧ����������
	}
}
//2.��ί��ѧ�����
void SetScore(vector<Student> &vstu)
{
	srand((unsigned int)time(NULL));
	//����ѧ��
	for (vector<Student>::iterator it = vstu.begin(); it != vstu.end(); ++it)
	{
		//�������
		deque<int> dScore;
		//��ί��ѧ�����
		for (int i = 0; i < 10; i++)
		{
			int score = rand() % 70 + 30;
			dScore.push_back(score);
		}
		//����
		sort(dScore.begin(),dScore.end());
		//ȥ����߷ֺ���ͷ�
		dScore.pop_back();
		dScore.pop_front();

		//���ܷ�
		int tota = 0;
		for (deque<int>::iterator sit = dScore.begin(); sit != dScore.end(); ++sit)
		{
			tota += (*sit);
		}

		//��ƽ����
		int agescore = tota / dScore.size();

		//ƽ���ִ洢��������
		it->mScore = agescore;

	}
}

bool mycomapre(Student &s1, Student &s2)
{
	return s1.mScore > s2.mScore;
}
//3.��������ӡ
void ShowStudentScore(vector<Student> &vstu)
{
	
	sort(vstu.begin(), vstu.end(), mycomapre);

	for (vector<Student>::iterator it = vstu.begin(); it != vstu.end(); ++it)
	{
		cout << "Name:" << it->name << " Score:" << it->mScore << endl;
	}


}
void test()
{
	//�洢ѧ��������
	vector<Student> vstu;

	//1.����ѧ��
	CreateStudent(vstu);
	//2.��ί��ѧ�����
	SetScore(vstu);
	//3.��������ӡ
	ShowStudentScore(vstu);

}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

