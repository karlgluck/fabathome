function [a,b] = tolerance(E, visited, y, x, tol)
%TOLERANCE Summary of this function goes here
%   Detailed explanation goes here
yMin = y-1;
yMax = y+1;
xMin = x-1;
xMax = x+1;
a = -1;
b = -1;

if tol >= 1
    for i = yMin:yMax
        for p = xMin:xMax
            if visited(i,p) == 0
                if E(i,p) == 1
                    a = i;
                    b = p;
                else
                    [a,b] = tolerance(E, visited, i, p, tol-1);
                end
            end
        end
    end
    
end

