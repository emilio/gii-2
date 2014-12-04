% default function for generating functions
function [ h ] = randomNoise(w, w0)
    h = w + 1 * randn(size(w));
end