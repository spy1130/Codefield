/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * Hashtable
 ******************************************************************************************/

template <typename K, typename V> //e��value
void UniPrint::p ( Hashtable<K, V>& HT ) { //����
   printf( "%s[%d]*(%d + %d)/%d:\n", typeid( HT ).name(), (int)&HT, HT.N, HT.removed->size(), HT.M ); //������Ϣ
   for ( Rank i = 0; i < HT.M; i++ ) //���Ͱ���
      printf ( "  %4d  ", i );
   printf ( "\n" );
   for ( Rank i = 0; i < HT.M; i++ ) //�������Ԫ��
      if ( HT.ht[i] ) printf ( "-<%04d>-", HT.ht[i]->key ); //��ʾ�ã���������int
      else if ( HT.removed->test(i) ) printf ( "-<****>-" );
      else printf ( "--------" );
   printf ( "\n" );
   for ( Rank i = 0; i < HT.M; i++ ) //�������Ԫ��
      if ( HT.ht[i] ) printf ( "    %c   ", HT.ht[i]->value ); //��ʾ�ã���������char
//      if (ht.ht[i]) printf("%8s", ht.ht[i]->value); //���Huffman������ʹ�õ�ɢ�б�
      else if ( HT.removed->test(i) ) printf ( "    *   " );
      else printf ( "        " );
   printf ( "\n" );
}
