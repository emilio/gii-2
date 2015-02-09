% Distorsion function for exercise 1
function [ h ] = fnej1( w, w0 )
    h = 1 ./ (1 + (w ./ w0) .* sqrt(-1));
end

