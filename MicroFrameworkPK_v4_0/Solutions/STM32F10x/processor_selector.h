////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _STM32F10X_PROCESSOR_SELECTOR_H_
#define _STM32F10X_PROCESSOR_SELECTOR_H_ 1

	/*===================================================
	 *  Processir selector - uncomment desired processor
	 *=================================================*/
	/* #define STM32F10X_LD */     /*!< STM32F10X_LD: STM32 Low density devices */
	/* #define STM32F10X_LD_VL */  /*!< STM32F10X_LD_VL: STM32 Low density Value Line devices */
	/* #define STM32F10X_MD */     /*!< STM32F10X_MD: STM32 Medium density devices */
	/* #define STM32F10X_MD_VL */  /*!< STM32F10X_MD_VL: STM32 Medium density Value Line devices */
	/* #define STM32F10X_HD */    	   /*!< STM32F10X_HD: STM32 High density devices */
	 #define STM32F10X_XL      /*!< STM32F10X_XL: STM32 XL-density devices */
	/* #define STM32F10X_CL */     /*!< STM32F10X_CL: STM32 Connectivity line devices */



	/*===================================================
	 *  DO NOT CHANGE
	 *=================================================*/
	#define PLATFORM_ARM_DEFINED

#endif
