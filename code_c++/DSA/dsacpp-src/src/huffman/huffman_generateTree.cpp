/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

/*DSA*/#include "huffman_PQ.h"

HuffTree* minHChar ( HuffForest* forest ) { //��Huffmanɭ�����ҳ�Ȩ����С�ĳ��ַ�
   ListNodePosi<HuffTree*> m = forest->first(); //���׽ڵ�������������нڵ�
   for ( ListNodePosi<HuffTree*> p = m->succ; forest->valid( p ); p = p->succ )
      if ( m->data->root()->data.weight > p->data->root()->data.weight ) //���ϸ���
         m = p; //�ҵ���С�ڵ㣨����Ӧ��Huffman������
   return forest->remove( m ); //��ɭ����ȡ����������������
}

HuffTree* generateTree ( HuffForest* forest ) { //Huffman�����㷨
   while ( 1 < forest->size() ) {
      HuffTree* T1 = minHChar ( forest ); HuffTree* T2 = minHChar ( forest );
      HuffTree* S = new HuffTree(); /*DSA*/printf ( "\n################\nMerging " ); print ( T1->root()->data ); printf ( " with " ); print ( T2->root()->data ); printf ( " ...\n" );
      S->insert ( HuffChar ( '^', T1->root()->data.weight + T2->root()->data.weight ) );
      S->attach ( T1, S->root() ); S->attach ( S->root(), T2 );
      forest->insertAsLast ( S ); /*DSA*/ //print(forest);
   } //assert: ѭ������ʱ��ɭ����Ψһ���б��׽ڵ��У����ǿ�����Huffman������
   return forest->first()->data;
}