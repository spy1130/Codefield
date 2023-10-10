#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include<map>
#include<ctime>
#include<algorithm>
#include<numeric>
#include<functional>

using namespace std;

class Player
{
public:
	string name;
	int age;
	int mScore[3];
};
//����ѡ��
void CreatePlayer(vector<int> &v1, map<int, Player> &mlist)
{
	string setName = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < 24; i++)
	{
		//����ѡ��
		Player p;
		p.name = "ѡ��";
		p.name += setName[i];
		p.age = 20;
		for (int j = 0; j < 3; j++)
		{
			p.mScore[j] = 0;
		}
		//����ѡ�ֱ��
		int ID = 100 + i;
		//����ѡ�ֱ��
		v1.push_back(ID);
		//����ѡ����Ϣ
		mlist.insert(make_pair(ID, p));

	}
}

//1.��ǩ
void PlayerByRandon(vector<int> &v)
{
	random_shuffle(v.begin(), v.end());
}
//2.����
void StartMatch(int index, vector<int> &v1, map<int, Player> &mlist, vector<int> &v2)
{
	//����multimap��������ֵ�Ƿ�����ʵֵ��ѡ�ֱ��
	multimap<int, int, greater<int>> mGroups;
	for (vector<int>::iterator sit = v1.begin(); sit != v1.end(); ++sit)
	{
		//�������
		deque<int> dScore;
		for (int i = 0; i < 10; i++)
		{
			int score = rand() % 50 + 50;
			dScore.push_back(score);
		}
		//����
		sort(dScore.begin(), dScore.end());
		//ȥ����ߺ���ͷ�
		dScore.pop_back();
		dScore.pop_front();

		//���ܷ�
		int toScore = accumulate(dScore.begin(), dScore.end(), 0);
		//��ƽ����
		int avgScore = toScore / dScore.size();

		//���浽ѡ����Ϣ��
		mlist[*sit].mScore[index - 1] = avgScore;

		//��ѡ�ַ���multimap������
		mGroups.insert(make_pair(avgScore, *sit));
		//����
		if (mGroups.size() == 6)
		{
			//������һ����6���ˣ�ȥ��������
			int cnt = 0;
			for (multimap<int, int, greater<int>>::iterator it = mGroups.begin(); it != mGroups.end()&& cnt<3;cnt++, ++it)
			{
				v2.push_back(it->second);

			}

			//�������
			mGroups.clear();
		}
		
	}
}

//3.��ӡ���ֽ���ѡ�ֵ�����
void ShowPlayerScore(int index, vector<int> &v, map<int, Player> &mlist)
{
	cout << "��" << index << "�ֽ�������:" << endl;
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << "Name:" << mlist[*it].name << " Age:" << mlist[*it].age << " Score:" << mlist[*it].mScore[index-1] << endl;
	}
}

void test()
{
	srand((unsigned int)time(NULL));
	vector<int> v1;//����ѡ�ֱ��
	map<int, Player> mlist;//����ѡ����Ϣ

	vector<int> v2;//�����һ�ֽ���ѡ�ֵı��
	vector<int> v3;//����ڶ��ֽ���ѡ�ֵı��
	vector<int> v4;//��������ֽ���ѡ�ֵı��

	//����ѡ��
	CreatePlayer(v1,mlist);

	//��һ��
	//1.��ǩ
	PlayerByRandon(v1);
	//2.����
	StartMatch(1,v1,mlist,v2);
	//3.��ӡ���ֽ���ѡ�ֵ�����
	ShowPlayerScore(1, v2, mlist);

	//�ڶ���
	//1.��ǩ
	PlayerByRandon(v2);
	//2.����
	StartMatch(2, v2, mlist, v3);
	//3.��ӡ���ֽ���ѡ�ֵ�����
	ShowPlayerScore(2, v3, mlist);

	//������
	//1.��ǩ
	PlayerByRandon(v3);
	//2.����
	StartMatch(3, v3, mlist, v4);
	//3.��ӡ���ֽ���ѡ�ֵ�����
	ShowPlayerScore(3, v4, mlist);
}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

