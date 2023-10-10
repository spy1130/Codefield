#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
#include<string>
#include<ctime>
using namespace std;

//merge�㷨 ����Ԫ�غϲ������洢����һ������
void test01()
{
	vector<int> v1;
	vector<int> v2;

	for (int i = 5; i >=0; i--)
	{
		v1.push_back(i + 1);
		v2.push_back(i + 2);
	}

	vector<int> v3;
	v3.resize(v1.size() + v2.size());

	//���������������ô�����������Ͳ���д
	/*merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());

	for_each(v3.begin(), v3.end(), [](int val){cout << val << " "; });*/

	////��������ǽ�����ô������������Ҫдgreater<int>()
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(),greater<int>());

	for_each(v3.begin(), v3.end(), [](int val){cout << val << " "; });
}

//sort�㷨 ����Ԫ������
void test02()
{
	vector<int> v;
	v.push_back(8);
	v.push_back(12);
	v.push_back(2);
	v.push_back(7);
	v.push_back(3);

	sort(v.begin(), v.end());
	for_each(v.begin(), v.end(), [](int val){cout << val << " "; });

	//���Ԫ���Ƕ�����ôҪд����������
	//sort(v.begin(), v.end(),��������);
}

//random_shuffle ϴ��
void test03()
{
	vector<int> v1;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i + 1);
	}
	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; });
	cout << endl;

	srand((unsigned int)time(NULL));
	random_shuffle(v1.begin(), v1.end());
	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; });
	cout << endl;

}

//reverse�㷨 ��תָ����Χ��Ԫ��
void test04()
{
	vector<int> v1;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i + 1);
	}
	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; });
	cout << endl;

	reverse(v1.begin(), v1.end());

	for_each(v1.begin(), v1.end(), [](int val){cout << val << " "; });
	cout << endl;
}
int main()
{
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

