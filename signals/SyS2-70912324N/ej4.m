%
% La representación del espectro está en el archivo practica1ej3.m
%

% Indicar cuál es el ancho de banda de la señal en cada uno de los casos (T=1, 5 y
% 20 s)
% Hay que tener en cuenta que la frecuencia en las gráficas está en rad/s

% B = 1 / tao Hz = 2 * pi / tao rad/s
% Para T = 1s:
%   Éste es el más difícil de ver: el pulso dos es el que se anula y ronda
%   los 12.5 rad/s
%   B = 1 / 0.5 = 2Hz = 4 * pi rad/s ~= 12.5 rad/s
% Para T = 5s y T = 20s:
%   Igual, al depender sólo del ancho del tren de pulsos, que no varía.

% Explicar qué sucede al variar el valor de T. ¿Cuál es la separación en frecuencia
% entre dos armónicos consecutivos, para cada uno de los casos? ¿Cuál es la
% relación entre dicha separación y el valor de T?
%
% La separación en frecuencias disminuye según se va aumentando el periodo.
% Para T = 1s se anula el armónico c2, ergo la separación en frecuencia
% será 12.5 / 2 rad/s ~= 6.25 rad/s
% Para T = 5s se anula el armónico c10, por lo que la separación en
% frecuencia será: 12.5 / 10 rad/s ~= 1.25 rad/s
% para t = 20s podemos deducir por tanto que se anulará el armónico c40,
% siendo la separación en frecuencia: 12.5 / 40 ~= .325 rad
%
% Nota: Tras el razonamiento anterior y preguntar resulta que la separación
% en frecuencia es 2 * pi / T rad/s (La frecuencia del armónico fundamental
% como tal (1/THz) pasada a radianes / s), lo cual encaja con mis cálculos.

% Indicar si en alguno de los tres casos hay algún armónico que se anula.
% Sí, como ya he dicho se anulan los armónicos c2, c10 y c40 por lo menos
% (respectivamente para T = 1, 5 y 20s)

% A la vista de los espectros obtenidos, ¿cuál es el nivel de continua (armónico
% cero) de las señales temporales?
%
% Nivel de continua = V * tao = 5 * 0.5 = 2.5 V