#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include <string>

//��װ��1,�����Ժͷ����ŵ�����,2.����Щ���ݸ���Ȩ��
class Maker
{
public://����Ȩ��
	void set(string Name,int Id)
	{
		id = Id;
		name = Name;
	}
	void printMaker()
	{
		cout << "id=" << id << " name=" << name << endl;
	}
private://˽��
	int id;
	string name;

protected://����
	int a;
};
//�̳У����м̳�
class Son :public Maker
{
public:
	void func()
	{   //�������a�Ǵӹ��еĸ��Ƴ�����
		a = 20;//��������ڿ��Է�������ı���Ȩ�޵ĳ�Ա
		
	}
};
//���ⲻ�ܷ���˽��Ȩ�޵ĳ�Ա
//������Է��ʹ���Ȩ�޵ĳ�Ա
//���ⲻ�ܷ��ʱ���Ȩ�޵ĳ�Ա
//��������ڿ��Է�������ı���Ȩ�޵ĳ�Ա
//������û��Ȩ��֮��

int main(){
    Maker s;
    s.set("spric",031130);
    s.printMaker();
    system("pause");
    return EXIT_SUCCESS;
}