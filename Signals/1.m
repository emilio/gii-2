%
% Ejercicio 1
%

%% plotsin: plots a function given an amplitude, a time range and angular freq
function []=plotsin(t, A, w, phase)
	f = A * sin(w * t + phase)
	plot(f)
endfunction
% a
% time from 0 to 5
t = [0:.01:5]
A = 1
w = pi
phase = 0
plotsin(t, A, w, phase)

A = 2
plotsin(t, A, w, phase)

%b
A = 1
plotsin(t, A, pi, phase)
hold on
plotsin(t, A, 4*pi, phase)

%c
% 1 Hz = pi rad/s
plotsin(t, A, pi, phase)
hold on
% 3 Hz = 3 pi rad/s
plotsin(t, A, 3* pi, phase)

