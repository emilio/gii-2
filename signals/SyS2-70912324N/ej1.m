% Representar en la misma figura la señal para amplitudes 1 y 2, con
% frecuencia angular w = pi rad/s y fase = 0°.
repej1('-b', 1)
hold on
repej1('-r', 2)
hold off

pause

% Representar en la misma figura la señal para frecuencias angulares de pi y 4 * pi
% rad/s, manteniendo la fase de 0° y la amplitud de 1, e indicar cuánto vale el
% periodo de la señal en cada caso.
repej1('-b', 1) % w = 2 * pi * f => f = 1/2Hz => T = 2s
hold on
repej1('-r', 1, 4 * pi) % w = 2 * pi * f => f = 2Hz => T = 0.5s
hold off

pause

% Representar en la misma figura la señal para frecuencias de 1 Hz y 3 Hz,
% manteniendo constantes la amplitud y la fase.
% w = 2 * pi * f
% w1 = 2 * pi
repej1('-b', 1, 2 * pi)
hold on

% w2 = 6 * pi
repej1('-r', 1, 6 * pi)
hold off
pause


% Representar en la misma figura la señal para fases de 0, pi/2 y pi/3, 
% manteniendo constantes la frecuencia angular de pi rad/s y amplitud de 1.
% ¿Mediante qué función trigonométrica se podría representar también la señal
% con desfase pi/2?
%
% - Mediante el coseno
repej1('-b', 1, pi)
hold on
repej1('-r', 1, pi, pi / 2)
repej1('-g', 1, pi, pi / 3)
hold off

