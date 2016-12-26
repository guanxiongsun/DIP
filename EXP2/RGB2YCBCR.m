function Ycbcr = RGB2YCBCR(X)
    X = double(X);
    A = [0.257 0.564 0.098;
         -0.148 -0.291 0.439;
         0.439 -0.368 -0.071];
    [m,n,~] = size(X);
    Ycbcr = zeros(size(X));
    for i = 1:m
        for j =1:n
            temp = reshape(X(i,j,:),[3,1]);
            temp2 = A*temp + [16;128;128];
            temp = reshape(temp2, [1,1,3]);
            Ycbcr(i,j,:) = temp;
        end
    end
    Ycbcr = uint8(Ycbcr);
end