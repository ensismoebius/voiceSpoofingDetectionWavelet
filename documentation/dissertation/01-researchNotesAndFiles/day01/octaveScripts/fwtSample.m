pkg load ltfat;

[signal, frequency] = audioread('duoTone.wav');
J = 14;
[c,info] = fwt(signal,'db20',J);
plotwavelets(c,info,frequency,'dynrange',90);