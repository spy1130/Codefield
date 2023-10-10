/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2003-2023. All rights reserved.
 ******************************************************************************************/

#include "stack/stack.h" //����ջ�ṹ������ֱ��ͼH[]�е������Σ�����ʱȡ����ߣ�

__int64 mr_STACKS( int H[], Rank n, Rank& mr_r, Rank& mr_s, Rank& mr_t ) { //��ĩ��-1�ڱ���H[]�ԷǸ�
   Rank* s = new Rank[n]; Stack<Rank> S; //���ҿɼ������
   for( Rank r = 0; r < n; r++ ) { //���μ����
      while ( !S.empty() && ( H[S.top()] >= H[r] ) ) S.pop(); //ÿһ��s(r)
      s[r] = S.empty() ? 0 : 1 + S.top();
      S.push(r);
   }
   while( !S.empty() ) S.pop();

   Rank* t = new Rank[n]; Stack<Rank> T; //����ɼ������
   for( Rank r = n-1; -1 != r; r-- ) { //���μ����
      while ( !T.empty() && H[r] <= H[T.top()] ) T.pop(); //ÿһ��t(r)
      t[r] = T.empty() ? n : T.top();
      T.push(r);
   }
   while( !T.empty() ) T.pop();

   __int64 maxRect = 0;
   for( Rank r = 0; r < n; r++ ) {
      __int64 mR = H[r] * (t[r] - s[r]);
      if ( maxRect < mR )
         { maxRect = mR; mr_r = r; mr_s = s[r]; mr_t = t[r]; }
   }
   delete [] s; delete [] t;
   return maxRect;
} //ÿ���ջ����ջ���������Σ��ۼƳɱ�O(n)
