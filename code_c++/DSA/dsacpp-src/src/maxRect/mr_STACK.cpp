/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#include "stack/stack.h" //����ջ�ṹ������ֱ��ͼH[]�е������Σ�����ʱȡ����ߣ�

__int64 mr_STACK( int H[], Rank n, Rank& mr_r, Rank& mr_s, Rank& mr_t ) { //H[]�ԷǸ�
   Stack<Rank> SR; //��ջ����ջ������s[r]-1��r����ǰ��t = t[r]
   __int64 maxRect = 0;
   for (Rank t = 0; t <= n; t++ ) { //���������tΪ�ұ߽��
      while ( !SR.empty() && ( t == n || H[SR.top()] > H[t] ) ) { //ÿһ���������
         Rank r = SR.pop(), s = SR.empty() ? 0 : SR.top() + 1;
         __int64 mR = H[r] * ( t - s );
         if ( maxRect < mR )
            { maxRect = mR; mr_r = r; mr_s = s; mr_t = t; }
      }
      if ( t < n ) SR.push( t ); //ջ��ֻ��¼���е�H[s] = min{ H[k] | s <= k <= t }
   } //assert: SR is empty at exit
   return maxRect;
} //ÿ���ջ����ջ���������Σ��ۼƳɱ�O(n)
