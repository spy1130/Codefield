
[x1,fs] = audioread('segment2.wav');
x1 = x1(:,1);
y1 = fft(x1);
y1 = fftshift(y1);
N = length(x1);
derta_fs = fs/N;

N1 = floor(fs/(2*derta_fs));
w = [-N1*derta_fs:derta_fs:(N-N1-1)*derta_fs];

%***********************************
%ֱ�ӽضϸ�Ƶ��
originalY1 = y1;
fc1 = 4000;
frequency = [-fs/2:derta_fs:fs/2-derta_fs];
clipStart = ceil((-frequency(1)-fc1)/derta_fs);
clipEnd = floor((frequency(end)-fc1)/derta_fs);
y1([1:clipStart-1],:) = 0;
y1([end-clipEnd+1:end],:) = 0;

figure(1);
plot([-fs/2:derta_fs:fs/2-derta_fs],abs(y1)/fs);
title('������');
figure(2)
plot([-fs/2:derta_fs:fs/2-derta_fs],atan2(imag(y1),real(y1)));
title('��λ��');

newx = real(ifft(ifftshift(y1)));
audiowrite('segment2AfterIdeaLowPassFilter.wav', newx, fs);
figure(3);
plot(newx);
title('ʱ����');

y1 = originalY1;
y1([clipStart:end-clipEnd],:) = 0;
newx = ifft(ifftshift(y1));
audiowrite('segment2AfterIdeaHighPassFilter.wav',newx, fs);



%������˹��ͨ�˲������ ��ֹƵ��4000 ���˥�� 20DB�� ���ɴ���0.1pi
fc1 = 4000;
N1 = 2*pi*0.9/(0.1*pi);
wc1 = 2*pi*fc1/fs;
if rem(N1,2)
    N1 = N1+1;
end;
Window = boxcar(N1+1);
b1 = fir1(N1,wc1/pi,Window);
figure(4);
freqz(b1,1,512);

x1_low = filter(b1,1,x1);
figure(5);
plot(x1_low);
title('�źž�����ͨ�˲���ʱ����ͼ');

y_low = fftshift(fft(x1_low));
figure(6);
plot([-fs/2:derta_fs:fs/2-derta_fs],abs(y_low));
title('�źž�����ͨ�˲���������');

figure(7)
plot([-fs/2:derta_fs:fs/2-derta_fs],atan2(imag(y_low),real(y_low)));
title('�źž�����ͨ�˲�����λ��');

audiowrite('segment2AfterLowpassFilter.wav', x1_low, fs);

%��ͨ�˲�����ֹƵ��4000�����˥��40DB�� ���ɴ���0.1PI��
fc2 = fc1;
N2 = 2*pi*3.1/(0.1*pi);
wc2 = 2*pi*fc2/fs;
N2 = N2+mod(N2,2);
Window = hanning(N2+1);
b2 = fir1(N2,wc2/pi,'high',Window);
figure(8);
freqz(b2,1,512);

x1_high = filter(b2,1,x1);
title('��ͨ�˲�����Ƶ����Ӧ');
x1_high = filter(b2,1,x1);
figure(8);
plot(x1_high);
title('�źž�����ͨ�˲���ʱ����ͼ');
figure(9);
y1_high = fftshift(fft(x1_high));
plot([-fs/2:derta_fs:fs/2-derta_fs],abs(y1_high));
title('�źž�����ͨ�˲����ķ�����');

figure(10)
plot([-fs/2:derta_fs:fs/2-derta_fs],atan2(imag(y1_high),real(y1_high)));
title('�źž�����ͨ�˲�����λ��');


audiowrite('segment2AfterHighFilter.wav',x1_high, fs);