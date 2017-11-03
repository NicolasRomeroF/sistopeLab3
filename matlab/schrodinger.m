N=256
c=1
dt=0.1
dd=2
H0=zeros(N);
TF=1000;
0.4*N
0.6*N

for i = 0.4*N:0.6*N
    for j = 0.4*N:0.6*N
        A=round(i)
        B=round(j)
        H0(A,B)=20;
    end
end
H0
H1=H0;

for i = 2:N-1
    for j = 2:N-1
        H1(i,j)=H0(i,j)+((c^2)/2)*((dt/dd)^2)*(H0(i+1,j)+H0(i-1,j)+H0(i,j-1)+H0(i,j+1)-4*H0(i,j));
    end
end

H=H1;
for t = 0:TF
    for i = 2:N-1
        for j = 2:N-1
            H(i,j)=2*H1(i,j)-H0(i,j)+(c^2)*((dt/dd)^2)*(H1(i+1,j)+H1(i-1,j)+H1(i,j-1)+H1(i,j+1)-4*H1(i,j));
        end
    end
    H0=H1;
    H1=H;
    H;
end

H

imagesc(H); axis('off');axis('square');











