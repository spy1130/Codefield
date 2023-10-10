#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;

// BF�㷨
int BF(string A, string B)
{
    cout << "�������ѯ��ʼ��λ�ã�(��һλΪ0)" << endl;
    int i = 0; // �����ϵ�
    cin >> i;
    int j = 0; // �Ӵ��ϵ�
    while (i < (int)A.length() && j < (int)B.length())
    { // �����ַ�����Ϊ�Ƚϵ���β��ֻ����һ������β������ѭ����
        if (A[i] == B[j])
        {
            i++;
            j++;
        }
        else
        {
            i = i - j + 1; // i-(j-1),�ص�����ƥ�����ʼλ��
            j = 0;
        }
    }
    if (j >= (int)B.length())
    {
        cout << "�Ѳ�ѯ����" << endl;
        return i - (int)B.length();
    }
    else
    {
        cout << "δ��ѯ����" << endl;
        return -1;
    }
}
// ��ȡNEXT����
void getNext(const string& p,int next[])
{

    int len = (int)p.size();
    next[0] = -1;// λ�� 0 �ϵ�Ԫ��ǰ��û���Ӵ�,�ж�Ϊ��Чλ������д��-1��Ҳ��ʹnext��Ӧ������ǰ׺�ͺ�׺
    int k = -1;// ������¼ÿһ��next�����±��Ӧ��������ǰ׺�ͺ�׺
    int j = 0;// i��ǰ��������ƥ����ַ�λ�ã�Ҳ��next���������
    while(j < len - 1)
    {
        if(k == -1 || p[j] == p[k])
        {
            ++j;
            ++k;
            next[j] = k;
        }
        else
        {
            k = next[k];
        }
    }
}
// ��һ������
// �����ͼ�ⷨhttps://www.bilibili.com/video/BV16X4y137qw/?p=1&t=1307
//nextʵ��https://zhuanlan.zhihu.com/p/159429201
int kmp(const string &A, const string &B)
{
    int n = (int)A.size();
    int ans = -1;
    int i = 0;
    int j = 0;
    int patternLen = (int)B.size();

    int next[patternLen] = {0}; // ���ù�
    getNext(B, next);

    while (i < n)
    {
        if (j == -1 || A[i] == B[j])
        {
            ++i;
            ++j;
        }
        else
        {
            j = next[j];
        }

        if (j == patternLen)
        {
            ans = i - patternLen;
            break;
        }
    }

    return ans;
}
int main()
{
    string a = "abbbadabaabcabadba";
    string b = "abaabcaba";
    string chose;
    cout << "��������ҷ�ʽ(BF/KMP):"<<endl ;
    cin>>chose;
    if (chose == "BF")
    {
        int flag = BF(a, b);
        if (flag == -1)
        {
            cout << "ģʽ��������֮�в����ڣ�" << endl;
        }
        else
        {
            cout << "ģʽ��������֮�д��ڣ�" << endl;
            cout << "��ʼλ��Ϊ��" << flag + 1 << endl;
        }
    }
    else if (chose == "KMP")
    {
        cout << kmp(a, b) << endl;
        return 0;
    }
    else
    {
        cout << "������**?" << endl;
        return -1;
    }

    system("pause");
    return EXIT_SUCCESS;
}
// string A = "abbbadabaabcabadba";
// string B = "abaabcaba";
// cout << "������������" << endl;
//string a; // �ı�����������
//cin >> a;
//cout << "������ģʽ����" << endl;
//string b;
//cin >> b; // ģ�崮/ģʽ��(����ƥ����Ӵ�)
