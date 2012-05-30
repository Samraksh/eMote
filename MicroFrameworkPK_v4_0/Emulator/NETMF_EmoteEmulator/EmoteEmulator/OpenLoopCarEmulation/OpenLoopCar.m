a=[423 613];
c=[367.74 764.83]
r=161.567;

v=50;

%Determine angular displacement

%theta = pi/180;
theta = v/r
figure();
hold on;
plot (c(1),c(2),'og');
plot(a(1),a(2),'ob');
    for i=1:7
    if(i>1)
        a(1)=x(i-1);
        a(2)=y(i-1);
    end
    %Initial angle from y-axis
    alpha = atan2(a(2)-c(2),a(1)-c(1))

    x(i)=c(1)+r* cos(alpha+theta);
    y(i)=c(2)+r* sin(alpha+theta);
     plot(x(i),y(i),'or');
end

axis ([200 550 600 950])
hold off;

