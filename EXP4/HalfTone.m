function X = HalfTone(x, T)
    x = double(x);
    K = [0 0 0 7 5;3 5 7 5 3;1 3 5 3 1]/48;
    [m,n] = size(x);
    [p,q] = size(K);
    x = [zeros(m,2) x zeros(m,2)];
    x = [x;zeros(2,n+4)];
    for i = 1:m
        for j = 3:n+2
            if(x(i,j)>T)
                E = -(255-x(i,j))*K;
                for u = 1:p
                    for v = 1:q
                        x(i+u-1,j+v-3) = x(i+u-1,j+v-3) + E(u,v);
                        x(i,j) = 255;
                    end
                end
            else
                E = x(i,j)*K;
                for u = 1:p
                    for v = 1:q
                        x(i+u-1,j+v-3) = x(i+u-1,j+v-3) + E(u,v);
                        x(i,j) = 0;
                    end
                end
            end
        end
    end
    X = x(1:m,3:n+2);
end