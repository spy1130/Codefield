#include "dynamicArray.h"


//��ʼ������ �������� ��ʼ��������
struct dynamicArray * init_dynamicArray(int capacity)
{
	struct dynamicArray * array = malloc(sizeof(struct dynamicArray));

	if (array == NULL)
	{
		return NULL;
	}

	//���������Գ�ʼ��
	array->m_Capacity = capacity;
	array->m_Size = 0;
	array->pAddr = malloc(sizeof(void *)* capacity);

	if (array->pAddr == NULL)
	{
		return NULL;
	}

	return array;
}


//����Ԫ��
void insert_dynamicArray(struct dynamicArray * arr, int pos, void * data)
{
	if (arr == NULL)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	if (pos < 0 || pos > arr->m_Size)
	{
		//��Ч��λ��  ����β��
		pos = arr->m_Size;
	}

	//�ж��Ƿ��пռ���в��룬���û�пռ��ˣ���ô��̬��չ
	if (arr->m_Size >= arr->m_Capacity)
	{
		//1����������ռ��С
		int newCapacity = arr->m_Capacity * 2;

		//2�������¿ռ�
		void ** newSpace = malloc(sizeof (void *)* newCapacity);

		//3�� ��ԭ�����ݿ������¿ռ���
		memcpy(newSpace, arr->pAddr, sizeof(void*)* arr->m_Capacity);

		//4�� �ͷ�ԭ�пռ�
		free(arr->pAddr);

		//5�� ����ָ���ָ��
		arr->pAddr = newSpace;

		//6����������������
		arr->m_Capacity = newCapacity;
	}


	//��������

	for (int i = arr->m_Size - 1; i >= pos; i--)
	{
		//���ݺ���
		arr->pAddr[i + 1] = arr->pAddr[i];
	}
	//�������ݷ��뵽ָ��λ����
	arr->pAddr[pos] = data;


	//���������С
	arr->m_Size++;
}

//��������
void foreach_dynamicArray(struct dynamicArray * arr, void(*myPrint)(void *))
{
	if (arr == NULL)
	{
		return;
	}

	if (myPrint == NULL)
	{
		return;
	}

	for (int i = 0; i < arr->m_Size; i++)
	{
		myPrint(arr->pAddr[i]);
	}
}


//ɾ������
void removeByPos_dynamicArray(struct dynamicArray * arr, int pos)
{
	if (arr == NULL)
	{
		return;
	}

	//��Чλ�� ��ֱ��return
	if (pos < 0 || pos >arr->m_Size - 1)
	{
		return;
	}

	//�ƶ�����
	for (int i = pos; i < arr->m_Size - 1; i++)
	{
		arr->pAddr[i] = arr->pAddr[i + 1];
	}

	//���´�С
	arr->m_Size--;

}

//����ֵ ��ɾ������������
void removeByValue_dynamicArray(struct dynamicArray * arr, void * data, int(*myCompare)(void *, void *))
{
	if (arr == NULL)
	{
		return;
	}
	if (data == NULL)
	{
		return;
	}

	for (int i = 0; i < arr->m_Size; i++)
	{
		if (myCompare(arr->pAddr[i], data))
		{
			//����Աȳɹ��ˣ���ôҪɾ��i�±��Ԫ��
			removeByPos_dynamicArray(arr, i);
			break;
		}
	}


}

//��������
void destroy_dynamicArray(struct dynamicArray * arr)
{

	if (arr == NULL)
	{
		return;
	}

	if (arr->pAddr != NULL)
	{
		free(arr->pAddr);
		arr->pAddr = NULL;
	}


	free(arr);
	arr = NULL;

}
