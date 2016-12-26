function R = bf(u,v)
    N = 8;
    [X,Y] = meshgrid(0:N-1,0:N-1);
    R = Alpha(u)*Alpha(v)*cos((2*X+1)*u*pi/(2*N)).*cos((2*Y+1)*v*pi/(2*N));
end

function x = Alpha(k)
    N = 8;
    if k == 0
        x = sqrt(1/N);
    else
        x = sqrt(2/N);
    end
end