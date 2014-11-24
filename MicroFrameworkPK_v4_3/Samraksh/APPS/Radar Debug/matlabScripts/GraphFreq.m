function GraphFreq(Real,Imag, Rate)

N = length(Real);

Comp = Real + i*Imag;
Trans = fft(Comp) / N;

DeltaF = Rate/N;

if mod(N,2) % i.e., is odd
  Mid = (N + 1)/2;
  
  Forward = [[Mid + 1 : N], [1 : Mid]];
  FStep = [[-Mid + 1 : -1], [0 : Mid]];
else
  Mid = N/2;
  Order = [[Mid + 1 : N], [1 : Mid + 1]];
  FStep = [[-Mid : -1], [0 : Mid]];
end;

plot(FStep*DeltaF, abs(Trans(Order)), 'b.-')

xlabel('Freq in Hz');
ylabel('Amplitude in ADC Units')