// 2������sleep����(Linuxƽ̨)��Ͻṹ�壬��дһ��ģ��ʱ��
#include <stdio.h>
#include <windows.h>

typedef struct Time
{ // ��ʾʱ�����������
    int hour;
    int minute;
    int second;
} Time;

int main(){
    Time data[1]={
        {10,00,00}
    };
    printf("��ǰʱ��Ϊ��");
    while (1)
    {Sleep(1000);
        data[0].second++;
        if(data[0].second >= 60){
            data[0].second=0;
            data[0].minute++;
            if(data[0].minute >=60){
                data[0].minute=0;
                data[0].hour++;
                if(data[0].hour >=24){
                    data[0].hour =0;
                }
            }
        }

    }
    printf("\r%02d:%02d:%02d",data[0].hour,data[0].minute,data[0].second);
    fflush(stdout);
}
/*��ʾ��
1)#include <unistd.h>
  sleep(1);  //����Ϊ��λ

2) fflush(stdout); //��Ϊˢ�´�ӡ������
   "\r"��ת���ַ����������״�ӡ*/
