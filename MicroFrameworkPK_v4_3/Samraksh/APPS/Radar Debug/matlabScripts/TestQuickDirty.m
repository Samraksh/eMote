function QuickDirtyTest(FileName)

Lambda = 5.8e9/3e8;
Rate = 128;

[Real,Imag,firmwareRaw] = ReadRadarData(FileName);

%Real([1:20]) = 0;
%Imag([1:20]) = 0;
%firmwareRaw([1:20]) = 0;

Real = Real([100:13000]);
Imag = Imag([100:13000]);
firmwareRaw = firmwareRaw([100:13000]);

Time = [0 : length(firmwareRaw)-1]/Rate;

ImagA = Imag - median(Imag);
RealA = Real - median(Real);
%ImagA = Imag - 1562;
%RealA = Real - 1568;
Rot = atan2(ImagA,RealA);
UnRot = UnWrap(Rot/(2*pi), -0.5,0.5);
unwrapMatlab = UnRot/Lambda/2;

%raw
% median I - 1562
% median Q - 1568
%firmwareRaw = firmwareRaw / 4096;
%UnRotX = UnWrap(firmwareRaw/(2*pi), -0.5,0.5);
%UnRotX = UnWrap(firmwareRaw, -pi,pi);
%UnRotX = firmwareRaw/(2*pi);
%firmwareUnwrap = UnRotX/Lambda/2;


%Range = Max - Min;
DeltaX = diff(firmwareRaw);
UnDeltaX = [0; mod(DeltaX+pi ,2*pi)-pi];
UnPhaseX = cumsum(UnDeltaX);

%DeltaX = diff(firmwareRaw);
%ModX = [0;mod(DeltaX + pi, 2*pi) - pi];
%UnDeltaX = [0; mod(DeltaX + 0.5,1) - 0.5];
%firmwareUnwrap = cumsum(ModX);

%% Make Graph
subplot(3,1,1);
GraphRelRange(RealA,ImagA, Lambda,Rate);
%plot(Time, Real)
axis tight;

%% Make Graph
subplot(3,1,2);
plot(Time, Imag)
plot(Time, Real)
xlabel('matlab arctan');
axis tight;

subplot(3,1,3);
%GraphAbs(Real,Imag, Rate);
%plot(Time, Imag, 'b.-')
plot(Time, firmwareRaw)
xlabel('firmware raw');
axis tight;
