% Take 100Hz signal, amplitude modulate it with 2.5KHz, then run it through FFT
clear ; close all; clc

freq1 = 100; 
period1 = 1 / freq1; 
w1 = 2 * pi * freq1; 
num_tsteps = 1000; 
num_periods = 2; 
tstep = num_periods * period1 / num_tsteps; 
t = 0:tstep:(num_periods * period1); 
x1 = sin(w1 * t); 
plot(t, x1); 

disp(['100Hz signal.  Press any key to continue']);
pause;

freq2 = 2500;
period2 = 1 / freq2;
w2 = 2 * pi * freq2;

x2 = sin(w2 * t);

x3 = x1 .* x2;
plot(t, x3);

disp(['Modulated with 2.5KHz signal.  Press any key to continue']);
pause;

n = 2**16;
f_s = num_tsteps / (period1*num_periods);

y = abs(fft(x3, n));
y = fftshift(y);
f = f_s*(-n/2:n/2-1)/n;
plot(f, y);
axis([1500, 3500]);

disp(['Frequency spectrum with FFT.  Press any key to continue']);
pause;