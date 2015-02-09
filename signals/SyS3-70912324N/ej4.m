t = -30:pi/100:30;
T = 100;
f = getFn(t, T, 2000);

% Obtenemos la función F(w) a partir de f(t)
[w, F] = espectro(t, f, T);

% Obtenemos la salida para diversos anchos de banda
for w0 = [2, 2*pi, 100]
    G = F .* fnej3(w, w0);
    [t2, g] = inv_espectro(w, G, T);
    
    % Representación de entrada y salida con respecto al tiempo
    % Se puede comprobar que el pulso sale distorsionado (más pequeño)
    figure(1);
    plot(t2, f);
    hold on
    plot(t2, g, '-r');
    hold off
    
    figure(2);
    plot(w, abs(F)); % Espectro en amplitud de f
    hold on
    plot(w, abs(G), '-r'); % Espectro en amplitud de g
    hold off
    pause
end

% Comentar la influencia del ancho de banda del sistema y de la señal en
% los resultados obtenidos. ¿En qué caso hay más distorsión de la señal y 
% en cuál la distorsión es menor? ¿A qué crees que es debido?

% A mayor ancho de banda, menor distorsión
% Esto es debido a que al aplicar H(w) queda el módulo
% sqrt(1^2 + (w / w0)^2)
% Que es más cercano a uno cuanto más grande sea w0.
% Al ser más cercano a uno la relación G/F, la distorsión es menor