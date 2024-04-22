clc;
clear;
t=0:0.1:10;
f=sin(t);
h=0.5*exp(-t);
Lf=length(f);
Lh=length(h);
ncb=t(1)+t(1);
nce=t(Lf)+t (Lh);
ncl=ncb:0.1:nce;
y=[zeros(1,length(ncl))];
for i=0:Lf+Lh-2
    for j=0:Lf-1
        if((i-j>=0)&(i-j+1<=Lh))
            y(i+1)=y(i+1)+f(j+1).*h(i-j+1);
        end
    end
end
subplot(3,1,1);
plot(t,h);
title('f(t)');
subplot(3,1,2);
plot(t,h);
title('h(t)');
subplot(3,1,3);
plot (ncl,y);
title('卷积的近似计算结果');