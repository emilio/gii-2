function [w,F]=espectro(t,x,P)
% Funcion que a partir de una se?al temporal calcula su integral de Fourier
%
% Parametros:
%
% t-> vector de tiempos (con espaciado uniforme)
% x-> valores de la se?al en cada instante de tiempo
% P-> Periodo de la se?al en el tiempo
% los tama?os de los vectores t y x deben coincidir
%
% Valores devueltos:
%
% w -> Vector de frecuencias (rad/s)
% F -> Valores de la integral de Fourier en las frecuencias del vector
%      anterior (Son valores complejos)

Tmuestreo=t(2)-t(1);
repeticiones=(t(length(t))-t(1))/P;
X=fft(x);

X=fftshift(X);
F=X*Tmuestreo/repeticiones;

npuntos=length(x);
w=-1/(Tmuestreo*2):1/(Tmuestreo*(npuntos-1)):1/(Tmuestreo*2);
w=w*2*pi;
