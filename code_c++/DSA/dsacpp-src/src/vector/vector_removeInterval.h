/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename T> Rank Vector<T>::remove( Rank lo, Rank hi ) { //0 <= lo <= hi <= n
   if ( lo == hi ) return 0; //����Ч�ʿ��ǣ����������˻����
   while ( hi < _size ) _elem[lo++] = _elem[hi++]; //��׺[hi, _size)˳��ǰ��hi-loλ
   _size = lo; shrink(); //���¹�ģ��lo=_size֮��������������㣻���Ҫ��������
   //���б�Ҫ��������
   return hi - lo; //���ر�ɾ��Ԫ�ص���Ŀ
}
