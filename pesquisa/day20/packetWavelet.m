signal = [32, 10, 20, 38, 37, 28, 38, 34, 18, 24, 24, 9, 23, 24, 28, 34];
J = 4;
[c,info] = wpfbt(signal,{'db1',J,'full'});
plotwavelets(c,info,44100,'dynrange',90);