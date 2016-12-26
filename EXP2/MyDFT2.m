%¶þÎ¬DFT±ä»»
function [X] = MyDFT2(x)
    [m,n] = size(x);
    X = zeros(m,n);
    for u =1:m
        X(u,:) = MyDFT(x(u,:)')';
    end
    for v =1:n
        X(:,v) = MyDFT(X(:,v));
    end
end
