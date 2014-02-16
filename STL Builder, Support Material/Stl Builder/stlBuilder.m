% Converts a two dimension matrix containing a black and white image to an
% STL formatted file and exports it in two files f1 and f2.
%
% Input:
%        Im - Two dimension matrix containing a black and white bitmap
%        image
%        f1 - file name without extension
%        f2 - file name without extension
%
% TODO Clockwise first step : (0,1) , (1,1) , (1,0)
% TODO Clockwise first step : (1,-1) , (1,0) , (1,1)
% TODO DFS search for path planning
% TODO Invert Im for easier accessing
function stlBuilder(Im,file1,file2)
    % Log of lines drawn on display
    f1 = [file1,'.stl'];
    f2 = [file2,'.stl'];

    f1id = fopen(f1,'w');
    fprintf(f1id,['solid ',file1,'\n']);
    fclose(f1id);
    f2id = fopen(f2,'w');
    fprintf(f2id,['solid ',file2,'\n']);
    fclose(f2id);
    


    % Detect edges in the input image
    E = edge(Im);
    
    % Initialize the displays
    figure(1)
    title('Image with edge detection');
    edge(Im)
    
    % Initialize size and the outside flag
    [YMAX XMAX] = size(Im);
    visited = zeros(YMAX,XMAX);
    outside = 0;
    %Path Traversal Methods
    for i = 1:YMAX 
        for j = 1:XMAX
            if visited(i,p)~= 1
               if E(i,p) == 0
                   visited(i,p) = 1;
               else
                   visited = findPath(E, visited, i, p, outside);
               end
            end
        end
    end
    
    
    
    f1id = fopen(f1,'a');
    fprintf(f1id,'endsolid');
    fclose(f1id);
    f2id = fopen(f2,'a');
    fprintf(f2id,'endsolid');
    fclose(f2id);
    
    fclose('all');

end