#include <stdio.h>
#include <stdlib.h> //srand(),  rand()
#include <time.h>	//time()
int main()
{
	srand((unsigned)time(NULL));
	int answer = rand() % 10;
	int gus = 0;
	do
	{
		printf("��������µ�����");
		scanf("%d", &gus);
		if (answer > gus)
		{
			printf("С��С��\n");
		}
		else if (answer < gus)
		{
			printf("���˴���\n");
		}
		else
		{
			printf("��ϲ������");
			break;
		}
	} while (gus >= 0 && gus <= 9);
	system("pause");
	return 0;
}