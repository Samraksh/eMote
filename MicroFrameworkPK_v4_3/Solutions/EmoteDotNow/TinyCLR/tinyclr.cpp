////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <..\math\Include\arm_math.h>

#define TEST_LENGTH_SAMPLES 128

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
uint32_t fftSize = 64;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;
uint32_t refIndex = 213, testIndex = 0;

void fftTest()
{
	arm_status status;
	arm_cfft_radix4_instance_q31 S;
	q31_t maxValue;

	q31_t testOutput_q31[TEST_LENGTH_SAMPLES];

	float32_t testOutput_f32[TEST_LENGTH_SAMPLES];

	q31_t testInput_q31_10khz[TEST_LENGTH_SAMPLES];


	float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES] =
	{
			0.995185, 0.980785, 0.956940, 0.923880, 0.881921, 0.831470, 0.773010, 0.707107, 0.634393, 0.555570, 0.471397, 0.382683, 0.290285, 0.195090, 0.098017, 0.000000, -0.098017, -0.195090, -0.290285, -0.382683, -0.471397, -0.555570, -0.634393, -0.707107, -0.773010, -0.831470, -0.881921, -0.923880, -0.956940, -0.980785, -0.995185, -1.000000, -0.995185, -0.980785, -0.956940, -0.923880, -0.881921, -0.831470, -0.773010, -0.707107, -0.634393, -0.555570, -0.471397, -0.382683, -0.290285, -0.195090, -0.098017, -0.000000, 0.098017, 0.195090, 0.290285, 0.382683, 0.471397, 0.555570, 0.634393, 0.707107, 0.773010, 0.831470, 0.881921, 0.923880, 0.956940, 0.980785, 0.995185, 1.000000, 0.995185, 0.980785, 0.956940, 0.923880, 0.881921, 0.831470, 0.773010, 0.707107, 0.634393, 0.555570, 0.471397, 0.382683, 0.290285, 0.195090, 0.098017, 0.000000, -0.098017, -0.195090, -0.290285, -0.382683, -0.471397, -0.555570, -0.634393, -0.707107, -0.773010, -0.831470, -0.881921, -0.923880, -0.956940, -0.980785, -0.995185, -1.000000, -0.995185, -0.980785, -0.956940, -0.923880, -0.881921, -0.831470, -0.773010, -0.707107, -0.634393, -0.555570, -0.471397, -0.382683, -0.290285, -0.195090, -0.098017, -0.000000, 0.098017, 0.195090, 0.290285, 0.382683, 0.471397, 0.555570, 0.634393, 0.707107, 0.773010, 0.831470, 0.881921, 0.923880, 0.956940, 0.980785, 0.995185, 1.000000

	};

	for(UINT16 i = 0 ; i < TEST_LENGTH_SAMPLES; i++)
	{

		testInput_q31_10khz[i] = 0;
		//testInput_f32_10khz[i] = i;

	}

	for(UINT16 i = 0 ; i < TEST_LENGTH_SAMPLES; i++)
	{
			testOutput_f32[i] = 0;

			testOutput_q31[i] = 0;

	}

	arm_float_to_q31(testInput_f32_10khz,testInput_q31_10khz,TEST_LENGTH_SAMPLES);

	status = ARM_MATH_SUCCESS;

		/* Initialize the CFFT/CIFFT module */
	status = arm_cfft_radix4_init_q31(&S, fftSize,
		  								ifftFlag, doBitReverse);

		/* Process the data through the CFFT/CIFFT module */
	arm_cfft_radix4_q31(&S, testInput_q31_10khz);

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

	arm_q31_to_float(testInput_q31_10khz, testOutput_f32, TEST_LENGTH_SAMPLES);



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
    ClrStartup( clrSettings );
    
	///////// native testing
	//q15_t x = 20;
    //q15_t y = 10;
    //q15_t result = 0;
    //CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
    //fftTest();
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

