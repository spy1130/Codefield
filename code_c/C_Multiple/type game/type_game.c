#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define max 51
void help()
{
    printf("�����Ŵ�,���������ʼ,��������ĸ���Ὺʼ��ʱ,��������ʾΪ_");
}
void rom(char *str);
void game(char *str);
int main()
{
    char str[max] = {0};
    while (1)
    {
        help();
        rom(str);
        _getch();
        game(str);
        char ch=_getch();
        if(ch==27){
            break;
        }
        system("cls");
    }
    return 0;
}
void rom(char *str)
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < max - 1; i++)
    {
        str[i] = rand() % 26 + 'a';
    }
    str[max - 1] = '\0';
}

void game(char *str)
{
    char ch;
    int count = 0, time_strat, time_end;
    system("cls");
    printf("\n%s\n", str);
    for (int i = 0; i < max - 1; i++)
    {   ch=_getch();
        if (i == 0)
        {   
            time_strat=time(NULL);
        }
        if (ch ==str[i])
        {
            printf("%c",ch);
            count++;
        }
        else{
            printf("_");
        }
    }
    time_end=time(NULL);
    printf("\n�������!\n��ʱ%d s\n", time_end-time_strat);
    printf("��ȷ��%lf %% \n", (count*1.0/(max -1))*100);
    printf("����Esc�˳�,���⽡����! \n\n" );

}