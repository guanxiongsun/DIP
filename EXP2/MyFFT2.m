function X = MyFFT2(x)
    x = Padding(x);
    [m,n] = size(x);
    X = zeros(size(x));
    for p = 1:m
        X(p,:) = MyFFT(x(p,:)')';
    end
    for q = 1:n
        X(:,q) = MyFFT(X(:,q));
    end
end

function X = Padding(x)
    [m,n] = size(x);
    k1 = ceil(log2(m));
    k2 = ceil(log2(n));
    m1 = 2^k1;
    n1 = 2^k2;
    if (m ~= m1)
        X = [x;zeros(m1-m,n)];
    end
    if (n ~= n1)
        X = [X zeros(m1,n1-n)];
    end
end

function X= MyFFT(x)
    N = size(x,1);
    X = zeros(N,1);
    if N == 2
        X(1) = x(1)+x(2);
        X(2) = x(1)-x(2);
    else
        EvenNum = 2*(0:N/2-1);
        OddNum = 2*(1:N/2)-1;
        WNm = zeros(N/2,1);
        for p = 1:N/2
            WNm(p) = exp(-1i*2*pi*(p-1)/N);
        end
        
        A = MyFFT(x(EvenNum+1));
        B = MyFFT(x(OddNum+1));
        
        X = cat(1,A+B.*WNm,A-B.*WNm);
    end
end