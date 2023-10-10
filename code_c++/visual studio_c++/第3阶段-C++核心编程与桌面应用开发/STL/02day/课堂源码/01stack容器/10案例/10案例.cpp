#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<map>
using namespace std;
#include<string>
#include<ctime>

#define SALE_DEPATMENT 1 //���۲���
#define DEVELOP_DEPATMENT 2 //�з�����
#define FINACIAL_DEPATMENT 3 //������

class Woker
{
public:
	string name;//����
	int age;//����
	int Salary;//н��
};

//����Ա��
void CreateWoker(vector<Woker> &vWoker)
{
	srand((unsigned int)time(NULL));

	string setName = "ABCDE";
	for (int i = 0; i < 5; i++)
	{
		Woker woker;
		woker.name = "Ա��";
		woker.name += setName[i];
		woker.age = rand() % 30 + 30;
		woker.Salary = rand() % 20000 + 20000;
		vWoker.push_back(woker);
	}

}
//���飬�ѷֺ����Ա������mWokers
void WokerByGroup(vector<Woker> &vWoker, multimap<int,Woker> &mWokers)
{
	//����Ա��
	for (vector<Woker>::iterator it = vWoker.begin(); it != vWoker.end(); ++it)
	{
		//���ɲ��ű��
		int id = rand() % 3 + 1;
		//Ա�����浽mWokers
		mWokers.insert(make_pair(id, *it));
	}
}
void myGroup(multimap<int, Woker> &mWokers, int id)
{
	//��ѯid
	multimap<int, Woker>::iterator it = mWokers.find(id);
	//��ѯ��������
	int mcount = mWokers.count(id);

	//��ʱ����
	int index = 0;
	for (; it != mWokers.end() && index<mcount; ++it,++index)
	{
		cout << "Name:" << it->second.name << " Age:" << it->second.age << " Salary:" << it->second.Salary << endl;
	}

}
//��ӡԱ����Ϣ
void PrintWoker(multimap<int, Woker> &mWokers)
{

	cout << "������Ա����Ϣ����:" << endl;
	myGroup(mWokers, FINACIAL_DEPATMENT);

	cout << "�з�����Ա����Ϣ����:" << endl;
	myGroup(mWokers, DEVELOP_DEPATMENT);

	cout << "���۲���Ա����Ϣ����:" << endl;
	myGroup(mWokers, SALE_DEPATMENT);

}

void test()
{
	//����δ�����Ա����Ϣ
	vector<Woker> vWoker;
	//��������Ա������Ϣ
	multimap<int, Woker> mWokers;

	//����Ա��
	CreateWoker(vWoker);
	//���飬�ѷֺ����Ա������mWokers
	WokerByGroup(vWoker, mWokers);
	//��ӡԱ����Ϣ
	PrintWoker(mWokers);
}

int main()
{
	test();
	
	system("pause");
	return EXIT_SUCCESS;
}

