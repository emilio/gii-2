% default function for generating functions
function [ h ] = randomNoise2(w, w0)
    h = w + 1 * 2 * randn(size(w));
end