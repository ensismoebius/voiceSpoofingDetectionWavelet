pkg load ltfat

t=0:0.001:2;

x=2*sin(20*pi*t) + sin(100*pi*t);
subplot(2,1,1);
plot(1000*t,x);
grid;
xlabel("Time in milliseconds");
ylabel("Signal amplitude");

subplot(2,1,2);
y=fft(x);
plot(1000*t,abs(y));
xlabel("Frequency");
ylabel("Signal amplitude");