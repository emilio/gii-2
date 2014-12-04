% ws = array of diferent w's, such as: [2, 4, 7] for a 3-armonic expression
function [ y ] = fnej2(ws, t, w0, hfn)
    y = 0;
    
    if nargin < 4
        hfn = 'noDistorsionFn';
    end
    
    if nargin < 3
        w0 = 0;
    end
    
    if not(iscell(ws))
        ws = {ws};
    end

    for i = 1:numel(ws)
        w = ws{i};
        h = feval(hfn, w(2), w0);
        y = y + w(1) * abs(h) * sin(3 * t + angle(h));
    end
end

% f(t) = 2 * sen(3t) + 4
