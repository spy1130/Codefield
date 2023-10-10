/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 * ͼGraph
 ******************************************************************************************/
template <typename Tv, typename Te> //�������͡�������
void UniPrint::p ( GraphMatrix<Tv, Te>& s ) { //����
   Rank inD = 0; for ( Rank i = 0; i < s.n; i++ ) inD += s.inDegree ( i );
   Rank outD = 0; for ( Rank i = 0; i < s.n; i++ ) outD += s.outDegree ( i );
   printf ( "%s[%d]*(%d, %d):\n", typeid ( s ).name(), (int) &s, s.n, s.e ); //������Ϣ
// ������
   print ( s.n ); printf ( " " ); print ( inD ); printf ( "|" );
   for ( Rank i = 0; i < s.n; i++ ) { print ( s.vertex ( i ) ); printf ( "[" ); print ( s.status ( i ) ); printf ( "] " ); }
   printf ( "\n" );
// �����У�����
   print ( outD ); printf ( " " ); print ( s.e ); printf ( "|" );
   for ( Rank i = 0; i < s.n; i++ ) { print ( s.inDegree ( i ) ); printf ( " " ); }
   printf ( "| dTime fTime Parent Weight\n" );
// ˮƽ�ָ���
   printf ( "-----------+" ); for ( Rank i = 0; i < s.n; i++ ) printf ( "------" );
   printf ( "+----------------------------\n" );
// �������������
   for ( Rank i = 0; i < s.n; i++ ) {
      print ( s.vertex ( i ) ); printf ( "[" ); print ( s.status ( i ) ); printf ( "] " ); print ( s.outDegree ( i ) ); printf ( "|" );
      for ( Rank j = 0; j < s.n; j++ )
         if ( s.exists ( i, j ) ) { print ( s.edge ( i, j ) ); print ( s.type ( i, j ) ); }
         else printf ( "     ." );
      printf ( "| " ); print ( s.dTime ( i ) ); printf ( " " ); print ( s.fTime ( i ) );
      printf ( "     " ); if ( -1 == s.parent ( i ) ) print ( "^" ); else print ( s.vertex ( s.parent ( i ) ) );
      printf ( "  " ); if ( INT_MAX > s.priority ( i ) ) print ( s.priority ( i ) ); else print ( " INF" );
      printf ( "\n" );
   }
   printf ( "\n" );
}