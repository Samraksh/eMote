////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _STM32F10X_BOARD_SELECTOR_H_
#define _STM32F10X_BOARD_SELECTOR_H_ 1

	/*===================================================
	 *  Board selector - uncomment desired board
	 *=================================================*/
	//#define USE_STM32100B_EVAL
	//#define USE_STM3210B_EVAL
	//#define USE_STM3210E_EVAL
	//#define USE_STM3210C_EVAL
	//#define USE_CUSTOM_BOARD			//load custom board files to \DeviceCode\board\custom
	#define USE_PRIMER2
	
	/*===================================================
	 *  DO NOT CHANGE
	 *=================================================*/
	 
	#ifdef USE_CUSTOM_BOARD
		#include <custom/custom.h>		//custom board files
	#elif defined USE_PRIMER2
		#include <stm32f10x.h>
	#elif defined USE_STM32100B_EVAL
		#include <stm32_eval/stm32_eval.h>
		#include <stm32_eval/stm32100b_eval/stm32100b_eval.h>
	#elif defined USE_STM3210B_EVAL
		#include <stm32_eval/stm32_eval.h>
		#include <stm32_eval/stm3210b_eval/stm3210b_eval.h>
	#elif defined USE_STM3210E_EVAL
		#include <stm32_eval/stm32_eval.h>
		#include <stm32_eval/stm3210e_eval/stm3210e_eval.h>
	#elif defined USE_STM3210C_EVAL
		#include <stm32_eval/stm32_eval.h>
		#include <stm32_eval/stm3210c_eval/stm3210c_eval.h>
	#endif   

#endif
