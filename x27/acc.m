
clear;
fullsteps2 = 540;
fz(1)=240;
e=exp(1)

for n = 1:fullsteps2
    if n < fullsteps2/2 
        fz(n+1) = fz(n) + e*fz(n)/(n+2)^2;
    elseif n >= fullsteps2/2  	
        fz(n+1) = fz(n) - e*fz(n)/(fullsteps2-n+2)^2;
    end 
end
disp(fz');
plot(fz);

%for n = 1:fullsteps2
%    if n < fullsteps2/2 
%        fz(n+1) = fz(n) + fz(n)/exp(n);
%    elseif n >= fullsteps2/2  	
%        fz(n+1) = fz(n) - fz(n)/exp(fullsteps2-n-1);
%    end 
%end