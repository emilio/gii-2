function x=pulso(t,amplitud,ti,tf)
% Proporciona los valores de una se?al pulso para los instantes de tiempo
% proporcionados
%
% Parametros de la funcion
%
% t -> Instantes de tiempo a evaluar
% amplitud -> amplitud del pulso generado
% ti -> instante en el que inicia el pulso
% tf -> instante en el que finaliza el pulso
x=amplitud*(t>(ti) & t<=(tf));