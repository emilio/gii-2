%
% Representar la señal compuesta por dos armónicos dada por la siguiente...
%

A1 = 1;
A2 = 1/3;
w = 2 * pi;
t = [0:.01:5];

y1 = A1 * sin(w*t);
y2 = A2 * sin(w*t);

plot(t, y1, '-b')
hold on
plot(t, y2, '-r')
pause

% T = Ty1 = Ty2
% w = 2 * pi * f => f = 1 => T = 1/f = 1
y = y1 + y2;

plot(t, y, '-g')
plot(t, y + 1, '-r')