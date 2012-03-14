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
}

/*
Get one sample from startChannel and place in dataBuf. Ignore numChannels for now.
Uses 24-clock transfers as described in datasheet, but as 3x 8-bit xfers.
This is the most inefficient but its all we've tested so far.

TODO: Support 15/16 clock conversions. Multi channels. Fix redundancy with init code.
Faster clock.

Returns: 1 always
*/

u8 hal_adc_getData(u16 *dataBuf, u8 startChannel, u8 numChannels) {
	u8 toADC[3] = {0x87, 0, 0 }; // 0x87 is a special control byte for only channel one.
	u8 fromADC[3] = { 0, 0, 0 };
	u16 t;
	SPI_CONFIGURATION config;
	SPI_XACTION_8 action;
	
	config.MSK_IDLE = FALSE;		// Default Clock polarity
	config.MSK_SampleEdge = FALSE;	// Default Phase
	config.Clock_RateKHz = 0; 		// 0 for 32768 divider, slow to start
	config.SPI_mod = 1;				// Connected to CS1 line on SPI1
	config.MD_16bits = FALSE;		// 8-bits for now
	config.DeviceCS = NULL;			// Managed internally by SPI module
	config.CS_Setup_uSecs = 0;		// Ignored by SPI driver
	config.CS_Hold_uSecs = 0;		// Ignored by SPI driver
	
	action.Read8 = fromADC;			// Read buffer
	action.ReadCount = 3;			// Just read everything and sort at the end
	action.ReadStartOffset = 0;		// See above
	action.SPI_mod = 1;				// ADC on SPI1 CS1
	action.Write8 = toADC;			// Write buffer, see ADS7844 docs
	action.WriteCount = 3;			// Just write everything
	
	// Configure using above. Note that "start" does not actually start anything,
	// In fact it forces disabled.
	// This step overlaps with init (but init must be called first).
	TIAM3517_SPI_Driver::Xaction_Start( config );
	
	// Do the writes/reads
	TIAM3517_SPI_Driver::Xaction_nWrite8_nRead8( action );
	
	// Data should now be in fromADC, parse and place in dataBuf
	t = (((fromADC[1] & 0x7f) << 8) | fromADC[0]) >> 3;
	
	dataBuf[0] = t;
	
	return 1;
}
