% default function for generating functions
function [ h ] = randomNoise3(w, w0)
    h = w + 3 * randn(size(w));
end