k = -5:10;  
f = 5 * (0.8 .^ k) .* cos(0.9 * pi * k);  %矩阵之间点乘
stem(k, f); 
grid on; 
