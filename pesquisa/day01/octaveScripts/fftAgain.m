close all;
clear all;

clc;

sampleFrequency = 100;
samplePeriod = 1/sampleFrequency;

signalFrequecy = 5;
signalLength = 100000;

timeAxisRange = ( 0:signalLength-1  ) * samplePeriod;

signal = sin( 2*pi*signalFrequecy*timeAxisRange );

signalLength = 100000;

% Plots the signal
figure;
subplot(2,1,1);
plot(timeAxisRange(1:100), real(signal(1:100)), 'b', 'linewidth', 1);
title("Time domain representation");
xlabel("Time in seconds");
ylabel("Amplitude");

% Perform the fast fourrier transform
shiftedSpectrum = fftshift(fft(signal)/signalLength);

magnitudes = abs(shiftedSpectrum);

subplot(2,1,2);
plot(-(sampleFrequency - sampleFrequency/signalLength)/2 : (sampleFrequency/signalLength) : (sampleFrequency - sampleFrequency/signalLength)/2, magnitudes, 'r', 'linewidth', 3);
title("Frequency domain representation");
xlabel("Frequency in Hertz");
ylabel("Amplitude");


