clc;
clear;
t=-3:0.01:3;
ft=2*rectpuls(t,3);%高度为2，宽度为3
plot(t,ft);
axis([-3 ,3,-0.5,2.5]);