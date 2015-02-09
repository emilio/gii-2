function [ h ] = ej3h2( w, w0 )
    [x, y] = pol2cart(-4 * w, 1);
    h = x + y * sqrt(-1);
end

