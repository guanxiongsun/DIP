function Exp5
%检测角点大致剪裁出纸张位置大小
im = rgb2gray(imread('sample.jpg'));
im_new = rectcrop(im);

%图像锐化


%分块做二值化，因为整体的灰度差异可能很小，分局部处理会提升效果。
[m,n] = size(im_new);
half1 = im_new(1:m/3,:);
half2 = im_new(m/3+1:2*m/3,:);
half3 = im_new(2*m/3+1:m-200,:);
B1 = OSTU(half1);
[B2,t2] = OSTU(half2);
[B3,t3] = OSTU(half3);

imshow(half1,[]);
figure;
imshow(half2,[]);

%合并二值化的图像

%噪声处理，图像平滑（先膨胀后腐蚀）

%按行对图像分割

%按列对行分割

%统计字符数。
end
function im_new = rectcrop(im)
    points = detectHarrisFeatures(im);
    location = points.Location;
    num = size(location,1);
%     summ = sum(location,2);
%     pmin = find(summ == min(summ));
%     pmax = find(summ == max(summ));
    Lcorner = [location(1,:);location(2,:)];
    Rcorner = [location(num-1,:);location(num,:)];
    X = [Lcorner(:,1);Rcorner(:,1)];
    Y = [Lcorner(:,2);Rcorner(:,2)];
    X = sort(X);
    Y = sort(Y);
    im_new = imcrop(im,[X(2)+10 Y(2)+10 X(3)-X(2)-10 Y(3)-Y(2)-10]);
end