%������������һ��0��255
function Y = ScaleTo255(X)
    b = max(max(X));
    a = min(min(X));
    Y = (255/(b-a))*(X-a);
end