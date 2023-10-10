/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "string_pm/string_pm_test.h"

int* buildNext ( char* P ) { //����ģʽ��P��next���Ľ��汾��
   size_t m = strlen ( P ), j = 0; //��������ָ��
   int* next = new int[m]; int t = next[0] = -1; //next�������Ϊ-1
   while ( j < m - 1 )
      if ( 0 > t || P[t] == P[j] ) { //ƥ��
         if ( P[++t] != P[++j] ) //���������ж�
            next[j] = t; //Ψ���µ�һ���ַ�Ҳƥ��ʱ������ԭ������ֵ
         else
            next[j] = next[t]; //���򣬸���next[t]����ʱ���У�P[next[t]] != P[t] == P[j]��
      } else //ʧ��
         t = next[t]; //����������һֵ�ó��Ե�λ��
   /*DSA*/printString ( P ); printf ( "\n" );
   /*DSA*/printNext ( next, 0, m );
   return next;
}
