function [t,f]=inv_espectro(w,F,P)
% Funcion que a partir de una se?al en frecuencia calcula la se?al en el
% tiempo 
%
% Parametros:
%
% w-> vector de frecuencias (el mismo que el devuelto por la funcion espectro)
% F-> valores de la integral de Fourier
%
% los tama?os de los vectores w y F deben coincidir
%
% Valores devueltos:
%
% t -> Vector de tiempos (simetrico con respecto al instante t=0)
% f -> Valores de la se?al en el tiempo

w=w/(pi*2);

npuntos=length(F);
Wmax=w(npuntos);
Tmuestreo=1/(Wmax*2);
t=-Tmuestreo*(npuntos-1)/2:Tmuestreo:Tmuestreo*(npuntos-1)/2;
repeticiones=(t(length(t))-t(1))/P;

F=F*repeticiones/Tmuestreo;
F=ifftshift(F);



f=ifft(F);
f=fftshift(f);
f=real(f);
