/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

template <typename Tv, typename Te> //�����������BFS�㷨��ȫͼ��
void Graph<Tv, Te>::bfs( Rank s ) { // s < n
   reset(); Rank dClock = 0; //ȫͼ��λ
   for ( Rank v = s; v < s + n; v++ ) //��s��˳�μ�����ж���
      if ( UNDISCOVERED == status( v % n ) ) //һ��������δ������
         BFS( v % n, dClock ); //��������������һ��BFS
} //��˿���������ȫͼ�������帴�Ӷ���Ȼ����ΪO(n+e)

template <typename Tv, typename Te> //�����������BFS�㷨��������ͨ��
void Graph<Tv, Te>::BFS( Rank v, Rank& dClock ) { // v < n
   Queue<Rank> Q; status( v ) = DISCOVERED; Q.enqueue( v ); dTime( v ) = dClock++; //������
   for ( Rank fClock = 0; !Q.empty(); ) { //��Q���֮ǰ��������
      if ( dTime( v ) < dTime( Q.front() ) ) //dTime�����ӣ���ζ�ſ����µ�һ�������
         dClock++, fClock = 0; //dTime������fTime��λ
      v = Q.dequeue(); //ȡ���׶���v����
      for ( Rank u = firstNbr( v ); -1 != u; u = nextNbr( v, u ) ) //����v��ÿһ���ھ�u
         if ( UNDISCOVERED == status( u ) ) { //��u��δ�����֣�����֮
            status( u ) = DISCOVERED; Q.enqueue( u ); dTime( u ) = dClock;
            type( v, u ) = TREE; parent( u ) = v; //�������ߣ���չBFS��
         } else //��u�ѱ����֣����������ѷ�����ϣ���
            type( v, u ) = CROSS; //��(v, u)�����ڿ��
      status( v ) = VISITED; fTime( v ) = fClock++; //���ˣ�v�������
   } //for
} //BFS
