#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//������������ݽṹ
typedef struct GList{
	int tag; //����������ԭ�ӽ�㻹���ӱ���.��tagΪ1ʱ���ý�����ӱ��ڶ�����Ϊslink�����Դ���ӱ�ĵ�ַ����tagΪ0ʱ���ý����ԭ�ӽ��
	union{
		int data; //���Դ��ԭ�ӽ��ֵ�����������û��Զ���
		GList *slink; //ָ���ӱ��ָ��
	};
	GList *next; //ָ����һ������
} *GListPtr;
GListPtr GreateGList(GListPtr gl){
	char c;
	scanf("%c", &c);
	if(c != ' '){
		gl = (GListPtr)malloc(sizeof(GListPtr));
		if(c == '('){
			gl->tag = 1;
			gl->slink = GreateGList(gl->slink); //�ݹ鹹���ӱ���
		}else{
			gl->tag = 0; //����ԭ�ӽ��
			gl->data = c;
		}
	}else{
		gl = NULL;
	}
	scanf("%c", &c);
	if(gl != NULL){
		if(c == ','){
			gl->next = GreateGList(gl->next); //������������
		}else{
			gl->next = NULL; //�����������ţ���")"��";"ʱ���޺�����
		}
	}
	return gl;
}
void PrintGList(GListPtr gl){
	if(gl != NULL){
		if(gl->tag == 1){
			printf("(");
			if(gl->slink == NULL){
				printf("");
			}else{
				PrintGList(gl->slink); //�ݹ��������ӱ�
			}
		}else{
			printf("%c", gl->data); //������������ֵ
		}
 
		if(gl->tag == 1){
			printf(")");
		}
 
		if(gl->next != NULL){
			printf(",");
			PrintGList(gl->next); //�ݹ���������һ���ڵ�
		}
	}
}
int FindGList(GListPtr gl,int t){
	int mark = 0;
	if(gl != NULL){
		if(gl->tag == 0 && gl->data == t){
			mark = 1;
		}else {
			if(gl->tag == 1){
				mark = FindGList(gl->slink,t);
			}else{
				mark = FindGList(gl->next,t);
			}
		}
	}
	return mark; //�����ҳɹ�����1
}
int main(){

    system("pause");
    return EXIT_SUCCESS;
}
//https://blog.csdn.net/Alexshi5/article/details/99897725