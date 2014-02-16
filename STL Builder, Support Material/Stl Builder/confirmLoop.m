function confirmLoop(tempFile, file, status)
%CONFIRMLOOP copy the data of tempFile into file if status = 1
%   the function only copies when status is equal to 1, otherwise it simply
%deletes the temporary file.

%reads the content of tempFile
text = fileread (tempFile);

%if tempFile is confirmed, copy data over to file
if status == 1
    fid = fopen(file, 'a');
    fprintf(fid, text);
    fclose(fid);
end

%always delete tempFile
delete(tempFile);

end

