#include "config.h"

//��ȡ��Ч����
int getFileLine(char * fileName)
{
	FILE * file = fopen(fileName, "r");
	if (file == NULL)
	{
		return -1 ;
	}

	char buf[1024] = { 0 };
	int lines = 0;
	while (fgets(buf, 1024, file) != NULL)
	{
		//�������Ч�� ��ͳ��
		if (isValidLine(buf))
		{
			lines++;
		}
		
	}
	fclose(file);

	return lines;
}

//�жϵ�ǰ���Ƿ���Ч
int isValidLine(char * str)
{
	if (str[0] == ' ' || str[0] == '\0' ||  strchr(str,':') == NULL)
	{
		return 0; //��Ч���� �����ؼ�
	}
	return 1;
}


//�����ļ�
void parseFile(char * filePath, int lines, struct ConfigInfo **  configInfo)
{
	
	struct ConfigInfo  * info =  malloc(sizeof(struct ConfigInfo) * lines);

	if (info == NULL)
	{
		return;
	}

	FILE * file = fopen(filePath, "r");

	char buf[1024] = { 0 };
	int index = 0;
	while ( fgets(buf,1024,file ) != NULL)
	{
		//��������  ��Ч���ݲŽ���
		// heroName:aaaa\n
		if (isValidLine(buf))
		{
			memset(info[index].key, 0, 64);
			memset(info[index].value, 0, 64);

			char * pos = strchr(buf, ':'); //pos����ð������λ��

			strncpy(info[index].key, buf, pos - buf); //��key��ȡ�� �ṹ���� ���-1��ԭ���ǲ���Ҫ��ȡ���з�
			strncpy(info[index].value, pos + 1, strlen(pos + 1) - 1);

			/*printf("key =  %s\n", info[index].key);
			printf("value =  %s", info[index].value);*/
			index++;

		}

		memset(buf, 0, 1024);

	}


	*configInfo = info;
}


//��������ֵ ��ȡ ʵֵ 
char * getInfoByKey(char * key, struct ConfigInfo * configInfo, int line)
{
	for (int i = 0; i < line;i++)
	{
		if ( strcmp (key, configInfo[i].key  ) == 0)
		{
			return configInfo[i].value;
		}
	}
	return NULL;
}


//�ͷ���Ϣ
void freeSpace(struct ConfigInfo * configInfo)
{
	if (configInfo != NULL)
	{
		free(configInfo);
		configInfo = NULL;
	}

}