function [Real, Imag, firmwareUnwrap] = ReadRadarData(FileName)

Raw = ReadBin(FileName);

if (mod(length(Raw),3) ~= 0)
  warning('Data did not end of a boundary');
end
N = length(Raw) - mod(length(Raw),3)

R = Raw([1 : 3 : N]);
median(R)
%Real = R - median(R);
Real = R ;

I = Raw([2 : 3 : N]);
median(I)
%Imag = I - median(I);
Imag = I ;

firmwareUnwrap = Raw([3 : 3 : N]);
