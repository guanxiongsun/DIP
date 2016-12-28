function x = SaltNoise(x,snr)
    %x原始图像，snr是噪声占比,0代表无噪声，<1代表白噪声，>1代表黑噪声
    [m,n] = size(x);
    Noise = rand(m,n);
    Select = (Noise < snr);
    Noise1 = Noise.*Select;
    Select = (Noise1 > snr/2);
    Noise2 = Noise1+Select;
    for i=1:m
        for j=1:n
            if(Noise2(i,j)>1)
                x(i,j) = 255;
            elseif(Noise2(i,j)<=1&&Noise2(i,j)>0)
                x(i,j) = 0;
            end    
        end
    end
end