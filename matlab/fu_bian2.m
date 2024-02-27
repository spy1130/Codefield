% 定义信号 
Fs = 1000;
 % 采样频率 
 T = 1/Fs; 
% 采样周期 
L = 1000;
 % 信号长度 
 t = (0:L-1)*T; 
% 时间向量 
f1 = 5;
 % 第一个正弦波的频率 
 f2 = 50;
 % 第二个正弦波的频率 
 x = sin(2*pi*f1*t) + sin(2*pi*f2*t);
 % 信号 % 进行非周期性傅里叶变换 
 Y = fft(x); 
% 计算频率轴 
n = length(x); 
f = (0:n-1)*(Fs/n); 
% 绘制原始信号和傅里叶变换的结果 
figure; 
subplot(2,1,1); 
plot(t, x); 
title('原始信号');
 xlabel('时间 (s)');
 ylabel('幅度'); 
subplot(2,1,2);
 plot(f, abs(Y)); 
title('非周期性傅里叶变换结果');
 xlabel('频率 (Hz)'); 
ylabel('幅度');
