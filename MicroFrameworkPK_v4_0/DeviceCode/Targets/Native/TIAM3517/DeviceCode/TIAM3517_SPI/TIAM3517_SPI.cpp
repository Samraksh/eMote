////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "../TIAM3517.h"

//--//


/***************************************************************************/

/* 	Init SPI1
	Returns TRUE if init success
*/
BOOL TIAM3517_SPI_Driver::Initialize()
{   
	UINT32 l;
	UINT32 i; // Timeout
	
//  Sanity check
	l = __raw_readl(OMAP3_SPI_REVISION+OMAP3_SPI1_BASE);
	if (l != OMAP3_SPI_REVISION_EXPECTED) // Except 0x21 aka RTL version 2.1
		return FALSE;

//	Reset SPI
	l = __raw_readl(OMAP3_SPI1_BASE+OMAP3_SPI_SYSCONFIG);
	l = l & ~OMAP3_SPI_SYSCONFIG_RESET;
	__raw_writel(l, OMAP3_SPI1_BASE+OMAP3_SPI_SYSCONFIG);

//	Enable Clocks
	l = __raw_readl(OMAP3_SPI_FCLOCK);
	l |= OMAP3_SPI1_FCLOCK_EN;
	__raw_writel(l, OMAP3_SPI_FCLOCK);
	l = __raw_readl(OMAP3_SPI_ICLOCK);
	l |= OMAP3_SPI1_ICLOCK_EN;
	__raw_writel(l, OMAP3_SPI_ICLOCK);
	
	// Disable and clear SPI interrupts
	__raw_writel(0, 			OMAP3_SPI_IRQENABLE+OMAP3_SPI1_BASE); // Disable
	__raw_writel(0x0001777F, 	OMAP3_SPI_IRQSTATUS+OMAP3_SPI1_BASE); // Clear

//	Check for RESETDONE on SPI1, shouldn't need the loop at all
	while (i<10) {
		l = __raw_readl(OMAP3_SPI1_BASE+OMAP3_SPI_SYSSTATUS);
		if (l & OMAP3_SPI_RESETDONE == 1) return TRUE;
		i++;
	}
    return FALSE; // Timeout error
}


void TIAM3517_SPI_Driver::Uninitialize()
{    
    
}

/***************************************************************************/

void TIAM3517_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL TIAM3517_SPI_Driver::nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
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

BOOL TIAM3517_SPI_Driver::nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
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


/* 	Enables SPI channel
	Returns TRUE on success */
BOOL TIAM3517_SPI_Driver::Xaction_Start( const SPI_CONFIGURATION& Configuration )
{

	UINT32 conf_reg, modulctrl_reg, ctrl_reg, t;
	
	// TODO: Ignore Configuration struct until after demo...
	// Force channel 1 (offset 0) and SPI1 module (base reg)
	
	conf_reg 		= OMAP3_SPI_CHAN_ADDR( 0 /*chan*/, OMAP3_SPI1_BASE/*base*/, OMAP3_SPI_CHxCONF);
	modulctrl_reg 	= OMAP3_SPI_ADDR( OMAP3_SPI1_BASE/*base*/, OMAP3_SPI_MODULCTRL);
	ctrl_reg 		= OMAP3_SPI_CHAN_ADDR( 0 /*chan*/, OMAP3_SPI1_BASE/*base*/, OMAP3_SPI_CHxCTRL);
	
	// Setup CHxCONF
	t = __raw_readl(conf_reg);
	t &= ~(OMAP3_SPI_CHxCONF_IS_SLAVE); 	// Input Select, Master Mode
	t &= ~(OMAP3_SPI_CHxCONF_DPE1_SLAVE);
	t |= OMAP3_SPI_CHxCONF_DPE0_MASTER;
	t &= ~(OMAP3_SPI_CHxCONF_TRM_CLR); 		// Clear TRM for TX/RX
	t |= OMAP3_SPI_CHxCONF_TRM_TX; 			// Set for TX only
	t &= ~(OMAP3_SPI_CHxCONF_WL_CLR); 		// Clear then set 8-bit or 16-bit
	t |= OMAP3_SPI_CHxCONF_WL_8BIT; 		// Set for 8-bit
	t |= OMAP3_SPI_CHxCONF_EPOL_H;
	t &= ~(OMAP3_SPI_CHxCONF_POL_L); 		// Polarity
	t &= ~(OMAP3_SPI_CHxCONF_PHA_D); 		// Phase
	t &= ~(OMAP3_SPI_CHxCONF_CLKD_CLR); 	// Clear clock
	t |= OMAP3_SPI_CHxCONF_CLKD_32768; 		// 32768 CLK divider
	__raw_writel(t, conf_reg);
	
	// Setup MODULCTRL
	t = __raw_readl(modulctrl_reg);
	t &= ~(OMAP3_SPI_MODULCTRL_SLAVE);
	__raw_writel(t, modulctrl_reg);
	
	// Enable channel
	// Setup CHxCTRL
	t = __raw_readl(ctrl_reg);
	t |= OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(t, ctrl_reg); // Enable the port
	
	return TRUE;
}


BOOL TIAM3517_SPI_Driver::Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{   
	UINT32 ctrl_reg, t;

	ctrl_reg = OMAP3_SPI_CHAN_ADDR( 0 /*chan*/, OMAP3_SPI1_BASE/*base*/, OMAP3_SPI_CHxCTRL);
	
	t &= ~(OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(t, ctrl_reg); // Disable the port
    return TRUE;
}


BOOL TIAM3517_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    return TRUE;
}

BOOL TIAM3517_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    INT32 i;
    INT32 d;
    UINT8 Data8;
    
	UINT32 write_reg = OMAP3_SPI_CHAN_ADDR(0 /*chan*/, OMAP3_SPI1_BASE, OMAP3_SPI_TXx);
	UINT32 poll_reg  = OMAP3_SPI_CHAN_ADDR(0 /*chan*/, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	UINT32 stat_reg  = OMAP3_SPI_CHAN_ADDR(0 /*chan*/, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	UINT32 read_reg  = OMAP3_SPI_CHAN_ADDR(0 /*chan*/, OMAP3_SPI1_BASE, OMAP3_SPI_RXx);

    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;
	
	UINT32 tempWrite;

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                    { ASSERT(FALSE); return FALSE; }
    if(Write8 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read8 == NULL)) { ASSERT(FALSE); return FALSE; }

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
   
    while(i--)
    {
        // Write
		tempWrite = Write8[0];
		__raw_writel(tempWrite, write_reg);

        while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_TXS_EMPTY) == 0) { ; } 	// Poll for write done
		//while ( (__raw_readl(stat_reg) & OMAP3_SPI_CHxSTAT_RXS_FULL) == 0) { ; } 	// Poll for read done
        
        // reading clears the RBF bit and allows another transfer from the shift register
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

    return TRUE;
}

void TIAM3517_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{

}
