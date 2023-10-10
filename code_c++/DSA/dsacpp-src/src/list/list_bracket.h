/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> //�����±����������ͨ����ֱ�ӷ����б�ڵ㣨O(r)Ч�ʣ��䷽�㣬����ã�
ListNodePosi<T> List<T>::operator[]( Rank r ) const { // 0 <= r < size
   ListNodePosi<T> p = first(); //���׽ڵ����
   while ( 0 < r-- ) p = p->succ; //˳����r���ڵ㼴��
   return p; //Ŀ��ڵ�
}
