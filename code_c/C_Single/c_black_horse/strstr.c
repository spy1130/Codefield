#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int mystrStr(char *str, char *subStr)
{   
    int num =0;
    while (*str != "\0")
    {  
        if (*str != *subStr)
        {
            str++;
            num++;
            continue;
        }
      
        char * tmpstr =str;
        char * tmpSub= subStr;

        while (*tmpstr !='\0')
        {   
            if(*tmpstr != *tmpSub){
                str++;
                num++;
                break;
            }    
            tmpstr++;
            tmpSub++;
        }
        if(*tmpSub =='\0'){
            return num;
        }       
    }
    return -1;
}
void test()
{
    char *str = "abcdfgaufauwfdnfadawawd";
    int ret = mystrStr(str, "dnf");

    if (ret != -1)
    {
        printf("�ҵ����ַ��� λ�ã�%d\n", ret);
    }
    else
    {
        printf("δ�ҵ��ַ���\n");
    }
}

int main()
{
    test();
    system("pause");
    return 0;
}