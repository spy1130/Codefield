clc;
clear;

ts = 0;
te = 5;
dt = 0.01;
t = ts:dt:te;

% 系统函数
sys = tf([2 1],[1 4 3]);%(分子多项式的系数,分母多项式的系数)

% 输入信号
f = exp(-2*t).*((t>=0)&(t<=5)) + ((t>5)&(t<=te));

% 计算单位冲激响应
y1 = impulse(sys,t);
subplot(3,1,1);
plot(t,y1);
title('单位冲激响应');
grid on;

% 计算单位阶跃响应
y2 = step(sys,t);
subplot(3,1,2);
plot(t,y2);
title('单位阶跃响应');
grid on;

% 计算零状态响应
y3 = lsim(sys,f,t);
subplot(3,1,3);
plot(t,y3);
title('零状态响应');
grid on;