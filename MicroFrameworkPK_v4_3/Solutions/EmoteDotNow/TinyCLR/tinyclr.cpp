////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <..\math\Include\arm_math.h>

#define TEST_LENGTH_SAMPLES 32

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
uint32_t fftSize = 16;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
uint32_t refIndex = 213, testIndex = 0;

void fftTest()
{
	arm_status status;
	arm_cfft_radix4_instance_q31 S;
	q31_t maxValue;

	q31_t testOutput_q31[TEST_LENGTH_SAMPLES/2];

	float32_t testOutput_f32[TEST_LENGTH_SAMPLES/2];

	q31_t testInput_q31_10khz[TEST_LENGTH_SAMPLES];

	float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES] =
	{
				0.04166667,     0.1111111,      -0.004975124,   0.02325581,     0.004878049,    -0.00625,       0.004048583,    0.03030303,     0.01030928,     0.006622517,    0.009009009,    0.004237288,    -0.004424779,   0.02222222,     0.1111111,      0.2,    0.1,    0.008130081,    0.00952381,     0.01785714,     0.007092198,    0.01136364,     0.004854369,    0.005464481,    0.006329114,    0.004545454,    0.008849557,    0.015625,       -0.00591716,    0.007692308,    0.05,   0.006369427

	};

	for(UINT16 i = 0 ; i < TEST_LENGTH_SAMPLES; i++)
	{

		testInput_q31_10khz[i] = 0;
		//testInput_f32_10khz[i] = i;

	}

	for(UINT16 i = 0 ; i < (TEST_LENGTH_SAMPLES / 2); i++)
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


		/* Process the data through the Complex Magnitude Module for
		calculating the magnitude at each bin */
	arm_cmplx_mag_q31(testInput_q31_10khz, testOutput_q31,
		  				fftSize);

		/* Calculates maxValue and returns corresponding BIN value */
	//arm_max_q31(testOutput_q31, fftSize, &maxValue, &testIndex);

	arm_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);



	if(testIndex !=  refIndex)
	{
		status = ARM_MATH_TEST_FAILURE;
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
    //ClrStartup( clrSettings );
    q15_t x = 20;
    q15_t y = 10;
    q15_t result = 0;

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);

    //fftTest();
    powerTest();

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

