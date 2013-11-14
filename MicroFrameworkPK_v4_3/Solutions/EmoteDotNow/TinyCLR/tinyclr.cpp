////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <..\math\Include\arm_math.h>

#define TEST_LENGTH_SAMPLES 512

extern "C"
{
void arm_mult_q15(
  q15_t * pSrcA,
  q15_t * pSrcB,
  q15_t * pDst,
  uint32_t blockSize);

void arm_add_q15(
  q15_t * pSrcA,
  q15_t * pSrcB,
  q15_t * pDst,
  uint32_t blockSize);


void arm_cfft_radix4_q31(
  const arm_cfft_radix4_instance_q31 * S,
  q31_t * pSrc);

void arm_cmplx_mag_q31(
  q31_t * pSrc,
  q31_t * pDst,
  uint32_t numSamples);

void arm_max_q31(
  q31_t * pSrc,
  uint32_t blockSize,
  q31_t * pResult,
  uint32_t * pIndex);

void arm_bitreversal_q31(
  q31_t * pSrc,
  uint32_t fftLen,
  uint16_t bitRevFactor,
  uint16_t * pBitRevTable);

arm_status arm_sqrt_q31(
  q31_t in,
  q31_t * pOut);

void arm_float_to_q31(
  float32_t * pSrc,
  q31_t * pDst,
  uint32_t blockSize);



}
uint32_t fftSize = 256;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;
uint32_t refIndex = 213, testIndex = 0;

void fftTest()
{
	arm_status status;
	arm_cfft_radix4_instance_q15 S;
	q15_t maxValue;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);

	q15_t testOutput_q15[TEST_LENGTH_SAMPLES];

	q15_t testInput_q15[TEST_LENGTH_SAMPLES] =
	{
			9239, 3824, 7073, 7068, 3832, 9236, 7, 9999, -3817, 9242, -7062, 7079, -9233, 3839, -9999, 15, -9245, -3810, -7085, -7056, -3847, -9230, -23, -9999, 3802, -9248, 7051, -7090, 9227, -3854, 9999, -31, 9251, 3795, 7096, 7045, 3861, 9224, 39, 9999, -3788, 9254, -7040, 7101, -9221, 3869, -9999, 47, -9257, -3780, -7107, -7034, -3876, -9218, -55, -9999, 3773, -9260, 7028, -7113, 9215, -3883, 9999, -63, 9263, 3766, 7118, 7023, 3891, 9211, 71, 9999, -3758, 9266, -7017, 7124, -9208, 3898, -9999, 79, -9269, -3751, -7129, -7011, -3905, -9205, -87, -9999, 3743, -9272, 7006, -7135, 9202, -3913, 9999, -95, 9275, 3736, 7141, 7000, 3920, 9199, 103, 9999, -3729, 9278, -6994, 7146, -9196, 3927, -9999, 111, -9281, -3721, -7152, -6988, -3935, -9193, -119, -9999, 3714, -9284, 6983, -7157, 9190, -3942, 9999, -127, 9287, 3706, 7163, 6977, 3949, 9186, 135, 9999, -3699, 9290, -6971, 7168, -9183, 3957, -9998, 143, -9293, -3692, -7174, -6966, -3964, -9180, -151, -9998, 3684, -9296, 6960, -7180, 9177, -3971, 9998, -159, 9299, 3677, 7185, 6954, 3978, 9174, 167, 9998, -3669, 9302, -6948, 7191, -9171, 3986, -9998, 175, -9305, -3662, -7196, -6943, -3993, -9167, -183, -9998, 3655, -9308, 6937, -7202, 9164, -4000, 9998, -191, 9310, 3647, 7207, 6931, 4008, 9161, 199, 9998, -3640, 9313, -6926, 7213, -9158, 4015, -9997, 207, -9316, -3632, -7218, -6920, -4022, -9155, -214, -9997, 3625, -9319, 6914, -7224, 9151, -4030, 9997, -222, 9322, 3618, 7229, 6908, 4037, 9148, 230, 9997, -3610, 9325, -6903, 7235, -9145, 4044, -9997, 238, -9328, -3603, -7240, -6897, -4051, -9142, -246, -9996, 3595, -9331, 6891, -7246, 9139, -4059, 9996, -254, 9334, 3588, 7251, 6885, 4066, 9135, 262, 9996, -3580, 9336, -6879, 7257, -9132, 4073, -9996, 270, -9339, -3573, -7262, -6874, -4080, -9129, -278, -9996, 3566, -9342, 6868, -7268, 9126, -4088, 9995, -286, 9345, 3558, 7273, 6862, 4095, 9122, 294, 9995, -3551, 9348, -6856, 7279, -9119, 4102, -9995, 302, -9351, -3543, -7284, -6850, -4110, -9116, -310, -9995, 3536, -9353, 6845, -7289, 9113, -4117, 9994, -318, 9356, 3528, 7295, 6839, 4124, 9109, 326, 9994, -3521, 9359, -6833, 7300, -9106, 4131, -9994, 334, -9362, -3513, -7306, -6827, -4139, -9103, -342, -9994, 3506, -9365, 6821, -7311, 9099, -4146, 9993, -350, 9367, 3498, 7317, 6816, 4153, 9096, 358, 9993, -3491, 9370, -6810, 7322, -9093, 4160, -9993, 366, -9373, -3484, -7327, -6804, -4168, -9089, -374, -9992, 3476, -9376, 6798, -7333, 9086, -4175, 9992, -382, 9378, 3469, 7338, 6792, 4182, 9083, 390, 9992, -3461, 9381, -6786, 7344, -9079, 4189, -9992, 398, -9384, -3454, -7349, -6781, -4196, -9076, -406, -9991, 3446, -9387, 6775, -7355, 9073, -4204, 9991, -413, 9389, 3439, 7360, 6769, 4211, 9069, 421, 9991, -3431, 9392, -6763, 7365, -9066, 4218, -9990, 429, -9395, -3424, -7371, -6757, -4225, -9063, -437, -9990, 3416, -9398, 6751, -7376, 9059, -4233, 9990, -445, 9400, 3409, 7381, 6745, 4240, 9056, 453, 9989, -3401, 9403, -6739, 7387, -9053, 4247, -9989, 461, -9406, -3394, -7392, -6734, -4254, -9049, -469, -9988, 3386, -9409, 6728, -7398, 9046, -4261, 9988, -477, 9411, 3379, 7403, 6722, 4269, 9042, 485, 9988, -3371, 9414, -6716, 7408, -9039, 4276, -9987, 493, -9417, -3364, -7414, -6710, -4283, -9036, -501, -9987, 3356, -9419, 6704, -7419, 9032, -4290, 9987, -509
	};




	//arm_float_to_q31(testInput_f32_10khz,testInput_q31_10khz,TEST_LENGTH_SAMPLES);

	status = ARM_MATH_SUCCESS;

		/* Initialize the CFFT/CIFFT module */
	status = arm_cfft_radix4_init_q15(&S, fftSize,
		  								ifftFlag, doBitReverse);

	while(TRUE)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
		/* Process the data through the CFFT/CIFFT module */
		arm_cfft_radix4_q15(&S, testInput_q15);
		CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	}

#if 0
	for(UINT16 i = 0 ; i < TEST_LENGTH_SAMPLES; i++)
	{
		testInput_q31_10khz[i] = (testInput_q31_10khz[i] << 4);
	}
#endif
		/* Process the data through the Complex Magnitude Module for
		calculating the magnitude at each bin */
	//arm_cmplx_mag_q31(testInput_q31_10khz, testOutput_q31,
	//	  				fftSize);

		/* Calculates maxValue and returns corresponding BIN value */
	//arm_max_q31(testOutput_q31, fftSize, &maxValue, &testIndex);

	//arm_q31_to_float(testInput_q31_10khz, testOutput_f32, TEST_LENGTH_SAMPLES);



	if(testIndex !=  refIndex)
	{
		status = ARM_MATH_TEST_FAILURE;
	}
}

void rfftTest()
{
	arm_rfft_instance_q31 S;
	arm_cfft_radix4_instance_q31 S_CFFT;

	q31_t testOutput_q31_10khz[TEST_LENGTH_SAMPLES];

	q31_t testInput_q31_10khz[TEST_LENGTH_SAMPLES];

	float32_t testOutput_f32[TEST_LENGTH_SAMPLES];

	arm_cfft_radix4_init_q31(&S_CFFT, fftSize,
			  								ifftFlag, doBitReverse);

	arm_rfft_init_q31(&S, &S_CFFT,fftSize,
				ifftFlag, doBitReverse);

#if 0
	float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES] =
	{
			0.04166667,     0.1111111,      -0.004975124,   0.02325581,     0.004878049,    -0.00625,       0.004048583,    0.03030303,     0.01030928,     0.006622517,    0.009009009,    0.004237288,    -0.004424779,   0.02222222,     0.1111111,      0.2,    0.1,    0.008130081,    0.00952381,     0.01785714,     0.007092198,    0.01136364,     0.004854369,    0.005464481,    0.006329114,    0.004545454,    0.008849557,    0.015625,       -0.00591716,    0.007692308,    0.05,   0.006369427

	};
#endif

	float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES] =
	{
			-0.005208333,   0.005319149,    0.01818182,     0.05263158,     0.005208333,    0.004166667,    0.04166667,     0.008695652,    0.03846154,     0.006329114,    0.01052632,     0.005181347,    0.025,  0.004716981,    0.01851852,     0.03846154,     0.006666667,    0.005747126,    0.01785714,     0.003952569,    0.007936508,    0.004149378,    0.008849557,    -0.005464481,   0.00462963,     0.1666667,      0.003968254,    0.008695652,    0.005649718,    0.0106383,      0.006802721,    0.006451613,    0.005813953,    0.02564103,     0.004310345,    0.01219512,     0.01298701,     0.008,  0.1428571,      -0.004739337,   0.01234568,     0.004201681,    0.1428571,      0.005050505,    0.005780347,    0.005263158,    0.02083333,     0.004716981,    0.03125,        0.02439024,     0.00952381,     0.009090909,    0.004255319,    0.005347594,    0.01754386,     0.005208333,    0.006097561,    0.004524887,    0.01075269,     0.005882353,    0.009803922,    0.01369863,     0.01470588,     0.01098901,     -0.01010101,    0.007692308,    0.005154639,    0.02173913,     0.07142857,     0.07692308,     0.01190476,     0.006060606,    0.02564103,     -0.03448276,    0.004149378,    0.004716981,    0.006711409,    0.004545454,    0.008474576,    0.006896552,    0.03225806,     0.01020408,     0.01298701,     0.3333333,      0.006622517,    0.1428571,      0.006666667,    0.01428571,     0.00617284,     0.004587156,    0.008695652,    0.007042253,    0.008403362,    0.02,   0.005586592,    0.03703704,     0.007407407,    0.009174312,    0.006329114,    0.006993007,    0.004081632,    -0.004739337,   0.005494506,    0.004329004,    0.01428571,     0.007936508,    0.006896552,    0.004347826,    0.005747126,    0.008474576,    0.005076142,    0.01030928,     0.0212766,      0.005291005,    0.005681818,    0.005050505,    0.004545454,    0.01149425,     0.03333334,     0.004201681,    0.03448276,     0.004201681,    0.006993007,    0.0625, 0.004901961,    0.009433962,    0.01538462, 0.009615385
	};

	for(UINT16 i = 0 ; i < TEST_LENGTH_SAMPLES; i++)
	{
		testOutput_q31_10khz[i] = 0;
		testOutput_f32[i] = 0;
	}

	arm_float_to_q31(testInput_f32_10khz,testInput_q31_10khz,TEST_LENGTH_SAMPLES);

	arm_rfft_q31(&S,  testInput_q31_10khz, testOutput_q31_10khz);

	arm_q31_to_float(testOutput_q31_10khz, testOutput_f32, TEST_LENGTH_SAMPLES);

	for(UINT32 i = 0 ;i < TEST_LENGTH_SAMPLES; i++)
	{
		hal_printf("%f", testOutput_f32[i]);
	}


}

void powerTest()
{
	float32_t x = 0.035;
	q15_t y = 0;
	q63_t result = 0;

	q31_t resultq31 = 0;

	float32_t resultInFloat = 0;

	arm_float_to_q15(&x, &y, 1);

	arm_power_q15(&y, 1, &result);

	resultq31 = clip_q63_to_q31(result);

	arm_q31_to_float(&resultq31, &resultInFloat, 1);

	hal_printf("%f", resultInFloat);

}

////////////////////////////////////////////////////////////////////////////////
void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);
    CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, FALSE);

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, FALSE);

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;
#if 0
    CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
    UINT8 k = 0;

    while(true)
    {
    	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
    	for(UINT8 i = 0; i < 255; i++)
    		k = i;
    	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
    }
#endif
    //ClrStartup( clrSettings );
    q15_t x = 20;
    q15_t y = 10;
    q15_t result = 0;



    fftTest();
    //powerTest();
    //rfftTest();

#if 0

    while(TRUE)
    {
    	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
    	arm_add_q15(&x, &y, &result, 1);
    	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
    }
    hal_printf("Result = %d \n", result);
    hal_printf("Result = %d\n", result);
    hal_printf("Result = %d\n", result);
#endif


#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

