/*
HAL layer ADC implementiona for SOC8200 platform.
The ADC is not built into the chip but is on the main CPU board.
It is an ADS7844, 8 channel, interfaced on SPI1 channel 1 (i.e. CS1 or "module 1").

This HAL layer is written to match the interface written by
Sandip Bapat originally for the STM32F1X series, found at:
DotNet-MF/MicroFrameworkPK_v4_0/Interops/Samraksh

Nathan Stohs
nathan.stohs@samraksh.com
2012-03-14
*/

#include <tinyhal.h>
#include "../TIAM3517.h"

void hal_adc_init(u8 sampTime);
u8 hal_adc_getData(u16 *dataBuf, u8 startChannel, u8 numChannels);


/*
Init the SPI channel for the ADC. Ignore sampTime.
TODO: Less clunky init code?
*/
void hal_adc_init(u8 sampTime) {
	TIAM3517_SPI_Driver::Initialize();
	
	/*
	SPI_CONFIGURATION config;
	
	config.MSK_IDLE = FALSE;		// Default Clock polarity
	config.MSK_SampleEdge = FALSE;	// Default Phase
	config.Clock_RateKHz = 0; 		// 0 for 32768 divider, slow to start
	config.SPI_mod = 1;				// Connected to CS1 line on SPI1
	config.MD_16bits = FALSE;		// 8-bits for now
	config.DeviceCS = NULL;			// Managed internally by SPI module
	config.CS_Setup_uSecs = 0;		// Ignored by SPI driver
	config.CS_Hold_uSecs = 0;		// Ignored by SPI driver
	*/
	
	/* 
	Init step above "should" already do this init by default... so try and skip.
	Uncomment above config and below command if there are issues.
	Note that Xaction_Start in this case doesn't actually start anything, just more config.
	*/
	
	//TIAM3517_SPI_Driver::Xaction_Start( config );
}

/*
Get one sample from startChannel and place in dataBuf.
Uses 24-clock transfers as described in datasheet, but as 3x 8-bit xfers.
This is the most inefficient but its all we've tested so far.

For a simple sample numChannels should be 1.

TODO: Support 15/16 clock conversions. Multi channels. Fix redundancy with init code.
Faster clock.

Return: 0 on fail
*/

u8 hal_adc_getData(u16 *dataBuf, u8 startChannel, u8 numChannels) {
	u8 toADC[3] = {0x87, 0, 0 }; // 0x87 is a special control byte for channel 0.
	u8 fromADC[3] = { 0, 0, 0 };
	u16 t;
	u32 i;
	SPI_XACTION_8 action;
	
	// Sanity check for range
	if (startChannel + numChannels > 8 || startChannel > 7) return 0;
	
	toADC[0] |= (startChannel & 0x7) << 3;	// Modify control byte to set ADC channel.
	
	for(i=0; i<numChannels; i++) {
		
		action.Read8 = fromADC;			// Read buffer
		action.ReadCount = 3;			// Just read everything and sort at the end
		action.ReadStartOffset = 0;		// See above
		action.SPI_mod = 1;				// ADC on SPI1 CS1
		action.Write8 = toADC;			// Write buffer, see ADS7844 docs and above control byte
		action.WriteCount = 3;			// Just write everything
		
		// Do the writes/reads
		TIAM3517_SPI_Driver::Xaction_nWrite8_nRead8( action );
		
		// Data should now be in fromADC, parse and place in dataBuf as 12-bit sample.
		t = (((fromADC[1] & 0x7f) << 8) | fromADC[2]) >> 3;
		
		dataBuf[i] = t;
		toADC[i] += 0x8; // Set control byte to next channel
	}
	
	return 1;
}
