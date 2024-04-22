clc;clear;
omega=pi/3;
k=-9:9;
x=square (omega*k,50);
stem(k,x);
grid on;