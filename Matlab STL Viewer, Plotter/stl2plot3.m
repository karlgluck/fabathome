function stl2plot3(filein)
% stl2plot3.m v3.3
% Cornell University fab@home
% Jimmy Liu and Karina Sobhani
% Last updated: 3/13/2010


% Open and read the file
% Store array indices for coordinates
% v1.0: Accepts variable-length coordinates
% v3.0: Autodetects and accepts binary STL files
% v3.1: Opens new figure window

if ~exist(filein,'file')
        error(['File ''%s'' not found. If the file is not on MATLAB''s path' ...
               ', be sure to specify the full path to the file.'], filein);
end

figure;
hold on;
axis equal;
view(3);

% Determine if an STL file is binary or ASCII

% Open file as ASCII
fid = fopen(filein, 'rt');
if fid == -1
    % error reading file
end
txt = fscanf(fid, '%c');
fclose(fid);

if isempty(txt) || length(txt) < 16
    error('MATLAB:stlread:incorrectFormat', ...
          'File does not appear to be an ASCII or binary STL file.');
end

% Read final 16 characters of file
% Look for the string 'endsolid' near the end of the file
i2  = length(txt);
i1  = i2 - 15;

k = strfind(lower(txt(i1:i2)), 'endsolid');

if ~isempty(k)

    % ==ASCII==
    coords = [strfind(txt, 'vertex') + 7, length(txt)];

    x = zeros(1,4);
    y = zeros(1,4);
    z = zeros(1,4);

    for i = 0:(length(coords)-1)/3 - 1
        for n = 1:3
            linei = txt(coords(n+i*3):coords(n+i*3+1));
            pt = sscanf(linei, '%f %f %f');
            x(n) = pt(1);
            y(n) = pt(2);
            z(n) = pt(3);        
        end

        x(4) = x(1);
        y(4) = y(1);
        z(4) = z(1);

        plot3(x,y,z);
    end

else
    % ==BINARY==
    fid = fopen(filein,'r');    
    if ~isempty(ferror(fid))
        error(lasterror); %#ok
    end

    M = fread(fid,inf,'uint8=>uint8');
    fclose(fid);

    F = [];
    V = [];
    N = [];

    if length(M) < 84
        error('MATLAB:stlread:incorrectFormat', ...
            'Incomplete header information in binary STL file.');
    end

    % Bytes 81-84 are an unsigned 32-bit integer specifying the number of faces
    % that follow.
    numFaces = typecast(M(81:84),'uint32');
    %numFaces = double(numFaces);
    if numFaces == 0
        warning('MATLAB:stlread:nodata','No data in STL file.');
        return
    end

    T = M(85:end);
    F = NaN(numFaces,3);
    V = NaN(3*numFaces,3);
    N = NaN(numFaces,3);

    numRead = 0;
    while numRead < numFaces
        % Each facet is 50 bytes
        %  - Three single precision values specifying the face normal vector
        %  - Three single precision values specifying the first vertex (XYZ)
        %  - Three single precision values specifying the second vertex (XYZ)
        %  - Three single precision values specifying the third vertex (XYZ)
        %  - Two unused bytes
        i1    = 50 * numRead + 1;
        i2    = i1 + 50 - 1;
        facet = T(i1:i2)';

        n  = typecast(facet(1:12),'single');
        v1 = typecast(facet(13:24),'single');
        v2 = typecast(facet(25:36),'single');
        v3 = typecast(facet(37:48),'single');

        n = double(n);
        v = double([v1; v2; v3]);

        % Figure out where to fit these new vertices, and the face, in the
        % larger F and V collections.        
        fInd  = numRead + 1;        
        vInd1 = 3 * (fInd - 1) + 1;
        vInd2 = vInd1 + 3 - 1;

        V(vInd1:vInd2,:) = v;
        F(fInd,:)        = vInd1:vInd2;
        N(fInd,:)        = n;

        numRead = numRead + 1;
    end

    x = zeros(1,4);
    y = zeros(1,4);
    z = zeros(1,4);

    for i = 1:length(V)
        for n = 1:3
            x(n) = V(i,1);
            y(n) = V(i,2);
            z(n) = V(i,3);
        end

        x(4) = x(1);
        y(4) = y(1);
        z(4) = z(1);

        plot3(x,y,z);
    end
end

hold off;

end