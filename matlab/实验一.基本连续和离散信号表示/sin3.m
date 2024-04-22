clc; 
clear;
f0=6;
w0=2*pi*f0;
t=0:0.001:1;
ft=4*sin(w0*t+pi/5);
plot(t,ft);
grid on;