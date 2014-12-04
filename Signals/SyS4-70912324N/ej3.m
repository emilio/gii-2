t = -5:pi/100:5;
ws = {[1 2], [2 1], [3 4]}; % f(t) = 1 * sen(2 * t) + 2 * sen(t) + 3 * sen(4 * t)
w0 = 2 * pi;

figure(1);

% Sin distorsionar
y = fnej2(ws, t, w0, 'noDistorsionFn');
plot(t, y);

% Retardo
hold on
g = fnej2(ws, t, w0, 'ej3h1');
plot(t, g, '-r');

% Distorsión de retardo
hold on
g = fnej2(ws, t, w0, 'ej3h2');
plot(t, g, '-g');


% Individualmente
pause

% Cada armónico y su suma
figure(3)

%armónicos sin distorsionar
y1_nodist = fnej2(ws(1), t);
y2_nodist = fnej2(ws(2), t);
y3_nodist = fnej2(ws(3), t);

for fn = {'ej3h1', 'ej3h2'}
    clf

    y = fnej2(ws, t, w0, fn{1});
    y1 = fnej2(ws(1), t, w0, fn{1});
    y2 = fnej2(ws(2), t, w0, fn{1});
    y3 = fnej2(ws(3), t, w0, fn{1});
    
    % pintamos el normal y el distorsionado
    plot(t, y1);
    hold on
    plot(t, y1_nodist, '-y');
    hold on
    
    plot(t, y2);
    hold on
    plot(t, y2_nodist, '-y');
    hold on
    
    plot(t, y3);
    hold on
    plot(t, y3_nodist, '-y');
    hold on
    
    % pintamos la onda distorsionada
    % Esta no debería verse, ya que debería superponerse la verde, por el
    % teorema de respuesta lineal
    plot(t, y, '-r');
    hold on
    plot(t, y1 + y2 + y3, '-g');
    
    pause
end
