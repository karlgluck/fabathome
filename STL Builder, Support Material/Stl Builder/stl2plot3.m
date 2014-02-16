% stl2plot3.m v0.5
% Cornell University fab@home
% Jimmy Liu and Krina Sobhina
% Last updated: February 2010


% Open and read the file
% Store array indices for coordinates
% v0.5: ONLY ACCEPTS 13-CHARACTER COORDINATES

fid = fopen('result.stl', 'rt');
if fid == -1
    % error reading file
end
txt = fscanf(fid, '%c');

coords = findstr('vertex', txt) + 7;

% Each coordinate must be 13 characters long (scientific notation):
%        8 characters represent the mantissa (x.xxxxxx);
%        5 characters represent the exponent (e+xxx).
% The number of coordinates to import should be a multiple of 3;
%    every third coordinate is repeated starting with the first.
hold on;
axis equal;
view(3);
for i = 0:length(coords)/3 - 1
    x = zeros(1,4);
    y = zeros(1,4);
    z = zeros(1,4);
    
    for n = 1:3
        xstr = blanks(13);
        ystr = blanks(13);
        zstr = blanks(13);
        for m = 1:13
            xstr(m) = txt(coords(n+i*3) + m -  1);
            ystr(m) = txt(coords(n+i*3) + m + 13);
            zstr(m) = txt(coords(n+i*3) + m + 27);
        end
        x(n) = str2double(xstr);
        y(n) = str2double(ystr);
        z(n) = str2double(zstr);
    end
    x(4) = x(1);
    y(4) = y(1);
    z(4) = z(1);
    
    plot3(x,y,z);
end