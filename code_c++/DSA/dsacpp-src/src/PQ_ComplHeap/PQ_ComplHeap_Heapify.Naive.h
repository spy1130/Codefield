/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void heapify( T* A, const Rank n ) { //���������㷨��O(nlogn)ʱ��
   for ( Rank i = 1; i < n; i++ ) //�Զ����£�����
      percolateUp( A, i ); //�����˲�����ڵ�
}
