function buildTriangle(p1, p2, f1, f2)
%BUILDTRIANGLE builds 2 triangles given 2 points and writes it to f1, f2
%   f1, f2 has opposite normal vectors

%generating 3rd point by shifting p1 by +1 z
p3 = [p1(1:2) 1];

%calculating vectors
v1 = p2 - p1;
v2 = p3 - p1;

%calculating unit normal - V1 X V2
n = [(v1(2)*v2(3)-v1(3)*v2(2)) (v1(1)*v2(3) - v1(3)*v2(1))...
    (v1(1)*v2(2) - v1(2)*v2(1))];

lengthN = sqrt(sum(n.*n));
n = n/lengthN;

%creating triangle structure and writing to files
s = struct('normal', n, 'p1', p1, 'p2', p2, 'p3', p3);
writePoint(s,f1,f2);

%calculating 4th point by shifting p2 by +1 z
p4 = [p2(1:2) 1];

%calculating vectors
v1 = p3 - p4;
v2 = p2 - p4;

%calculating unit normal - V1 X V2
n = [(v1(2)*v2(3)-v1(3)*v2(2)) (v1(1)*v2(3) - v1(3)*v2(1))...
    (v1(1)*v2(2) - v1(2)*v2(1))];
lengthN = sqrt(sum(n.*n));
n = n/lengthN;

%creating triangle structure and writing 2nd triangle to files
s = struct('normal', n, 'p1', p3, 'p2', p4, 'p3', p2);
writePoint(s,f1,f2);

end

