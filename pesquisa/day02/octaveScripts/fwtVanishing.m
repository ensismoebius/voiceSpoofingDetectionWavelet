pkg load ltfat;

[sig, frequency] = audioread('duoTone.wav');

subplot(2,1,1)
plot(sig)
grid on
title('Signal')

[c,info] = fwt(sig,'db8',1);

subplot(2,1,2)
plotwavelets(c,info);

%subplot(3,1,2)
%plot(c)
%ylim([-40, 40])
%xlim([-10, 100])
%grid on
%title('Approximation Coefficients - db1')
