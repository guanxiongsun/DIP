%保存各个分量
function [R, G, B] = Seperate(X)
    width = size(X,1);
    height = size(X,2);
    R = zeros(width,height);
    G = zeros(width,height);
    B = zeros(width,height);
    
    for i = 1:width
        for j = 1:height
            R(i,j) = X(i,j,1);
            G(i,j) = X(i,j,2);
            B(i,j) = X(i,j,3);
        end
    end
end
