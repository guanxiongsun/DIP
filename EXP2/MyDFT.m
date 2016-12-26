%一维DFT变换矩阵形式
function [X] = MyDFT(x)
    A = CreateA(size(x,1));
    X = A*x;
end

function A = CreateA(m)
    A =zeros(m,m);
    for p =1:m
        for q =1:m
            A(p,q) = exp(-1i*2*pi*(p-1)*(q-1)/m);
        end
    end
end