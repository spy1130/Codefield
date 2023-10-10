/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

using U = unsigned int; //Լ��������T�����U�����ת��ΪU�������˶���

template <typename T> //���б�����ʼ��λ��p�����Ϊn����������������
void List<T>::radixSort( ListNodePosi<T> p, Rank n ) { // valid(p) && Rank(p) + n <= size
   /*DSA*///printf ( "RadixSort ...\n" );
   ListNodePosi<T> head = p->pred; //����������Ϊ(head, tail)
   ListNodePosi<T> tail = p; for ( Rank i = 0; i < n; i++ ) tail = tail->succ;
   for ( U radixBit = 0x1; radixBit && (p = head); radixBit <<= 1 ) //���·�����
      for ( Rank i = 0; i < n; i++ ) //���ݵ�ǰ����λ�������нڵ�
         radixBit & U (p->succ->data) ? //�ּ�Ϊ��׺��1����ǰ׺��0��
            insert( remove( p->succ ), tail ) : p = p->succ;
}
