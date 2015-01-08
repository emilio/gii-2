BITS_PER_SEC = 1;
TIME_INTERVAL = 0.01;
PULSE_DURATION = 1 / BITS_PER_SEC;
NRZ_MIN = -5;
NRZ_MAX = 5;

W0 = [2, 2*pi, 100];

% Ejercicio uno. Codificar al menos 20 bits generados aleatoriamente
data = binario(20);

% Cogemos el número de bits, para calcular el vector t
bits = length(data);
seconds = bits / BITS_PER_SEC;
t = 0:TIME_INTERVAL:seconds;


% Codificamos la senal
encoded = 0;
current_bit = 0;
for i = data
    amplitude = NRZ_MIN;
    if i
        amplitude = NRZ_MAX;
    end
    % current_bit * PULSE_DURATION = tiempo en el que inicia el pulso
    % current_bit * PULSE_DURATION + PULSE_DURATION = tiempo en el que se
    % inicia más tiempo que dura, es decir: tiempo en el que finaliza
    encoded = encoded + pulso(t, amplitude, current_bit * PULSE_DURATION, current_bit * PULSE_DURATION + PULSE_DURATION);
    current_bit = current_bit + 1;
end

fig = figure;
set(fig,'name', 'Sesión 6 ej 1', 'numbertitle','off')
plot(t, encoded);
title('Señal codificada');


[w, F] = espectro(t, encoded, 1);

for w0 = W0
    fig = figure;
    set(fig,'name', sprintf('Sesión 6 ej 2 (w0 = %d)', w0), 'numbertitle', 'off');
    
    % imprimimos la senal original en frecuencia
    s(1) = subplot(2, 2, 1);
    plot(w, abs(F));
    title(s(1), 'Espectro en frecuencia de la señal original');

    % y en tiempo
    s(2) = subplot(2, 2, 2);
    plot(t, encoded);
    title(s(2), 'Señal original');
    
    % La pasamos por el sistema
    H = .9 ./ (1 + w * sqrt(-1) ./ w0);
    G = H .* F;

    % imprimimos el espectro en frecuencia
    s(3) = subplot(2, 2, 3);   
    plot(w, abs(G), '-r');
    title(s(3), sprintf('Recibida w0 = %.2f', w0));
    
    % y lo invertimos para imprimirlo en funcion del tiempo
    [~, g] = inv_espectro(w, G, 1);
    s(4) = subplot(2, 2, 4);
    plot(t, g, '-r');
    title(s(4), sprintf('Recibida w0 = %.2f', w0));
    
    % Los datos decodificados serán una cantidad de ceros igual al número
    % de bits: último tiempo (t(size(t))) por los bits por segundo
    % decoded_data = zeros(1, t(size(t)) * BITS_PER_SEC);
    decoded_data = zeros(1, bits);
    % El intervalo de tiempo que hay entre bits
    bit_interval = 1 / TIME_INTERVAL;
    
    % decodificamos la sennal: de 0 a length - 1
    for i = drange(0:(bits - 1))
        time_val = i * PULSE_DURATION + PULSE_DURATION / 2;
        index = time_val / TIME_INTERVAL;
        if g(index) > NRZ_MAX - (NRZ_MAX - NRZ_MIN) / 2
            decoded_data(i + 1) = 1;
        else
            decoded_data(i + 1) = 0;
        end
    end
    
    % imprimir la señal original a la consola
    data
    % imprimir los bits decodificados a la consola
    decoded_data
end