[Header,Info,BMP] = bmpRead('LENA.bmp');

Ycbcr = RGB2YCBCR(BMP);

[Y,~,~] = Seperate(Ycbcr);

%分成kxk的图像，用三维数组R保存
k = 8;
[m,n] = size(Y);
n1 = ceil(m/k);
n2 = ceil(n/k);
%padding
if (n~= n2*k)
    Y = [Y,zeros(m,n2*k-n)];
end
if (m~= n1*k)
    Y = [Y;zeros(n1*k-m,n2*k)];
end
%分块存为cell矩阵
c = mat2cell(Y,k*ones(n1,1),k*ones(n2,1));


%DFT2
C = c;
tic;
for p = 1:n1
    for q = 1:n2
%         figure;
%         imshow(c{i,j},[]);
        C{p,q} = MyDFT2(c{p,q});
    end
end
toc;
img=[];
for p=1:n1
    for q=1:n2
        img = cat(2,img,C{p,q});
    end
end
result = [];
for q = 1:n1
    result = cat(1,result,img(:,(q-1)*n2*8+1:q*n2*8));
end
figure;
imshow(log(abs(result)),[]);

%DCT2
C2 = c;
for p = 1:n1
    for q = 1:n2
%         figure;
%         imshow(c{i,j},[]);
         C2{p,q} = MyDCT2(c{p,q});
%        C2{p,q} = dct(c{p,q});
    end
end

%将cell拼成平铺图像
img=[];
for p=1:n1
    for q=1:n2
        img = cat(2,img,C2{p,q});
    end
end
result = [];
for q = 1:n1
    result = cat(1,result,img(:,(q-1)*n2*8+1:q*n2*8));
end
figure;
imshow(log(abs(result)),[]);

%使用DCT复原,x+y<end留下
C3=C2;
ed = 30;
for p = 1:n1
    for q = 1:n2
        mat = C3{p,q};
        for i =1:8
            for j=1:8
                if(i+j>ed)
                    mat(i,j) = 0;
                end
            end
        end
%         C3{p,q} = MyDCT2(mat);   
        C3{p,q} = dct(mat);
    end
end

img=[];
for p=1:n1
    for q=1:n2
        img = cat(2,img,C3{p,q});
    end
end
result = [];
for q = 1:n1
    result = cat(1,result,img(:,(q-1)*n2*8+1:q*n2*8));
end
figure;
imshow(log(abs(result)),[]);

%快速余弦变化
YY = MyFDCT2(Y);

%显示8x8的DCT的基函数
figure;
for u = 0:7
    for v = 0:7
        R = bf(u,v);
        subplot(8,8,(u)*8+v+1);
        imshow(R,[]);
    end
end
% result = [];
% for v = 1:8
%     result = cat(1,result,R(:,(v-1)*64+1:v*64));
% end
% figure;
% imshow(log(abs(result)),[]);

% im1 = fft2(Y);
% im2 = MyDFT2(Y);
% figure;
% imshow(log(abs(im1)),[]);
% figure;
% imshow(log(abs(im2)),[]);