#include <tinyhal.h>
#include <Samraksh\Acoustic.h>
#include <Samraksh\Algorithms.h>

#include "Comp.h"
#include "IntAttrib.h"
#include "ButterFly.h"
#include "FFT.h"
#include "TwiddleWalk.h"

//#include "..\..\..\Targets\Native\STM32F10x\DeviceCode\math\arm_math.h"
#include <..\math\Include\arm_math.h>


#define ADC_SAMPLE_RATE 4000
#define CHUNK_DIVISOR 4
#define CHUNK_SIZE  ADC_SAMPLE_RATE/CHUNK_DIVISOR
#define FFT_SIZE    1024
#define FFT_ln2OfN    10
#define DFREQ    ADC_SAMPLE_RATE/FFT_SIZE

INT8 testFunction(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm){
	int i;

	for (i=0; i<length; i++){
		buffer[i] = buffer[i] * 2;
	}

	if (historyUpdateControl == true){
		processingOutput[0] = 10;
		processingOutput[1] = 10;
		processingOutput[2] = 10;
		processingOutput[3] = 10;
		processingOutput[4] = 10;
		processingOutput[5] = 10;
	} else {
		processingOutput[0] = 1;
		processingOutput[1] = 1;
		processingOutput[2] = 2;
		processingOutput[3] = 3;
		processingOutput[4] = 5;
		processingOutput[5] = 8;
	}

	return TRUE;
}

bool AcousticInitialized = false;

FftCompFixLenT<Int16T, ButterFlyAccT<Int16T>, BitWalkT<Int16T> >* AcousticFft = NULL;

UINT16 buffer_calc[FFT_SIZE];
UINT16 buffer_dest[FFT_SIZE];

INT8 DetectionCalculationNative(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm)
{
    INT8 fRet = 0;
    INT32 shift;
    if(AcousticInitialized == false) {
        UINT32 ln2OfN = FFT_ln2OfN;
        AcousticFft = new FftCompFixLenT<Int16T,ButterFlyAccT<Int16T>,BitWalkT<Int16T> >((UINT32)FFT_ln2OfN);
    }
    for(int itr=0; itr < length; itr+=length)
    {
        memcpy(&(buffer[itr]),buffer_calc, CHUNK_SIZE*sizeof(buffer[0]) );
        memset(&(buffer_calc[CHUNK_SIZE]), 0, FFT_SIZE - CHUNK_SIZE);

        //TODO: real fft or stride data + subtract mean.
        //shift = AcousticFft->operator()(reinterpret_cast< CompT<Int16T>* >(buffer));

        uint8_t ifftFlag = 0;
        uint8_t doBitReverse = 1;
        amr_rfft_instance_q15 S;
        arm_rfft_init_q15(&S, FFT_SIZE, ifftFlag, doBitReverse);
        arm_rfft_q15(&S, buffer_calc, buffer_dest);

        //get a map of frequency bins to desired frequencies.
        //pass desired frequencies to the median line fit.

/*

        dfreq = Fs/NFFT;                         % Frequency increment
        xdft = fft(data,NFFT);
        xdft = xdft(1:floor(NFFT/2)+1);
        psdx = 1/Fs * 1/NFFT *abs(xdft).^2;
        psdx(2:end-1) = 2*psdx(2:end-1);        % total Energy at this freq
        freq = 0:dfreq:Fs/2;                    % frequency axis
        %% Truncate data between 50 and 200 Hz
si = ( (freq>freq_limits(1)) .* (freq<freq_limits(2)) )==1;
X = 10*log10(freq(si));
Y = 10*log10(psdx(si)/dfreq);
%% Find Median line fit
        [m, Yc, Xc] = MedianLineFit_iterative(X,Y);
function [ varargout ] = MedianLineFit_iterative( X,Y,NumIter,PlotIterations )
%MedianLineFit  This function implements the median line fit
%  Y_i=m?(X_i-X_c )+Y_c+?_i
%% Input
if ( (nargin< 3) || (~exist('NumIter','var')) ), NumIter = 5; end
if ( (nargin< 4) || (~exist('PlotIterations','var')) ), PlotIterations = 0; end
%% Initialization
Xc = nanmedian(X);
mc = 0;
Yc = MedSlope( X,Y,Xc,mc ); % Or alternatively nanmedian(Y);

if PlotIterations
    h_fig1 = figure();
    h_axes1 = gca;
    hold on;
    h_fig2 = figure();
    h_axes2 = gca;
    hold on;
end
for iter = 1:NumIter
    mc = MedSlope( X,Y,Xc,Yc );
    Yc = MedHeight( X,Y,Xc,mc );
    if PlotIterations
        plot(h_axes1, iter, mc, '.');
        plot(h_axes2, iter, Yc, '.');
    end
end
%% Output
if nargout>=1, varargout{1} = mc; end
if nargout>=2, varargout{2} = Yc; end
if nargout>=3, varargout{3} = Xc; end
end

*/
    }



}
