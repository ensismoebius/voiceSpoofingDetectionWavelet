close all;
clear all;

clc;

[signal, frequency] = audioread('duoTone.wav');
signalLength = length(signal);

period = 1/frequency;
timeAxisRange = ( 0:signalLength-1  ) * period;

% Plots the signal
figure;
subplot(2,1,1);
plot(timeAxisRange(1:signalLength), real(signal(1:signalLength)), 'b', 'linewidth', 1);
title("Time domain representation");
xlabel("Time in seconds");
ylabel("Amplitude");

% Perform the fast fourrier transform
shiftedSpectrum = fftshift(fft(signal)/signalLength);

% Calculate the magnitudes for ploting
magnitudes = abs(shiftedSpectrum);

subplot(2,1,2);
plot(-(frequency - frequency/signalLength)/2 : (frequency/signalLength) : (frequency - frequency/signalLength)/2, magnitudes, 'r', 'linewidth', 3);
title("Frequency domain representation");
xlabel("Frequency in Hertz");
ylabel("Amplitude");

% Play the signal
player = audioplayer (signal, frequency);
play(player);



