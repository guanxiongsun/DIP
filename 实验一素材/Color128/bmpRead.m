
function [Header,Info,X] = bmpRead(~)
    filename = 'LENA.bmp';
    fid = fopen(filename);
    %初始化文件头
    Header = InitializeHeader();
    
    %读取文件头信息
    Header.bfType = char(fread(fid, 2, 'uint8')');
    Header.bfSize = fread(fid,1,'uint32');
    fseek(fid,4,'cof');
    Header.bfOffBytes = fread(fid,1,'uint32');
    
    %初始化位图信息头
    Info = InitializeInfo();
    
    %读取信息头
    Info.biSize = fread(fid,1,'uint32');
    Info.biWidth = fread(fid,1,'int32');
    Info.biHeight = fread(fid,1,'int32');
    Info.biPlanes = fread(fid,1,'uint16');
    Info.biBitDepth = fread(fid,1,'uint16');
    Info.biCompress = fread(fid,1,'uint32');
    Info.biImageSize = fread(fid, 1, 'uint32');
    Info.biHorzResolution = fread(fid, 1, 'uint32');
    Info.biVertResolution = fread(fid, 1, 'uint32');
    Info.biClrUsed = fread(fid, 1, 'uint32');
    Info.biImportantClr = fread(fid, 1, 'uint32');
    
    %不使用调色板读取RGB图像数据
    X = RGBRead(fid, Info);
    BMP = X;
    imshow(BMP);
    title('BMP')
    
    YIQ = rgb2yiq(BMP);
    XYZ = rgb2xyz(BMP);
    
    %分解RGB
    [R, G, B] = Seperate(BMP);
    %显示单独分量
    figure;
    imshow(R,[]);
    title('R')
    figure;
    imshow(G,[]);
    title('G')
    figure;
    imshow(B,[]);
    title('B');
    
    [Y,I,Q] = Seperate(YIQ);
    Y = ScaleTo255(Y);
    I = ScaleTo255(I);
    Q = ScaleTo255(Q);
    figure;
    imshow(uint8(Y));
    title('Y');
    figure;
    imshow(uint8(I));
    title('I');
    figure;
    imshow(uint8(Q));
    title('Q');
    
    [X,Y_xyz,Z] = Seperate(XYZ);
    X = ScaleTo255(X);
    Y_xyz = ScaleTo255(Y_xyz);
    Z = ScaleTo255(Z);
    figure;
    imshow(uint8(X));
    title('X');
    figure;
    imshow(uint8(Y));
    title('Y_xyz');
    figure;
    imshow(uint8(Z));
    title('Z');
    
    [H,S,I] = rgb2hsi(BMP);
    H = ScaleTo255(H);
    S = ScaleTo255(S);
    I = ScaleTo255(I);
    figure;
    imshow(uint8(H));
    title('H');
    figure;
    imshow(uint8(S));
    title('S');
    figure;
    imshow(uint8(I));
    title('I');
end

%保存各个分量
function [R, G, B] = Seperate(X)
    width = size(X,1);
    height = size(X,2);
    R = zeros(width,height);
    G = zeros(width,height);
    B = zeros(width,height);
    
    for i = 1:width
        for j = 1:height
            R(i,j) = X(i,j,1);
            G(i,j) = X(i,j,2);
            B(i,j) = X(i,j,3);
        end
    end
end

function YIQ = rgb2yiq(X)
    X = double(X);
    A = [0.299 0.587 0.114;
         0.596 -0.274 -0.322;
         0.211 -0.523 0.312];
    [m,n,~] = size(X);
    YIQ = zeros(size(X));
    for i = 1:m
        for j =1:n
            temp = reshape(X(i,j,:),[3,1]);
            temp2 = A*temp;
            temp = reshape(temp2, [1,1,3]);
            YIQ(i,j,:) = temp;
        end
    end
end

function [H,S,I] = rgb2hsi(X)
    [R,G,B] = Seperate(X);
    R = R/255;
    G = G/255;
    B = B/255;
    I = 1/3*(R+G+B);
    S = 1-3./(R+G+B).*min(min(R,G),B);
    theta = acos((((R-G)+(R-B))/2)/((R-G).^2+((R-B).*(G-B)).^0.5));
    H = theta;
    H(B>G) = 2*pi - H(B>G);
    H = H/(2*pi);
end

%将单独分量归一到0到255
function Y = ScaleTo255(X)
    b = max(max(X));
    a = min(min(X));
    Y = (255/(b-a))*(X-a);
end

function XYZ = rgb2xyz(X)
    X = double(X);
    A = [0.49 0.31 0.2;
         0.177 0.813 0.011;
         0 0.01 0.9];
    [m,n,k] = size(X);
    XYZ = zeros(size(X));
    for i = 1:m
        for j =1:n
            temp = reshape(X(i,j,:),[3,1]);
            temp = A*temp;
            temp = reshape(temp, [1,1,3]);
            XYZ(i,j,:) = temp;
        end
    end
end

function [X] = RGBRead(fid, Info)
    w = Info.biWidth;
    h = Info.biHeight;
    % NOTE: BMP files are stored so that scanlines use a multiple of 4 bytes.
    scanlineLength = 4 * ceil((3 * w) / 4);
    numSamples = scanlineLength * abs(h);
    Data = fread(fid,numSamples,'uint8=>uint8');
    if (h >= 0)
        Data = rot90(reshape(Data, scanlineLength, abs(h)));
    else
        Data = reshape(Data, scanlineLength, abs(h))';
    end

    if (w ~= scanlineLength/3)
        Data = Data(:, 1:(3 * w));
    end

    X(1:abs(h), 1:w, 3) = Data(:,1:3:end);
    X(:, :, 2) = Data(:,2:3:end);
    X(:, :, 1) = Data(:,3:3:end);
end

function [Header] = InitializeHeader()
    Header.bfType = '';
    Header.bfSize = [];
    Header.bfReserved1 = 0;
    Header.bfReserved2 = 0;
    Header.bfOffBytes = [];
end

function [Info] = InitializeInfo()
    Info.biSize = [];
    Info.biWidth = [];
    Info.biHeight = [];
    Info.biPlanes = [];
    Info.biBitDepth = [];
    Info.biCompress = [];
    Info.biImageSize = [];
    Info.biHorzResolution = [];
    Info.biVertResolution = [];
    Info.biClrUsed = [];
    Info.biImportantClr = [];
end