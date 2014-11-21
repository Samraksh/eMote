function GraphAbs(Real,Imag, Rate)

Abs = sqrt(Real .^ 2 + Imag .^ 2);

N = length(Real);
Time = [0 : N-1]/Rate;

plot(Time,Abs);

xlabel('Time in Seconds');
ylabel('Amplitude in ADC Units')