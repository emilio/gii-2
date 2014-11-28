function [ h ] = fnej3( w, w0 )
    h = 1 ./ (1 + (w .* sqrt(-1)) ./ w0);
end

