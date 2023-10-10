/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

using Rank = unsigned int;

// �����������ؼ���ֱ��ͼH[]�е������Σ��������ʱȡ�����ߣ�
__int64 mr_BRUTE( int H[], Rank n, Rank& mr_r, Rank& mr_s, Rank& mr_t ) { //������O(n^2)
   __int64 maxRect = 0;
   for ( Rank r = 0, s = 0, t = 0; r < n; r++, s = t = r ) {
      do s--; while ( (-1 != s) && (H[s] >= H[r]) ); s++;
      do t++; while ( (t < n) && (H[r] <= H[t]) );
      __int64 rect = (__int64) H[r] * ( t - s );
      if ( maxRect < rect )
         { maxRect = rect; mr_r = r; mr_s = s; mr_t = t; }
   }
   return maxRect;
} //ÿ��������κ�ʱO(n)���ۼ�O(n^2)
