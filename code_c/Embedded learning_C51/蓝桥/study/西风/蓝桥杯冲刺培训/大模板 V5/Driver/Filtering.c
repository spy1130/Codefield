#include <Filtering.h>
#include <string.h>

#define N 10 // �˲����ڴ�С

int data_array[2][N]; // �洢�����ڵ�����
int sum[2]; // �洢���������ݵĺ�
int index[2]; // ��ǰ���ݵ�����

// ����ƽ���˲�������
// ������new_data - �µ���������
// ����ֵ���˲��������
int Moving_Average_Filter(int ch,int new_data) 
{
    sum[ch] -= data_array[ch][index[ch]]; // ��ȥ�����б��滻������
    data_array[ch][index[ch]] = new_data; // �洢������
    sum[ch] += new_data; // �����µĺ�
    index[ch] = (index[ch] + 1) % N; // ��������
    return sum[ch] / N; // ����ƽ��ֵ
}

// ��ֵ�˲�������
// ������new_data - �µ���������
// ����ֵ���˲��������
int Median_Filter(int new_data) 
{
//    int sorted_data[N];
//    int i, j, temp;

//    // �������ݵ���ʱ����
//    for (i = 0; i < N; i++) {
//        sorted_data[i] = data_array[i];
//    }

//    // ð������
//    for (i = 0; i < N - 1; i++) 
//		{
//      for (j = 0; j < N - i - 1; j++) 
//			{
//            if (sorted_data[j] > sorted_data[j + 1]) 
//						{
//                // ����Ԫ��λ��
//                temp = sorted_data[j];
//                sorted_data[j] = sorted_data[j + 1];
//                sorted_data[j + 1] = temp;
//            }
//      }
//    }

//    data_array[index] = new_data; // �洢������
//    index = (index + 1) % N; // ��������
//    return sorted_data[N / 2]; // ������ֵ
}
