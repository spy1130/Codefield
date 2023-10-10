#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<vector>

void printVector(const vector<int> &vec)
{
	for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void printRvector(vector<int> &vec)
{
	for (vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}
/*
����
vector<T> v; //����ģ��ʵ����ʵ�֣�Ĭ�Ϲ��캯��
vector(v.begin(), v.end());//��v[begin(), end())�����е�Ԫ�ؿ���������
vector(n, elem);//���캯����n��elem����������
vector(const vector &vec);//�������캯����

*/
void test01()
{
	int arr[] = { 2, 3, 1, 9, 8 };
	vector<int> v(arr, arr + sizeof(arr) / sizeof(int));
	printVector(v);

	printRvector(v);

	vector<int> v2(10, 6);
	printVector(v2);
}
/*
���ø�ֵ����
assign(beg, end);//��[beg, end)�����е����ݿ�����ֵ������
assign(n, elem);//��n��elem������ֵ������
vector&operator=(const vector  &vec);//���صȺŲ�����
swap(vec);// ��vec�뱾���Ԫ�ػ�����

*/
void test02()
{ 
	vector<int> v;
	v.assign(10, 6);

	vector<int> v2;
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);

	printVector(v);
	printVector(v2);

	cout << "===================" << endl;

	v.swap(v2);

	printVector(v);
	printVector(v2);

}
/*
��С����
size();//����������Ԫ�صĸ���
empty();//�ж������Ƿ�Ϊ��
resize(int num);//����ָ�������ĳ���Ϊnum���������䳤������Ĭ��ֵ�����λ�á����������̣���ĩβ�����������ȵ�Ԫ�ر�ɾ����
resize(int num, elem);//����ָ�������ĳ���Ϊnum���������䳤������elemֵ�����λ�á����������̣���ĩβ����������>�ȵ�Ԫ�ر�ɾ����
capacity();//����������
reserve(int len);//����Ԥ��len��Ԫ�س��ȣ�Ԥ��λ�ò���ʼ����Ԫ�ز��ɷ��ʡ�

*/

void test03()
{
	//1.resize���ٿռ䣬����ʼ��
	//2.reserve���ٿռ䣬������ʼ��

	vector<int> v2;
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);

	cout << "size��" << v2.size() << endl;
	v2.resize(5);
	cout << "size��" << v2.size() << endl;
	printVector(v2);

	v2.reserve(20);
	v2.push_back(20);
	printVector(v2);
	cout << "size��" << v2.size() << endl;
	cout << "capacity:" << v2.capacity() << endl;

	//cout << v2[10] << endl;//err,���ܷ���δ��ʼ���Ŀռ�


}

//reserve�����ã�Ԥ���ٿռ�
void test04()
{
	vector<int> v;

	v.reserve(10001000);

	int *p = NULL;
	int num = 0;

	for (int i = 0; i < 10001000; i++)
	{
		v.push_back(i);
		if (p != &v[0])
		{
			p = &v[0];
			num++;
		}
	}

	cout << "num=" << num << endl;

}

//swap��ʹ��,��С����
void test05()
{
	vector<int> v;
	for (int i = 0; i < 100000; i++)
	{
		v.push_back(i);
	}

	cout << "����:" << v.capacity() << endl;
	cout << "��С��" << v.size() << endl;

	cout << "----------------" << endl;

	v.resize(10);
	cout << "����:" << v.capacity() << endl;
	cout << "��С��" << v.size() << endl;

	cout << "----------------" << endl;

	vector<int> v2(v);//���ÿ������죬������С
	cout << "����:" << v2.capacity() << endl;
	cout << "��С��" << v2.size() << endl;
	cout << "-------------------" << endl;
	vector<int>(v).swap(v);//���������v���󽻻�
	cout << "����:" << v.capacity() << endl;
	cout << "��С��" << v.size() << endl;
}

/*
���ݴ�ȡ����
at(int idx); //��������idx��ָ�����ݣ����idxԽ�磬�׳�out_of_range�쳣��
operator[];//��������idx��ָ�����ݣ�Խ��ʱ������ֱ�ӱ���
front();//���������е�һ������Ԫ��
back();//�������������һ������Ԫ��

*/
void test06()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);


	cout << v.front() << endl;
	cout << v.back() << endl;

	v.front() = 100;
	v.back() = 300;

	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;

}

/*
�����ɾ������
insert(const_iterator pos, int count,ele);//������ָ��λ��pos����count��Ԫ��ele.
push_back(ele); //β������Ԫ��ele
pop_back();//ɾ�����һ��Ԫ��
erase(const_iterator start, const_iterator end);//ɾ����������start��end֮���Ԫ��
erase(const_iterator pos);//ɾ��������ָ���Ԫ��
clear();//ɾ������������Ԫ��

*/

void test07()
{
	vector<int> v;
	for (int i = 0; i < 5; i++)
	{
		v.push_back(i + 1);
	}

	printVector(v);

	v.insert(v.begin() + 1,2, 100);
	printVector(v);

	v.pop_back();
	printVector(v);
	cout << "-------------" << endl;
	v.erase(v.begin());
	printVector(v);

	v.erase(v.begin() + 1, v.end() - 1);
	printVector(v);

	v.clear();
	printVector(v);


}
int main()
{
	test07();
	system("pause");
	return EXIT_SUCCESS;
}

