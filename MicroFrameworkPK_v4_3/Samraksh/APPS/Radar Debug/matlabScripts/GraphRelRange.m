function GraphRelRange(Real,Imag, Lambda,Rate)

Rot = atan2(Imag,Real);
UnRot = UnWrap(Rot/(2*pi), -0.5,0.5);
RelRange = UnRot/Lambda/2;

N = length(Real);
Time = [0 : N-1]/Rate;

plot(Time,RelRange);

xlabel('Time in Seconds');
ylabel('Relitive Range in Meeters')
