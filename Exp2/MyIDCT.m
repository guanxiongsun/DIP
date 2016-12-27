function x = MyIDCT(X)
    N = size(X,1);
    A=zeros(N);
    for i=0:N-1
        for j=0:N-1
            if i==0
                a=sqrt(1/N);
            else
                a=sqrt(2/N);
            end            
            A(i+1,j+1)=a*cos(pi*(j+0.5)*i/N); %���ɱ任����
        end
    end
    x=A'*X*A;  %DCT���任�ָ��ľ���
%     A=zeros(size(x,1));
%     for u=0:size(x,1)-1
%         for v=0:size(x,1)-1
%             if u==0
%                 a=sqrt(1/(size(x,1)));
%             else
%                 a=sqrt(2/(size(x,1)));
%             end            
%             A(u+1,v+1)=a*cos(pi*(v+0.5)*u/(2*size(x,1))); %���ɱ任����
%         end
%     end
%     Y=A*x*A'   %DCT�任��ľ���
%     X=A'*Y*A;  %DCT���任�ָ��ľ���
end