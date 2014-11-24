function [Real, Imag] = ReadRadarDataMedian(FileName,Median)

Raw = ReadBin(FileName);

N = length(Raw);
if (mod(N,2) ~= 0)
  warning('Data did not end of a boundary');
end

R = Raw([1 : 2 : N]);
Real = R - Median;

I = Raw([2 : 2 : N]);
Imag = I - Median;
