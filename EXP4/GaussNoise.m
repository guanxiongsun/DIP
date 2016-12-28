function x = GaussNoise(x,mu,sigma)
    x = double(x);
    [m,n] = size(x);
    Noise = rand(m,n)*m-m/2;
    Noise = gaussmf(Noise,[sigma,mu])*225;
    x = x + Noise;
end