clc;clear;k=-8:8;
omega=pi/4;
phi=pi/3;
fk=2*sin(omega*k+phi);
stem(k,fk);
grid on;