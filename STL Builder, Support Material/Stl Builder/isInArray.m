function [result] = isInArray(E, y, x)
%IS Summary of this function goes here
%   Detailed explanation goes here
[yMax, xMax] = size(E);
if y > 1 && y <= yMax && x > 1 && x <= xMax
    result = 1;
else
    result = 0;
end

end

