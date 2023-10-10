/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> void heapify( T* A, const Rank n ) { //Floyd�����㷨��O(n)ʱ��
   for ( Rank i = n / 2 - 1; - 1 != i; i-- ) //�Ե׶��ϣ�����
      percolateDown( A, n, i ); //�����˺ϲ��Ӷ�
}
