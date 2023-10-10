/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

//���ֲ����㷨���汾C��������������������[lo, hi)�ڲ���Ԫ��e��0 <= lo <= hi <= _size
template <typename T> static Rank binSearch( T* S, T const& e, Rank lo, Rank hi ) {
   /*DSA*/printf ( "BIN search (C)\n" );
   while ( lo < hi ) { //ÿ������������һ�αȽ��жϣ���������֧
      /*DSA*/ //for ( int i = 0; i < lo; i++ ) printf ( "     " ); if ( lo >= 0 ) for ( int i = lo; i < hi; i++ ) printf ( "....^" ); printf ( "\n" );
      Rank mi = ( lo + hi ) >> 1; //���е�Ϊ��㣨�������۰룬��Ч������ֵ��ʾ������һλ��
      ( e < S[mi] ) ? hi = mi : lo = mi + 1; //���ȽϺ�ȷ������[lo, mi)��(mi, hi)
   } //�ɹ����Ҳ�����ǰ��ֹ
   /*DSA*/ //for ( int i = 0; i < lo - 1; i++ ) printf ( "     " ); if ( lo > 0 ) printf ( "....|\n" ); else printf ( "<<<<|\n" );
   return lo - 1; //���ˣ�[lo]Ϊ����e����С�ߣ���[lo-1]��Ϊ������e�������
} //�ж������Ԫ��ʱ����������ߣ�����ʧ��ʱ������ʧ�ܵ�λ��
