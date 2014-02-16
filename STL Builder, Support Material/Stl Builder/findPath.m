function [visit] = findPath(E, visited, y, x, outside)
%FINDPATH 0 is clockwise, 1 is counterclockwise
%   Detailed explanation goes here

% Temp files for STL writing
tempfile1 = '1.temp';
tempfile2 = '2.temp';
p1 = [y x];
tol = 2;
if outside == 0
    if isInArray(y, x+1)
        if visited(y, x + 1) == 0
            visited(y,x+1) = 1;
            if E(y,x+1) == 1
                p2 = [y x+1];
                buildTriangle (p1,p2, tempfile1, tempfile2);
                findPath(E,visited,y, x+1,outside);
            else
                [tmpY, tmpX,visited] = tolerance(E, visited, y,x,tol);
                if tmpY == -1
                    confirmLoop(tempfile1,0,0);
                else
                    p2 = [tmpY tmpX];
                    buildTriangle (p1,p2, tempfile1, tempfile2);
                    findPath(E,visited,y, x+1,outside);
                end
            end
        end
    end
    if isInArray(y+1,x+1)
        if visited(y+1, x+1) == 0
            visited(y+1,x+1) = 0;
            if E(y+1,x+1) == 1
                p2 = [y+1 x+1];
                buildTriangle (p1,p2, tempfile1, tempfile2);
                findPath(E, visited,y+1,x+1,outside);
            else
                [tmpY, tmpX,visited] = tolerance(E, visited, y,x,tol);
                if tmpY == -1
                    confirmLoop(tempfile1,0,0);
                else
                    p2 = [tmpY tmpX];
                    buildTriangle (p1,p2, tempfile1, tempfile2);
                    findPath(E,visited,y, x+1,outside);
                end
            end
        end
    end
    if isInArray(y+1, x)
        if visited(y+1, x) == 0
            visited(y+1,x1) = 0;
            if E(y+1,x1) == 1
                p2 = [y+1 x];
                buildTriangle (p1,p2, tempfile1, tempfile2);
                findPath(E, visited,y+1,x,outside);
            else
                [tmpY, tmpX, visited] = tolerance(E, visited, y,x,tol);
                if tmpY == -1
                    confirmLoop(tempfile1,0,0);
                else
                    p2 = [tmpY tmpX];
                    buildTriangle (p1,p2, tempfile1, tempfile2);
                    findPath(E,visited,y, x+1,outside);
                end
            end
        end
    end
end


visit = visited;

end

