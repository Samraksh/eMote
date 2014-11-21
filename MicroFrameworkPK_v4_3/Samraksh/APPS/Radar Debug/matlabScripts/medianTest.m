function medianTest(FileNameBase)
FileName = [FileNameBase, '.data'];

Lambda = 5.8e9/3e8;
Rate = 256;

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
axis tight;

subplot(10,1,2);
GraphRelRange(Real2,Imag2, Lambda,Rate);
axis tight;

subplot(10,1,3);
GraphRelRange(Real3,Imag3, Lambda,Rate);
axis tight;

subplot(10,1,4);
GraphRelRange(Real4,Imag4, Lambda,Rate);
axis tight;

subplot(10,1,5);
GraphRelRange(Real5,Imag5, Lambda,Rate);
axis tight;

subplot(10,1,6);
GraphRelRange(Real6,Imag6, Lambda,Rate);
axis tight;

subplot(10,1,7);
GraphRelRange(Real7,Imag7, Lambda,Rate);
axis tight;

subplot(10,1,8);
GraphRelRange(Real8,Imag8, Lambda,Rate);
axis tight;

subplot(10,1,9);
GraphRelRange(Real9,Imag9, Lambda,Rate);
axis tight;

subplot(10,1,10);
GraphRelRange(Real10,Imag10, Lambda,Rate);
axis tight;
