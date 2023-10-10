/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#pragma once

#include "listNode.h" //�����б�ڵ���

template <typename T> class List { //�б�ģ����

private:
   Rank _size; ListNodePosi<T> header, trailer; //��ģ��ͷ�ڱ���β�ڱ�

protected:
   void init(); //�б���ʱ�ĳ�ʼ��
   Rank clear(); //������нڵ�
   void copyNodes( ListNodePosi<T>, Rank ); //�����б�����λ��p���n��
   ListNodePosi<T> merge( ListNodePosi<T>, Rank, List<T>&, ListNodePosi<T>, Rank ); //�鲢
   void mergeSort( ListNodePosi<T>&, Rank ); //�Դ�p��ʼ������n���ڵ�鲢����
   void selectionSort( ListNodePosi<T>, Rank ); //�Դ�p��ʼ������n���ڵ�ѡ������
   void insertionSort( ListNodePosi<T>, Rank ); //�Դ�p��ʼ������n���ڵ��������
   void radixSort( ListNodePosi<T>, Rank ); //�Դ�p��ʼ������n���ڵ��������

public:
// ���캯��
   List() { init(); } //Ĭ��
   List( List<T> const& L ); //���帴���б�L
   List( List<T> const& L, Rank r, Rank n ); //�����б�L���Ե�r�����n��
   List( ListNodePosi<T> p, Rank n ); //�����б�����λ��p���n��
   // ��������
   ~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
// ֻ�����ʽӿ�
   Rank size() const { return _size; } //��ģ
   bool empty() const { return _size <= 0; } //�п�
   ListNodePosi<T> operator[]( Rank r ) const; //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
   ListNodePosi<T> first() const { return header->succ; } //�׽ڵ�λ��
   ListNodePosi<T> last() const { return trailer->pred; } //ĩ�ڵ�λ��
   bool valid( ListNodePosi<T> p ) //�ж�λ��p�Ƿ����Ϸ�
   { return p && ( trailer != p ) && ( header != p ); } //��ͷ��β�ڵ��ͬ��NULL
   ListNodePosi<T> find( T const& e ) const //�����б����
   { return find( e, _size, trailer ); }
   ListNodePosi<T> find( T const& e, Rank n, ListNodePosi<T> p ) const; //�����������
   ListNodePosi<T> search( T const& e ) const //�����б����
   { return search( e, _size, trailer ); }
   ListNodePosi<T> search( T const& e, Rank n, ListNodePosi<T> p ) const; //�����������
   ListNodePosi<T> selectMax( ListNodePosi<T> p, Rank n ); //��p����n-1�������ѡ�������
   ListNodePosi<T> selectMax() { return selectMax( header->succ, _size ); } //���������
// ��д���ʽӿ�
   ListNodePosi<T> insertAsFirst( T const& e ); //��e�����׽ڵ����
   ListNodePosi<T> insertAsLast( T const& e ); //��e����ĩ�ڵ����
   ListNodePosi<T> insert( ListNodePosi<T> p, T const& e ); //��e����p�ĺ�̲���
   ListNodePosi<T> insert( T const& e, ListNodePosi<T> p ); //��e����p��ǰ������
   T remove( ListNodePosi<T> p ); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
   void merge( List<T>& L ) { merge( header->succ, _size, L, L.header->succ, L._size ); } //ȫ�б�鲢
   void sort( ListNodePosi<T>, Rank ); //�б���������
   void sort() { sort( first(), _size ); } //�б���������
   Rank deduplicate(); //����ȥ��
   Rank uniquify(); //����ȥ��
   void reverse(); //ǰ���ã�ϰ�⣩
// ����
   void traverse( void ( * )( T& ) ); //����ʵʩvisit����������ָ�룩
   template <typename VST> void traverse( VST& ); //����ʵʩvisit��������������
}; //List

#include "List_implementation.h"
