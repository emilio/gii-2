function D=binario(n)
% Proporciona un conjunto de valores binarios calculado de forma aleatoria
%
% Parametros
%
% n -> numero de valores binarios que se desean


rand('state',sum(100*clock));
x=rand(1,n);
D=x<0.5;