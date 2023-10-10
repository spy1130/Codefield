/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> template <typename PU> //���ȼ�������ȫͼ��
void Graph<Tv, Te>::pfs( Rank s, PU prioUpdater ) { // s < n
   reset(); //ȫͼ��λ
   for ( Rank v = s; v < s + n; v++ ) //��s��˳�μ�����ж���
      if ( UNDISCOVERED == status( v % n ) ) //һ��������δ������
         PFS( v % n, prioUpdater ); //��������������һ��PFS
} //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)

template <typename Tv, typename Te> template <typename PU> //�������͡������͡����ȼ�������
void Graph<Tv, Te>::PFS( Rank v, PU prioUpdater ) { //���ȼ�������������ͨ��
   priority( v ) = 0; status( v ) = VISITED; //��ʼ�������v����PFS����
   while ( 1 ) { //����һ����ͱ߼���PFS����
      for ( Rank u = firstNbr( v ); - 1 != u; u = nextNbr( v, u ) ) //��v��ÿһ���ھ�u
         prioUpdater( this, v, u ); //���������ȼ����丸��
      int shortest = INT_MAX;
      for ( Rank u = 0; u < n; u++ ) //����δ����������Ķ����У�ѡ����һ�����ȼ�
         if ( ( UNDISCOVERED == status( u ) ) && ( shortest > priority( u ) ) ) //��ߵ�
            { shortest = priority( u ), v = u; } //����v
      if ( shortest == INT_MAX ) break; //ֱ�����ж�����Ѽ���
      status( v ) = VISITED; type( parent( v ), v ) = TREE; //��v���������
   }
} //ͨ�������������ȼ����²���prioUpdater������ʵ�ֲ�ͬ���㷨����
