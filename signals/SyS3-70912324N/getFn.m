function [y] = getFn( t, T, n )
    y = 5 * .5 / T;
    for i = 1:n
       w = 2 * pi * i / T;
       y = y + 2/T * 5 * .5 * sin(.5 * w / 2) / (.5 * w / 2) * cos(w * t);
    end
end

