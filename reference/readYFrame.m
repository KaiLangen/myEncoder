function [Y U V] = readYFrame(fid,frameNum,sqtype)
    if sqtype == 'qcif'
        Y_BLOCK_SIZE = 144 * 176;
        U_BLOCK_SIZE = Y_BLOCK_SIZE/4;
        V_BLOCK_SIZE = U_BLOCK_SIZE;
        MAX_LUM = 255;
        fseek(fid, frameNum * (Y_BLOCK_SIZE+U_BLOCK_SIZE+V_BLOCK_SIZE),'bof');
        Y = fread(fid,Y_BLOCK_SIZE,'uchar');
        U = fread(fid,U_BLOCK_SIZE,'uchar');
        V = fread(fid,V_BLOCK_SIZE,'uchar');
        Y = reshape(Y,176,144)';
        
    elseif sqtype == 'cif'
        Y_BLOCK_SIZE = 288 * 352;
        U_BLOCK_SIZE = Y_BLOCK_SIZE/4;
        V_BLOCK_SIZE = U_BLOCK_SIZE;
        MAX_LUM = 255;
        fseek(fid, frameNum * (Y_BLOCK_SIZE+U_BLOCK_SIZE+V_BLOCK_SIZE),'bof');
        Y = fread(fid,Y_BLOCK_SIZE,'uchar');
        U = fread(fid,U_BLOCK_SIZE,'uchar');
        V = fread(fid,V_BLOCK_SIZE,'uchar');
        Y = reshape(Y,352,288)';
    end
end