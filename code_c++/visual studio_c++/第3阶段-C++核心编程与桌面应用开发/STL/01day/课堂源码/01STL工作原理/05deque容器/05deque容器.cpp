#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
using namespace std;

void test()
{
	deque<int>::iterator it;
	it++;
	it--;
	it + 2;
}
void printDeque(const deque<int> &deq)
{
	for (deque<int>::const_iterator it = deq.begin(); it != deq.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}
/*
����
deque<T> deqT;//Ĭ�Ϲ�����ʽ
deque(beg, end);//���캯����[beg, end)�����е�Ԫ�ؿ���������
deque(n, elem);//���캯����n��elem����������
deque(const deque &deq);//�������캯����

*/
void test01()
{
	int arr[] = { 1, 3, 8, 9, 2 };
	deque<int> d1(arr, arr + sizeof(arr) / sizeof(int));
	printDeque(d1);

	deque<int> d2(10, 6);
	printDeque(d2);
}
/*
��ֵ����
assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
deque&operator=(const deque &deq); //���صȺŲ�����
swap(deq);// ��deq�뱾���Ԫ�ػ���

*/
void test02()
{
	int arr[] = { 1, 3, 8, 9, 2 };
	deque<int> d1(arr, arr + sizeof(arr) / sizeof(int));
	printDeque(d1);

	deque<int> d2;
	d2.assign(d1.begin(), d1.end());
	d2.push_back(100);
	printDeque(d2);

	cout << "------------" << endl;
	d1.swap(d2);
	printDeque(d1);
	printDeque(d2);
}
/*
��С����
deque.size();//����������Ԫ�صĸ���
deque.empty();//�ж������Ƿ�Ϊ��
deque.resize(num);//����ָ�������ĳ���Ϊnum,�������䳤������Ĭ��ֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
deque.resize(num, elem); //����ָ�������ĳ���Ϊnum,�������䳤������elemֵ�����λ��,���������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����

*/

void test03()
{
	deque<int> d;
	cout << d.size() << endl;

	if (d.empty())
	{
		cout << "��" << endl;
	}

	d.resize(10, 7);
	printDeque(d);

}
/*
˫�˲����ɾ������
push_back(elem);//������β�����һ������
push_front(elem);//������ͷ������һ������
pop_back();//ɾ���������һ������
pop_front();//ɾ��������һ������

at(idx);//��������idx��ָ�����ݣ����idxԽ�磬�׳�out_of_range��
operator[];//��������idx��ָ�����ݣ����idxԽ�磬���׳��쳣��ֱ�ӳ���
front();//���ص�һ�����ݡ�
back();//�������һ������

*/

void test04()
{
	deque<int> d;
	d.push_back(10);
	d.push_front(20);
	printDeque(d);

	d[0] = 200;
	printDeque(d);
	//d.at(8) = 800;err
	d.at(1) = 100;
	printDeque(d);

	d.pop_back();
	d.pop_front();
	printDeque(d);
}

/*
�������
insert(pos,elem);//��posλ�ò���һ��elemԪ�صĿ��������������ݵ�λ�á�
insert(pos,n,elem);//��posλ�ò���n��elem���ݣ��޷���ֵ��
insert(pos,beg,end);//��posλ�ò���[beg,end)��������ݣ��޷���ֵ��

*/
void test05()
{
	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);
	d.push_back(40);
	d.push_back(50);

	d.insert(d.begin() + 1, 100);
	d.insert(d.begin() + 2, 2, 0);
	printDeque(d);

	deque<int> d2;
	d2.push_back(1000);
	d2.push_back(2000);
	d2.push_back(3000);

	d2.insert(d2.begin() + 1, d.begin(), d.end());
	printDeque(d2);
}
/*
ɾ������
clear();//�Ƴ���������������
erase(beg,end);//ɾ��[beg,end)��������ݣ�������һ�����ݵ�λ�á�
erase(pos);//ɾ��posλ�õ����ݣ�������һ�����ݵ�λ�á�

*/
void test06()
{
	deque<int> d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);
	d.push_back(40);
	d.push_back(50);
	deque<int>::iterator it=d.erase(d.begin() + 1, d.end() - 1);
	cout << *it << endl;
	printDeque(d);
	d.clear();
	printDeque(d);
}
int main()
{
	test06();

	system("pause");
	return EXIT_SUCCESS;
}

