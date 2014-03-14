////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#if 0
#include <tinyhal.h>
//#include "TIAM3517.h"

#define SOC8200_USE_SPI1
//#define SOC8200_USE_SPI2
//#define SOC8200_USE_SPI3
//#define SOC8200_USE_SPI4

//--//

/***************************************************************************/


static UINT32 SPI_CHAN_TO_MOD(UINT32 x) {
	if (x <  4) return OMAP3_SPI1;
	if (x <  6) return OMAP3_SPI2;
	if (x <  8) return OMAP3_SPI3;
	if (x == 8) return OMAP3_SPI4;
	return OMAP3_SPI1;
}

/*
	Returns TRUE if init success
*/
BOOL Initialize()
{   
	UINT32 l; // Temp var
	UINT32 i; // Timeout
	SPI_CONFIGURATION config;
	
	config.MSK_IDLE = FALSE;
	config.MSK_SampleEdge = FALSE;
	config.MD_16bits = FALSE;
	config.Clock_RateKHz = 0; // Should default to 32768 divider
	
//  Sanity check
	l = __raw_readl(OMAP3_SPI_REVISION+OMAP3_SPI1_BASE);
	if (l != OMAP3_SPI_REVISION_EXPECTED) // Except 0x21 aka RTL version 2.1
		return FALSE;

#ifdef SOC8200_USE_SPI1
//	Reset SPI 1
	l = __raw_readl(OMAP3_SPI1_BASE+OMAP3_SPI_SYSCONFIG);
	l &= ~OMAP3_SPI_SYSCONFIG_RESET;
	__raw_writel(l, OMAP3_SPI1_BASE+OMAP3_SPI_SYSCONFIG);
	// Set PADCONFs directly (see TIAM3517 for indirect macros)
	__raw_writel(MY_PADCONF_MCSPI1_CLK,  SPI_CONTROL_PADCONF_MCSPI1_CLK);
	__raw_writel(MY_PADCONF_MCSPI1_SOMI, SPI_CONTROL_PADCONF_MCSPI1_SOMI);
	__raw_writel(MY_PADCONF_MCSPI1_CS1,  SPI_CONTROL_PADCONF_MCSPI1_CS1);
	__raw_writel(MY_PADCONF_MCSPI1_CS3,  SPI_CONTROL_PADCONF_MCSPI1_CS3);
	// All SPI1 pads are now configured
	
	// Need to init 0 and 1 for SOC8200 board.
	config.SPI_mod  = 0;
	Xaction_Start(config);
	
	config.SPI_mod  = 1;
	Xaction_Start(config);
	
	//config.SPI_mod  = 2;
	//Xaction_Start(config);
	
	//config.SPI_mod  = 3;
	//Xaction_Start(config);
#endif
	
#ifdef SOC8200_USE_SPI2
// 	Reset SPI 2
	l = __raw_readl(OMAP3_SPI2_BASE+OMAP3_SPI_SYSCONFIG);
	l &= ~OMAP3_SPI_SYSCONFIG_RESET;
	__raw_writel(l, OMAP3_SPI2_BASE+OMAP3_SPI_SYSCONFIG);
	// TODO set PADCONFs
#endif
	
#ifdef SOC8200_USE_SPI3
// 	Reset SPI 3
	l = __raw_readl(OMAP3_SPI3_BASE+OMAP3_SPI_SYSCONFIG);
	l &= ~OMAP3_SPI_SYSCONFIG_RESET;
	__raw_writel(l, OMAP3_SPI3_BASE+OMAP3_SPI_SYSCONFIG);
	// TODO set PADCONFs
#endif
	
#ifdef SOC8200_USE_SPI4
// 	Reset SPI 4
	l = __raw_readl(OMAP3_SPI4_BASE+OMAP3_SPI_SYSCONFIG);
	l &= ~OMAP3_SPI_SYSCONFIG_RESET;
	__raw_writel(l, OMAP3_SPI4_BASE+OMAP3_SPI_SYSCONFIG);
	// TODO set PADCONFs
#endif

//	Enable Clocks
	l = __raw_readl(OMAP3_SPI_FCLOCK); // F CLOCK
	l |= OMAP3_SPI1_FCLOCK_EN;
	__raw_writel(l, OMAP3_SPI_FCLOCK);
	l = __raw_readl(OMAP3_SPI_ICLOCK); // I CLOCK
	l |= OMAP3_SPI1_ICLOCK_EN;
	__raw_writel(l, OMAP3_SPI_ICLOCK);
	
	// Disable and clear SPI interrupts
	__raw_writel(0, 			OMAP3_SPI_IRQENABLE+OMAP3_SPI1_BASE); // Disable
	__raw_writel(0x0001777F, 	OMAP3_SPI_IRQSTATUS+OMAP3_SPI1_BASE); // Clear
	// SPI INTERRUPTS ARE NOW DISABLED

//	Check for RESETDONE on SPI1, shouldn't actually need to loop
	while (i<10) {
		l = __raw_readl(OMAP3_SPI1_BASE+OMAP3_SPI_SYSSTATUS);
		if (l & OMAP3_SPI_RESETDONE == 1) return TRUE;
		i++;
	}
    return FALSE; // Timeout error
}


void Uninitialize()
{    
    
}

void ISR( void* Param )
{
    //ASSERT(0);
}

BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    if(!Xaction_Start( Configuration )) return FALSE;

    {
        SPI_XACTION_16 Transaction;

        Transaction.Read16          = Read16;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write16         = Write16;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod         = Configuration.SPI_mod;
        
        if(!Xaction_nWrite16_nRead16( Transaction )) return FALSE;
    }

    return Xaction_Stop( Configuration );
}

BOOL nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{   

    if(!Xaction_Start( Configuration )) return FALSE;

    {
        SPI_XACTION_8 Transaction;
     
        Transaction.Read8           = Read8;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write8          = Write8;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod         = Configuration.SPI_mod;
        Transaction.BusyPin.Pin     = GPIO_PIN_NONE;
        
        if(!Xaction_nWrite8_nRead8( Transaction )) return FALSE;
    }

    return Xaction_Stop( Configuration );
}


/* 	
	Setup SPI channel
	After init, channel in disabled state.
	Returns TRUE.
*/
BOOL Xaction_Start( const SPI_CONFIGURATION& Configuration )
{

	UINT32 conf_reg;		// SPI channel configuration register
	UINT32 modulctrl_reg;	// SPI module configuration register
	UINT32 ctrl_reg;		// SPI channel control register
	UINT32 t, n;			// temp
	UINT32 chan,mod;		// channel
	UINT32 mask;			// mask to find clock divider
	UINT32 clock;			// clock rate finder
	UINT32 myclock;
	
	mod = SPI_CHAN_TO_MOD(Configuration.SPI_mod);
	chan = Configuration.SPI_mod;
	
	// Find register set.
	if ( mod == OMAP3_SPI1) { 
	// no change to chan
	conf_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCONF);
	modulctrl_reg 	= OMAP3_SPI_ADDR		(OMAP3_SPI1_BASE, OMAP3_SPI_MODULCTRL);
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI2) { 
	chan = chan - 4;
	conf_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCONF);
	modulctrl_reg 	= OMAP3_SPI_ADDR		(OMAP3_SPI2_BASE, OMAP3_SPI_MODULCTRL);
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI3) { 
	chan = chan - 6;
	conf_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCONF);
	modulctrl_reg 	= OMAP3_SPI_ADDR		(OMAP3_SPI3_BASE, OMAP3_SPI_MODULCTRL);
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI4) { 
	chan = 8;
	conf_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCONF);
	modulctrl_reg 	= OMAP3_SPI_ADDR		(OMAP3_SPI4_BASE, OMAP3_SPI_MODULCTRL);
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	// Setup MODULCTRL
	t = __raw_readl(modulctrl_reg);
	t &= ~(OMAP3_SPI_MODULCTRL_SLAVE);			// Master Mode
	__raw_writel(t, modulctrl_reg);
	
	// Setup CHxCONF
	t = __raw_readl(conf_reg);
	
	// Master mode stuff (no support for slave)
	t &= ~OMAP3_SPI_CHxCONF_IS_SLAVE; 			// Input select
	t &= ~OMAP3_SPI_CHxCONF_DPE1_SLAVE;			// SOMI/SIMO setup
	t |=  OMAP3_SPI_CHxCONF_DPE0_MASTER;		// SOMI/SIMO setup
	
	t &= ~OMAP3_SPI_CHxCONF_TRM_CLR; 			// Clear TX and RX for TX/RX
	t |=  OMAP3_SPI_CHxCONF_TCS_3_5;			// 3.5 clocks before/after CS
	t &= ~OMAP3_SPI_CHxCONF_WL_CLR;				// Clear then set 8-bit or 16-bit
	
	if (Configuration.MD_16bits) 				// True for 16-bits
		t |= OMAP3_SPI_CHxCONF_WL_16BIT;
	else
		t |= OMAP3_SPI_CHxCONF_WL_8BIT;
	
	t |=  OMAP3_SPI_CHxCONF_EPOL_H;				// CS active low
	
	// Polarity
	if (Configuration.MSK_IDLE)
		t |=  OMAP3_SPI_CHxCONF_POL_L;
	else
		t &= ~OMAP3_SPI_CHxCONF_POL_L;
		
	// Phase
	if (Configuration.MSK_SampleEdge)
		t |=  OMAP3_SPI_CHxCONF_PHA_D;
	else
		t &= ~OMAP3_SPI_CHxCONF_PHA_D;

		
	// Loop to find fastest clock divider, 2^N
	// Log base 2 (aka find highest bit and round down).
	// Yes this is the obvious way, O(N), the algorithm indian shed a tear
	
	if (Configuration.Clock_RateKHz == 0) // check for 0.
		myclock = 1;
	else
		myclock = Configuration.Clock_RateKHz;
		
	n = OMAP3_BASE_SPI_CLK_KHZ / myclock; // Number we want to log2
	mask = 0xFFFF8000; // >= 32768
	clock = 0xF;
	while( clock > 0 ) {
		if (n & mask)
			break;
		else {
			clock--;
			mask = mask >> 1;
		}
	}
	
	// Clock rate
	t &= ~OMAP3_SPI_CHxCONF_CLKD_CLR;			// Clear Clock
	t |= clock;									// Use divider found above
	
	// Finally, write to register
	__raw_writel(t, conf_reg);
	
	// Make sure disabled
	t = __raw_readl(ctrl_reg) & ~(OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(t, ctrl_reg); // Disable the port
	
	return TRUE;
}

/*
	Actual "stop" done in the read/write code now, so nothing to do here.
	Returns: True
*/
BOOL Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{   
	return TRUE;
/*
	UINT32 ctrl_reg, t;
	UINT32 chan, mod;
	
	mod = SPI_CHAN_TO_MOD(Configuration.SPI_mod);
	chan = Configuration.SPI_mod;
	
	if ( mod == OMAP3_SPI1) { 
	// no change to chan
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI2) { 
	chan = chan - 4;
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI3) { 
	chan = chan - 6;
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI4) { 
	chan = 8;
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR	(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	t = __raw_readl(ctrl_reg);
	t &= ~OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(t, ctrl_reg); // Disable the port
*/
}

/*
	Perform 16-bit write or read
	Returns FALSE on error
*/
BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    INT32 i;
    INT32 d;
    UINT16 Data16;
	UINT32 chan, mod;
	UINT32 tempWrite;
    
	UINT32 write_reg;
	UINT32 poll_reg;
	UINT32 read_reg;
	UINT32 ctrl_reg;

    UINT16* Write16         = Transaction.Write16;
    INT32   WriteCount      = Transaction.WriteCount;
    UINT16* Read16          = Transaction.Read16;
    INT32   ReadCount       = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;

	mod = SPI_CHAN_TO_MOD(Transaction.SPI_mod);
	chan = Transaction.SPI_mod;
	
	// Find correct register set.
	if ( mod == OMAP3_SPI1) { 
	// no change to chan
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI2) { 
	chan = chan - 4;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI3) { 
	chan = chan - 6;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI4) { 
	chan = 8;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                     { ASSERT(FALSE); return FALSE; }
    if(Write16 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read16 == NULL)) { ASSERT(FALSE); return FALSE; }
	
	
	tempWrite = __raw_readl(ctrl_reg) | OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(tempWrite, ctrl_reg); // Enable the port

    if(ReadCount)
    {
        if((ReadCount + ReadStartOffset) < WriteCount) { ASSERT(FALSE); return FALSE; }   

        i = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount                  ;    // 
    }
    else
    {
        i = WriteCount;
        d = -1;
    }
    
    // we will use WriteCount to move in the Write16 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero
    WriteCount -= 1;
   
    while(i--)
    {
        // Write
		tempWrite = Write16[0];

        while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_TXS_EMPTY) == 0) { ; } 	// Poll for write ready
		__raw_writel(tempWrite, write_reg);
		
		while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_RXS_FULL) == 0)  { ; } 	// Poll for read done
		Data16 = __raw_readl(read_reg) & 0xFFFF;
        
        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write16++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
            Read16[0] = Data16;
            Read16++;
        }
    }
	
	tempWrite = __raw_readl(ctrl_reg) & (~OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(tempWrite, ctrl_reg); // Disable the port
	
    return TRUE;
}

/*
	Perform 8-bit write/read.
	Returns FALSE on error
*/
BOOL Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    INT32 i;
    INT32 d;
    UINT8 Data8;
	UINT32 chan, mod;
	UINT32 tempWrite;
    
	UINT32 write_reg;
	UINT32 poll_reg;
	UINT32 read_reg;
	UINT32 ctrl_reg;

    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;

	mod = SPI_CHAN_TO_MOD(Transaction.SPI_mod);
	chan = Transaction.SPI_mod;
	
	
	// Find correct register set.
	if ( mod == OMAP3_SPI1) { 
	// no change to chan
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI2) { 
	chan = chan - 4;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI3) { 
	chan = chan - 6;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}
	
	if ( mod == OMAP3_SPI4) { 
	chan = 8;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                    { ASSERT(FALSE); return FALSE; }
    if(Write8 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read8 == NULL)) { ASSERT(FALSE); return FALSE; }
	
	
	tempWrite = __raw_readl(ctrl_reg) | OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(tempWrite, ctrl_reg); // Enable the port

    if(ReadCount)
    {
        if((ReadCount + ReadStartOffset) < WriteCount) { ASSERT(FALSE); return FALSE; }   

        i = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount                  ;    // 
    }
    else
    {
        i = WriteCount;
        d = -1;
    }
    
    // we will use WriteCount to move in the Write8 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero
    WriteCount -= 1;
   
    // Do actual writes and reads.
    while(i--)
    {
        // Write
		tempWrite = Write8[0];

        while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_TXS_EMPTY) == 0) { ; } 	// Poll for write ready
		__raw_writel(tempWrite, write_reg);
		
		while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_RXS_FULL) == 0)  { ; } 	// Poll for read done
		Data8 = __raw_readl(read_reg) & 0xFF;
        
        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write8++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
            Read8[0] = Data8;
            Read8++;
        }
    }
	
	tempWrite = __raw_readl(ctrl_reg) & (~OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(tempWrite, ctrl_reg); // Disable the port

    return TRUE;
}

/*
	Not used.
*/
void GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{

}

//#endif
