% Representar t = Asen(wt + f)
% rep(A, w, t, f)
function [] = repej1(style, A, w, f, t)
    if ( nargin < 1 )
        style = '-b';
    end
    if ( nargin < 2 )
        A = 1;
    end
    if ( nargin < 3 )
        w = pi;
    end
    if ( nargin < 4 )
        f = 0;
    end
    if ( nargin < 5 )
        t = [1:.01:5];
    end
    
    y = A * sin(w * t + f);
    
    plot(t, y, style)
end
