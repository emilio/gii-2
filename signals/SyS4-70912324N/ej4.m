t = -10:pi/100:10;
ws = {[1 2], [2 1], [3 4]}; % f(t) = 1 * sen(2 * t) + 2 * sen(t) + 3 * sen(4 * t)
w0 = 2 * pi;
T = 1;



y = fnej2(ws, t);
[ w, Y ] = espectro(t, y, T);

figure(1);
plot(t, y, '-r') % Sin ruido
figure(2);
plot(w, Y, '-r');


% Ruido aleatorio a 3 niveles distintos
for fn = {'randomNoise', 'randomNoise2', 'randomNoise3'}
    g = fnej2(ws, t, w0, fn{1});
    [ w, G ] = espectro(t, g, T);
    
    figure(1);
    hold on
    plot(t, g);
    figure(2);
    hold on
    plot(w, G);
    
end