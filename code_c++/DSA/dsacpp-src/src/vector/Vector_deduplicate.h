/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> Rank Vector<T>::deduplicate() { //ɾ�������������ظ�Ԫ�أ���Ч�棩
   Rank oldSize = _size; //��¼ԭ��ģ
   for ( Rank i = 1; i < _size; ) //��ǰ����������_elem[1,_size)
      if ( -1 == find(_elem[i], 0, i) ) //��ǰ׺[0,i)��Ѱ����[i]��ͬ�ߣ�����һ������O(i)
         i++; //������ͬ���������������
      else
         remove(i); //����ɾ��[i]��O(_size-i)
   return oldSize - _size; //��ɾ��Ԫ������
}