% Fijando el valor de T=1 s y el intervalo de tiempo desde -3 a 3 segundos,
% representar la serie para un número de términos (número de armónicos) de 5,
% 15, 100 y 400, dado que no es posible sumar infinitos términos. Para
% implementar el sumatorio infinito de la serie, se puede utilizar la sentencia for
% de MATLAB. Comentar los resultados obtenidos. NOTA: Para una mejor
% visualización de los resultados, utilizar aquí un incremento en el vector de
% tiempos de pi/1000

t = [-3:pi/1000:3];

repej3(t, 1, 5)
pause
repej3(t, 1, 100)
pause
repej3(t, 1, 400)
pause

% Realizar ahora la representación gráfica para valores de T=1, 5 y 20 s, en el
% intervalo de -50 a 50 segundos, utilizando 400 armónicos. Comprobar a partir
% de las gráficas que el periodo de la señal en cada caso es el correcto. NOTA:
% Para una mejor visualización de los resultados, utilizar en este apartado un
% incremento en el vector de tiempos de pi/100.

t = [-50:pi/100:50];

for T = [1, 5, 20]
    y = repej3(t, T, 400);  
    pause
    [w, f] = espectro(t, y, T);
    plot(w, f, '-b')  
    pause
end


% ¿Cuál es el ancho de los pulsos del tren de pulsos rectangulares en cada caso?
% Siempre es de 0.5s
% ¿Cómo se debería modificar la expresión matemática de f(t) para representar
% un tren de pulsos con ancho de pulso igual a 1? ¿Y para representar un tren de
% pulsos con altura igual a 10?
% Para que el ancho sea igual a uno:
%  * cambiando las ocurrencias de 0.5 por 1 (tao)
% Para que la altura sea igual a 10:
%  * cambiando las ocurrencias de 5 por 10 (v 10)


% A partir de la expresión matemática de la función f(t) , ¿Cuál es el valor de los
% coeficientes cn de la serie de Fourier? ¿Cuál es el valor del coeficiente c0?
%  * Fuente: Tema 2 Parte 1 p. 16: Fórmula trigonométrica del desarrollo de la
% serie de fourier (|cn| = cn ya que son reales)
%    5 * .5 * sin(.5 * w / 2) / (.5 * w / 2)