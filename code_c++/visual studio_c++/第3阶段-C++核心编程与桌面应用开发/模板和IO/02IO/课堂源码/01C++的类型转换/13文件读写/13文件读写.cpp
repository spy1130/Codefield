#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//1.����ͷ�ļ�
#include<fstream>
//�ѳ����е���Ϣ�������������Ȼ��д���ļ���д�ļ���
void test01()
{
	//2.����������
	ofstream ofs;
	//3.���ļ�,��д�ķ�ʽ�򿪣����û���ļ����ʹ���
	ofs.open("test.txt", ios::out | ios::trunc);
	//4.�ж��Ƿ�򿪳ɹ�
	if (!ofs.is_open())
	{
		cout << "��ʧ��" << endl;
	}

	//5.д��Ϣ
	ofs << "����:���" << endl;
	ofs << "����:18" << endl;
	ofs << "���:180cm" << endl;

	//6.�ر��ļ�
	ofs.close();//�ر��ļ�����ˢ�»�����

}

//�Ѵ�����Ϣ���뵽��������Ȼ����������У����ļ���
void test02()
{
	ifstream ifs;
	ifs.open("test.txt", ios::in);
	if (ifs.is_open() == false)
	{
		cout << "��ʧ��" << endl;
	}
	//��һ�ַ�ʽ��ȡ�ļ�
	//һ��һ�ж�ȡ
	/*char buf[1024] = { 0 };
	while (ifs>>buf)
	{
		cout << buf << endl;
	}*/
	//�ڶ��ַ�ʽ���ļ�
	//char buf[1024] = { 0 };
	//while (!ifs.eof())//�ж��Ƿ�����ļ�β��
	//{
	//	ifs.getline(buf, sizeof(buf));
	//	cout << buf << endl;

	//}
	//�����ַ�ʽ��ȡ�ļ�
	//�����ַ���ȡ
	char c;
	while ((c=ifs.get())!=EOF)
	{
		cout << c;
	}

	//�ر��ļ�
	ifs.close();
}
int main()
{
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

