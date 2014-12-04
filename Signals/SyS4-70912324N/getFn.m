% Función de la sesión dos distorsionada opcionalmente por un sistema (w0, fn)
% Fn debe ser una string(http://www.mathworks.com/matlabcentral/newsreader/view_thread/17539)
function [y] = getFn( t, T, n, w0, fn )
    y = 5 * .5 / T;

    if nargin < 4
        w0 = 0; % Doesn't matter
    end
    if nargin < 5
        fn = 'noDistorsionFn';
    end

    for i = 1:n
       w = 2 * pi * i / T;
       h = feval(fn, w, w0);
       y = y + 2/T * 5 * .5 * abs(h) * sin(.5 * w / 2) / ((.5 * w) / 2) * cos((w * t) + angle(h));
    end
end