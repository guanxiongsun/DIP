function X = MyDCT2(x)
    X = zeros(size(x));
    [m,n] = size(x);
    for p = 1:m
        X (p,:) = MyDCT(x(p,:)')';
    end
    for q = 1:n
        X (:,q) = MyDCT(X(:,q));
    end
end

function X = MyDCT(x)
    N = size(x,1);
    A = CreateA(N);
    X = A*x;
end

function A = CreateA(N)
    A = zeros(N,N);
    for i =1:N
        for j =1:N
            A(i,j) =  cos((2*j-1)*(i-1)*pi/(2*N));
        end
    end
    A = sqrt(2/N)*A;
    A(1,:) = A(1,:)/sqrt(2);
end