/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

#include <assert.h>
#include "_share/util.h"
#include "UniPrint/print.h"

using Rank = unsigned int; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����

template <typename T> class CursorList { //�α�ʽ�б�
protected:
   Rank _size; Rank _capacity; //��ģ������
   Rank* _link; T* _elem; //�α�ָ�롢������
   Rank _data, _free; //��������Ϳ�����������
public:
   CursorList ( Rank c = DEFAULT_CAPACITY ) { //����Ϊc
      _link = new Rank[_capacity = c]; //�α�ָ������
      _elem = new T[_capacity = c]; memset ( _elem, 0, c * sizeof ( T ) ); //��������
      _data = _capacity; _size = 0; //���������ʼΪ��
      _free = 0; //�������������е�Ԫ���δ��Ӷ���
      for ( Rank i = 0; i + 1 < _capacity; i++ ) _link[i] = i + 1;
      _link[_capacity - 1] = _capacity;
   }
   ~CursorList() { delete [] _link; delete [] _elem; } //�ͷ��ڲ��ռ�
   Rank size() const { return _size; } //��ģ
   bool empty() const { return !_size; } //�п�
   Rank find ( T const& e ) const { //����
      Rank i = _data; //����������������
      while ( ( _capacity != i ) && ( e != _elem[i] ) ) i = _link[i]; //���αȶ�
      return i;
   }
   Rank insert ( T const& e ) { //����Ԫ��
      assert ( _free < _capacity );
      if ( _size >= _capacity ) return _capacity; //full & insert fails
      Rank k = _free; _free = _link[k]; _elem[k] = e;
      _link[k] = _data; _data = k;
      _size++; return k;
   }
   Rank remove ( Rank k ) { //ɾ����Ϊk��Ԫ��
      assert ( k < _capacity ); //��ǰ�����Ҳ�ȷ��k�Ϸ�
      if ( _data == k ) { //��[k]Ϊ�׽ڵ�
         _data = _link[k];
      } else { //����
         Rank i = _data; while ( k != _link[i] ) i = _link[i]; //find i = pred(k)
         _link[i] = _link[k];
      }
      _link[k] = _free; _free = k; _elem[k] = 0;
      _size--; return k;
   }
   void print() {
      printf ( "size = %4d : ", _size );
      for ( Rank i = _data; _capacity != i; i = _link[i] )
      {  printf("   "); UniPrint::p ( _elem[i] ); }
      printf ( "\n" );
      printf ( "\tRank: " ); for ( Rank i = 0; i < _capacity; i++ ) UniPrint::p ( i ); printf ( "\n" );
      printf ( "\tLink: " ); for ( Rank i = 0; i < _capacity; i++ ) UniPrint::p ( _link[i] ); printf ( "\n" );
      printf ( "\tFree: " ); for ( Rank i = _free; i < _capacity; i = _link[i] ) UniPrint::p ( i ); printf ( "\n" );
      printf ( "\tData: " ); for ( Rank i = _data; i < _capacity; i = _link[i] ) UniPrint::p ( i ); printf ( "\n" );
      printf ( "\n\n" );
   }
};

#include "cursor_list_implementation.h"