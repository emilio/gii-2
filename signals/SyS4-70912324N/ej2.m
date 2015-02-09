t = -30:pi/100:30;
ws = {[1 2], [2 1], [3 4]}; % f(t) = 1 * sen(2 * t) + 2 * sen(t) + 3 * sen(4 * t)
w0 = 2 * pi;

figure(1);

% Sin distorsionar
y = fnej2(ws, t);
plot(t, y);

% Atenuación
hold on
g = fnej2(ws, t, w0, 'ej2h1');
plot(t, g, '-r');

% Distorsión de atenuación
hold on
g = fnej2(ws, t, w0, 'ej2h2');
plot(t, g, '-g');

pause

% indiviadualmente
figure(2)
for w = ws
    clf
    y = fnej2(w, t);
    plot(t, y);
    
    hold on
    g = fnej2(w, t, w0, 'ej2h1');
    plot(t, g, '-r');
    
    hold on
    g = fnej2(w, t, w0, 'ej2h2');
    plot(t, g, '-g');
    pause
end

% Cada armónico y su suma
figure(3)
for fn = {'noDistorsionFn', 'ej2h1', 'ej2h2'}
    clf

    y = fnej2(ws, t, w0, fn{1});
    y1 = fnej2(ws(1), t, w0, fn{1});
    y2 = fnej2(ws(2), t, w0, fn{1});
    y3 = fnej2(ws(3), t, w0, fn{1});
    
    plot(t, y1);
    hold on
    plot(t, y2);
    hold on
    plot(t, y3);
    hold on
    % Esta no debería verse, ya que debería superponerse la verde, por el
    % teorema de respuesta lineal
    plot(t, y, '-r');
    hold on
    plot(t, y1 + y2 + y3, '-g');
    
    pause
end
    