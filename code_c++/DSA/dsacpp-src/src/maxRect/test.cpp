/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#include <cstdlib>
#include <ctime>
#include <cstdio>
using Rank = unsigned int;

__int64 mr_STACK ( int H[], Rank n, Rank& r, Rank& s, Rank& t ); //����һ��ջ��O(n)
__int64 mr_STACKS( int H[], Rank n, Rank& r, Rank& s, Rank& t ); //��������ջ��O(n)
__int64 mr_BRUTE ( int H[], Rank n, Rank& r, Rank& s, Rank& t ); //������O(n^2)

/******************************************************************************************
 * ֱ��ͼ�е�������
 ******************************************************************************************/
int main( int argc, char* argv[] ) {
   int* H; Rank n; //���Ϊn��ֱ��ͼ
   if ( 1 < argc ) { //������ָ�������磺77 4 120 16 96 59 0 15 123 8 79 73 57 96 84 101 26 12 88 81 111 18 87 117 46 90 94 70 125
      H = new int[ n = argc -1 ];
      for ( Rank i = 0; i < n; i++ )
         H[i] = atoi( argv[i+1] );
   } else { //�������
      srand ( ( unsigned int ) time ( NULL ) );
      H = new int[ 1 + ( n = rand() % 128 ) ];
      for ( Rank i = 0; i < n; i++ )
         H[i] = rand() % 128;
   }

   Rank r, s, t; //�����Σ�H[r] x [s, t)
   __int64 mrBrute = mr_BRUTE( H, n, r = -1, s = -1, t = -1 );
   printf( "MaxRect Brute-Force  : %I64d = %d x [%d,%d)\n", mrBrute, H[r], s, t );
   __int64 mrStacks = mr_STACKS( H, n, r = -1, s = -1, t = -1 );
   printf( "MaxRect using STACKS : %I64d = %d x [%d,%d)\n", mrStacks, H[r], s, t );
   __int64 mrStack  = mr_STACK ( H, n, r = -1, s = -1, t = -1 );
   printf( "MaxRect using STACK  : %I64d = %d x [%d,%d)\n", mrStack, H[r], s, t );

   for ( Rank i = 0; i < s; i++ ) {
      printf("%3d:%4d : ", i, H[i]);
      for ( int j = 0; j < H[i]; j++ ) printf("."); printf("\n");
   }
   for ( Rank i = s; i < r; i++ ) {
      printf("%3d:%4d : ", i, H[i]);
      for ( int j = 0; j < H[i]; j++ ) printf( (j < H[r]) ? "#" : "." ); printf("\n");
   }
   {
      printf("%3d:%4d : ", r, H[r]);
      for ( int j = 0; j < H[r]; j++ ) printf("O"); printf("\n");
   }
   for ( Rank i = r+1; i < t; i++ ) {
      printf("%3d:%4d : ", i, H[i]);
      for ( int j = 0; j < H[i]; j++ ) printf( (j < H[r]) ? "#" : "." ); printf("\n");
   }
   for ( Rank i = t; i < n; i++ ) {
      printf("%3d:%4d : ", i, H[i]);
      for ( int j = 0; j < H[i]; j++ ) printf("."); printf("\n");
   }

   delete [] H;
   return 0;
}
