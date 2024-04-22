clc;
clear;
a=1000;
t1=-5:1/a:5;
f1=2*stepfun(t1,0);%阶跃函数
f2=stepfun(t1,-1/a)-stepfun(t1,1/a);
subplot(2,3,1);
plot (t1,f1);
axis([-5,5,0,3]);%axis函数的参数是[xmin, xmax, ymin, ymax]，分别代表x轴和y轴的最小值和最大值。
title('f1');
subplot(2,3,2);%subplot(2, 3, 1)表示将图形窗口分为2行3列的网格，并在第1个位置创建子图
plot(t1,f2);
axis([-5,5,0,3]);
title('f2');
y=conv(f1,f2);
r=2*length(t1)-1;
t=-10:1/a:10;
subplot(2,3,3);
plot (t,y);
axis([-5,5,0,3]);
title('f1与f2的卷积');
f11=conv (f1,f1);
f22=conv (f2,f2);%计算两个序列的卷积
subplot(2,3,4);
plot (t,f11);
title('f1与f1的卷积');
axis([-5,5,0,10000]);
subplot(2,3,5);
plot(t,f22);
axis([-5,5,0,3]);
title('f2与f2的卷积');