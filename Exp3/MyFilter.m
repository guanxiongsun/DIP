function y = MyFilter(x, f)
    if nargin < 3
        shape = 'same';
    end
    if ~isa(x,'float')
        x = double(x);
    end
    if ~isa(f,'float')
        f = double(f);
    end

    stencil = rot90(f,2);

    if isvector(stencil) || numel(stencil) > numel(x)
        y = conv2(x,stencil,shape);
    else
        separable = false;
        try %#ok<TRYNC>
            [u,s,v] = svd(stencil,'econ'); % Check rank (separability) of stencil
            if s(2,2) <= length(stencil)*eps(s(1,1)) %only need to check if rank > 1
                separable = true;
            end
        end
        if separable
            % Separable stencil
            hcol = u(:,1) * sqrt(s(1));
            hrow = conj(v(:,1)) * sqrt(s(1));
            y = conv2(hcol, hrow, x, shape);
            if all(round(stencil(:)) == stencil(:)) && all(round(x(:)) == x(:))
                % Output should be integer
                y = round(y);
            end
        else
            % Nonseparable stencil
            y = conv2(x,stencil,shape);
        end
    end
end