% 1 - Señal de la sesión dos con T = 5
T = 5;

% a) Representar con un intervalo de 60 s usando 400 armónicos
figure(1);
t = -30:pi/100:30;
y = getFn(t, T, 400);
plot(t, y);

pause
% b) Representar espectro en frecuencia
figure(2);
[w, f] = espectro(t, y, T);
plot(w, f);
% Comprueba que la parte imaginaria es casi nula
hold on
plot(w, imag(f), '-r');

% La parte imaginaria es casi nula porque f(t) es un tren de pulsos
% rectangulares (función periódica par), por lo que idealmente en el
% dominio de la frecuencia debe de dar una función real, por las
% propiedades de la transformada de fourier

% Representar la función anterior superpuesta al espectro del apartado b). 
% Como vector de frecuencias que determina los valores de la gráfica, utilizar el 
% proporcionado por la función espectro en el apartado b).
%
% ¿Qué relación hay entre ambos espectros? ¿Cuál de ellos es continuo y por qué?
% 
% 
hold on
f2 = fnej1(w);
plot(w, f2, '-g');
% F2(w) es el contorno de F1(w). F2(w) es un espectro continuo,
% porquela integral de Fourier de un pulso no periódico. 

pause
% Representar los espectros en amplitud |F1(w)| y |F2(w)| superpuestos en la 
% misma figura. ¿Cuál es el ancho de banda de las señales en cada caso? ¿Cuál es 
% la componente de continua?
figure(3);
plot(w, abs(f));
hold on
plot(w, abs(f2), '-g')

% El ancho de banda es aprox 12,5 (el primer corte por cero)
% La componente de continua es es el área bajo la curva que representa a la 
% señal en el dominio del tiempo, es decir, el área del pulso, que coincide con F(0):
% 0.5 * 5 = 2.5


% 2- Invertir los espectros anteriores
[t3, f3] = inv_espectro(w, f, T);
[t4, f4] = inv_espectro(w, f2, 60);

pause;
figure(4);
plot(t3, f3);
hold on
plot(t3, f4, '-g');

% ¿Son coherentes los resultados? ¿Por qué?
% Sí que lo son, como se puede comprobar f3 es muy similar a la función
% inicial f (idealmente serían idénticas), y f4 es un único pulso (no
% periódico por lo tanto) centrado en el origen.

