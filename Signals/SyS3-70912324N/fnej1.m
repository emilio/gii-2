% Realizar una función de MATLAB (utilizando la instrucción function) 
% que se identifique con la siguiente expresión, correspondiente a la
% integral de Fourier  de un pulso rectangular no periódico:
function [ y ] = fnej1( w )
    y = 5 * .5 * (sin(.5 * w / 2) ./ (.5 * w / 2));
end

