[Header,Info,BMP] = bmpRead('LENA.bmp');

Ycbcr = RGB2YCBCR(BMP);

[Y,~,~] = Seperate(Ycbcr);

%�ֳ�kxk��ͼ������ά����R����
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
%�ֿ��Ϊcell����
c = mat2cell(Y,k*ones(n1,1),k*ones(n2,1));
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

% im1 = fft2(Y);
% im2 = MyDFT2(Y);
% figure;
% imshow(log(abs(im1)),[]);
% figure;
% imshow(log(abs(im2)),[]);