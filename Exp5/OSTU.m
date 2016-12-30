function [im,T] = OSTU(im)
    gmax = 0;
    Tmax = 0;
    for T=1:255
        g = cal(im,T);
        if g > gmax
            Tmax = T;
            gmax = g;
        end
    end
    T = Tmax;
    im = (im>T);
end

function g = cal(im, T)
    [m,n] = size(im);
    back = (im < T);
    front = ~back;
    u = sum(im(:))/(m*n);
    N0 = sum(back(:));
    N1 = m*n-N0;
    w0 = N0/(m*n);
    w1 = 1-w0;
    u0 = sum(im(back))/N0;
    u1 = sum(im(front))/N1;
    g = w0*w1*(u0-u1)^2;
end