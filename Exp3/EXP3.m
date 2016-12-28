Prewitt_h = [1 1 1;0 0 0;-1 -1 -1];
Prewitt_v = [1 0 -1;1 0 -1;1 0 -1];

Sobel_h = [1 2 1;0 0 0;-1 -2 -1];
Sobel_v = [1 0 -1;2 0 -2;1 0 -1];

Roberts_45 = [1 0;0 -1];
Roberts_135= [0 1;-1 0];

Laplacian = [0 -1 0;-1 4 -1;0 -1 0];

im = imread('LENA.BMP');
im = rgb2gray(im);

imshow(MyFilter(im,Prewitt_h),[]);
title('P_h');

figure;
imshow(MyFilter(im,Prewitt_v),[]);
title('P_v');

figure;
imshow(MyFilter(im,Sobel_h),[]);
title('S_h');

figure;
imshow(MyFilter(im,Sobel_v),[]);
title('S_v');

figure;
imshow(MyFilter(im,Roberts_45),[]);
title('R45');

figure;
imshow(MyFilter(im,Roberts_135),[]);
title('R135');

figure;
imshow(MyFilter(im,Laplacian),[]);
title('Laplacian');



