/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

static Rank hashCode( char c ) { return (Rank)c; } //�ַ�
static Rank hashCode( int k ) { return (Rank)k; } //�����Լ���������
static Rank hashCode( long long i ) { return (Rank)( ( i >> 32 ) + (int)i ); }
static Rank hashCode( char s[] ) { //�����ַ�����ѭ����λɢ���루cyclic shift hash code��
   Rank n = strlen( s ); Rank h = 0; //ɢ����
   for ( Rank i = 0; i < n; i++ ) { //�������ң��������ÿһ�ַ�
      h = ( h << 5 ) | ( h >> 27 ); //����5λ������32������27λ��Ϊ�Ŷ���ͨ�������λ�йأ�
      h += s[i]; //�ۼ��ϵ�ǰ�ַ��Ĺ���
   }
   return h; //������õ�ɢ���룬ʵ���Ͽ����Ϊ���Ƶġ�����ʽɢ���롱
} //32�ӽ���26���ʴ˷��������Ӣ���ַ������������ĵ������ַ�����������Ӧ�ĵ���
