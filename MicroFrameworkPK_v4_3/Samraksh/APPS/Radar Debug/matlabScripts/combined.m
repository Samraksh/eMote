function combined(FileNameBase)
FileName = [FileNameBase, '.data'];
Lambda = 5.8e9/3e8;
Rate = 256;
sampIntervalMilli = 3906; 
IQformat = 0;
startTimeSec = 0;
endTimeSec = 0;
cleaner = 'M';
xlimVal = 55;
%[Real,Imag] = ReadRadarData(FileName);
[RealF,ImagF] = ReadProcessData(FileName, sampIntervalMilli, IQformat, startTimeSec, endTimeSec, '1');
[Real,Imag] = ReadProcessData(FileName, sampIntervalMilli, IQformat, startTimeSec, endTimeSec, 'M');

figure(1);
%% Make Graph
subplot(4,1,1);
GraphRelRange(Real,Imag, Lambda,Rate);
ylim ([-1 3.5]);
xlim ([0 xlimVal]);
%axis tight;

subplot(4,1,2);
GraphRelRange(RealF,ImagF, Lambda,Rate);
%axis tight;
ylim ([-5 5])
xlim ([0 xlimVal]);

subplot(4,1,3);
GraphAbs(Real,Imag, Rate);
%axis tight;
ylim ([0 500])
xlim ([0 xlimVal]);

subplot(4,1,4);
GraphFreq(Real,Imag, Rate);
%axis tight;
ylim ([0 5])

figure(2);
[Real1,Imag1] = ReadRadarDataMedian(FileName,0);
[Real2,Imag2] = ReadRadarDataMedian(FileName,500);
[Real3,Imag3] = ReadRadarDataMedian(FileName,1000);
[Real4,Imag4] = ReadRadarDataMedian(FileName,1500);
[Real5,Imag5] = ReadRadarDataMedian(FileName,2000);
[Real6,Imag6] = ReadRadarDataMedian(FileName,2048);
[Real7,Imag7] = ReadRadarDataMedian(FileName,2072);
[Real8,Imag8] = ReadRadarDataMedian(FileName,2500);
[Real9,Imag9] = ReadRadarDataMedian(FileName,3000);
[Real10,Imag10] = ReadRadarDataMedian(FileName,3500);

%[Real1,Imag1] = ReadRadarDataMedian(FileName,1750);
%[Real2,Imag2] = ReadRadarDataMedian(FileName,1800);
%[Real3,Imag3] = ReadRadarDataMedian(FileName,1850);
%[Real4,Imag4] = ReadRadarDataMedian(FileName,1900);
%[Real5,Imag5] = ReadRadarDataMedian(FileName,1950);

%% Make Graph

subplot(10,1,1);
GraphRelRange(Real1,Imag1, Lambda,Rate);
%axis tight;
ylim ([-10e-3 10e-3])
xlim ([0 xlimVal]);

subplot(10,1,2);
GraphRelRange(Real2,Imag2, Lambda,Rate);
%axis tight;
ylim ([-10e-3 10e-3])
xlim ([0 xlimVal]);

subplot(10,1,3);
GraphRelRange(Real3,Imag3, Lambda,Rate);
%axis tight;
ylim ([-10e-3 10e-3])
xlim ([0 xlimVal]);

subplot(10,1,4);
GraphRelRange(Real4,Imag4, Lambda,Rate);
%axis tight;
ylim ([-.1 .3])
xlim ([0 xlimVal]);

subplot(10,1,5);
GraphRelRange(Real5,Imag5, Lambda,Rate);
%axis tight;
ylim ([-2 2])
xlim ([0 xlimVal]);

subplot(10,1,6);
GraphRelRange(Real6,Imag6, Lambda,Rate);
%axis tight;
ylim ([-2 2])
xlim ([0 xlimVal]);

subplot(10,1,7);
GraphRelRange(Real7,Imag7, Lambda,Rate);
%axis tight;
ylim ([-2 2])
xlim ([0 xlimVal]);

subplot(10,1,8);
GraphRelRange(Real8,Imag8, Lambda,Rate);
%axis tight;
ylim ([-.1 .3])
xlim ([0 xlimVal]);

subplot(10,1,9);
GraphRelRange(Real9,Imag9, Lambda,Rate);
%axis tight;
ylim ([-10e-3 10e-3])
xlim ([0 xlimVal]);

subplot(10,1,10);
GraphRelRange(Real10,Imag10, Lambda,Rate);
%axis tight;
ylim ([-10e-3 10e-3])
xlim ([0 xlimVal]);
