////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_TSC2046Config"
#endif

SPI_CONFIGURATION g_TSC2046_Config = { 61, false, false, false, false, 125, 1, 1, 0 };
TOUCH_PANEL_CalibrationData g_TSC2046_Calibration_Config = {-256, -192, 390400, 319296, -1984, -1920};
TOUCH_PANEL_SamplingSettings g_TSC2046_Sampling_Settings = {1, 2, 400, FALSE, {50, 200, 150, TOUCH_PANEL_SAMPLE_MS_TO_TICKS(50)}};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
