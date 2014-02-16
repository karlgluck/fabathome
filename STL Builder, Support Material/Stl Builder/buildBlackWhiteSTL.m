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
function buildBlackWhiteSTL(Im,file1,file2)
    % Log of lines drawn on display
    f1 = [file1,'.stl'];
    f2 = [file2,'.stl'];
    fid = fopen('exp.txt','w');
    
    f1id = fopen(f1,'w');
    fprintf(f1id,['solid ',file1,'\n']);
    fclose(f1id);
    f2id = fopen(f2,'w');
    fprintf(f2id,['solid ',file2,'\n']);
    fclose(f2id);
    
    
    
    % Temp files for STL writing
    tempfile1 = '1.temp';
    tempfile2 = '2.temp';

    % Detect edges in the input image
    E = edge(Im);
    
    % Initialize the displays
    figure(1)
    title('Image with edge detection');
    edge(Im,'sobel',0.4)
    figure(2)
    title('Paths (Green = clockwise, Red = counter-clockwise)');
    legend
    hold on
    
    % Initialize size and the outside flag
    [ymax xmax] = size(Im);
    visited = zeros(ymax,xmax);
    triangles = zeros(ymax,xmax);
    outside = 0;
    
    % Create the border around the entire image
    p1 = [0    , 0   ];
    p2 = [ymax , 0   ];
    p3 = [ymax , xmax];
    p4 = [0    , xmax];
    plot([p1(2),p2(2)],[p1(1),p2(1)],'b');
    plot([p2(2),p3(2)],[p2(1),p3(1)],'b');
    plot([p3(2),p4(2)],[p3(1),p4(1)],'b');
    plot([p4(2),p1(2)],[p4(1),p1(1)],'b');
    buildTriangle([p1(2),p1(1),0],[p2(2),p2(1),0],tempfile1,tempfile2);
    buildTriangle([p2(2),p2(1),0],[p3(2),p3(1),0],tempfile1,tempfile2);
    buildTriangle([p3(2),p3(1),0],[p4(2),p4(1),0],tempfile1,tempfile2);
    buildTriangle([p4(2),p4(1),0],[p1(2),p1(1),0],tempfile1,tempfile2);
    confirmLoop(tempfile1,f1,0);
    confirmLoop(tempfile2,f2,0);
    
    % Mark the border visited
    visited(:,1) = 1;
    visited(1,:) = 1;
    visited(ymax,:) = 1;
    visited(:,xmax) = 1;
    
    
    % Check every interior point
    loopid = 1;
    for y=1:ymax
        for x=1:xmax
            if (~visited(y,x) && E(y,x))
                % This pixel hasn't been visited and belongs to an edge
                p1 = [y,x];
                dir = [0,0];
                
                % Initialize the direction cycles
                cycle = zeros(8,2);
                if (outside)
                    cycle(1,:) = [0,1];
                    cycle(2,:) = [1,1];
                    cycle(3,:) = [1,0];
                    cycle(4,:) = [1,-1];
                    cycle(5,:) = [0,-1];
                    cycle(6,:) = [-1,-1];
                    cycle(7,:) = [-1,0];
                    cycle(8,:) = [-1,1];
                else
                    cycle(1,:) = [1,-1];
                    cycle(2,:) = [1,0];
                    cycle(3,:) = [1,1];
                    cycle(4,:) = [0,1];
                    cycle(5,:) = [-1,1];
                    cycle(6,:) = [-1,0];
                    cycle(7,:) = [-1,-1];
                    cycle(8,:) = [0,-1];
                end
                error = 0;
                
                % Create the temp files for loop storage
                fid2 = fopen(tempfile1, 'w');
                fclose(fid2);
                fid2 = fopen(tempfile2, 'w');
                fclose(fid2);
                
                % Follow this path until it connects with another visited
                % path component
                while(~visited(p1(1),p1(2)))
                    visited(p1(1),p1(2)) = loopid;
                    % Find the direction to travel in, then check in all
                    % directions clockwise if outside and counter clockwise
                    % if inside the object
                    found = 0;
                    
                    for i=1:8
                        if ((sum(dir ~= cycle(i,:))/2) && ...
                                p1(1) + cycle(i,1) > 0 && ...
                                p1(2) + cycle(i,2) > 0 && ...
                                p1(1) + cycle(i,1) < ymax && ...
                                p1(2) + cycle(i,2) < xmax && ...
                                E(p1(1),p1(2)) == ...
                                 E(p1(1) + cycle(i,1),p1(2) + cycle(i,2)))
                            dir = cycle(i,:);
                            found = 1;
                            break;
                        end
                    end
                    
                    if (~found)
                        % Open loop path, discontinue traversal
                        error = 1;
                        break;
                    end
                    p2 = p1 + dir;
                    
                   % Follow the current direction until a point is found
                    % that does not belong to this edge path
                    while (E(p2(1)+dir(1),p2(2)+dir(2)) && ...
                            ~visited(p2(1),p2(2)) && E(p1(1),p1(2)))
                        visited(p2(1),p2(2)) = 1;
                        p2 = p2 + dir;
                    end
                    
                    % p1 and p2 are two vertices that form an edge of the
                    % object, so draw the two triangles that make that edge
                    if (~error)
                        if (outside)
                            plot([p1(2),p2(2)],[p1(1),p2(1)],'r');
                            fprintf(fid,'Draw Outside Line (%i,%i) to (%i,%i) \n',...
                                    p1(2),p1(1),p2(2),p2(1));
                        else
                            plot([p1(2),p2(2)],[p1(1),p2(1)],'g');
                            fprintf(fid,'Draw Inside Line (%i,%i) to (%i,%i) \n',...
                                    p1(2),p1(1),p2(2),p2(1));
                        end
                        buildTriangle([p1(2),p1(1),0],[p2(2),p2(1),0],tempfile1,tempfile2);
                    end
                    p1 = p2;
                end
                fprintf(fid,'Error in loop: %i \n',error);
                confirmLoop(tempfile1,f1,error);
                confirmLoop(tempfile2,f2,error);
                
            end
            
            if (E(y,x) && (x+1 < xmax) && ~E(y,x+1))
                outside = ~outside;
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