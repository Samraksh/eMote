/*  Name : MSM8960_SPI.h
 *
 *  Author : Ananth.Muralidharan@Samraksh.info
 *
 *  Description : The header for SPI.
 *
 *  History :
 *
 */


#include <tinyhal.h>
#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\Krait\DeviceCode\krait.h>
//#include "../TIAM3517.h"

#define DEBUG_SPI

////Below taken from platform/msm_shared/include/gsbi.h
/* GSBI Registers */
#define GSBI_CTRL_REG(id)        		(GSBI_BASE(id) + 0x0)

#define GSBI_CTRL_REG_PROTOCOL_CODE_S   4
#define GSBI_PROTOCOL_CODE_I2C          0x2
#define GSBI_PROTOCOL_CODE_SPI          0x3
#define GSBI_PROTOCOL_CODE_UART_FLOW    0x4
#define GSBI_PROTOCOL_CODE_I2C_UART     0x6

#define GSBI_HCLK_CTL_S                 4
#define GSBI_HCLK_CTL_CLK_ENA           0x1


#define QUP_BASE(id)					(GSBI_BASE(id) + 0x80000)

#define QUP_CONFIG						(QUP_BASE(id) + 0x0000)
#define QUP_CONFIG_CORE_CLK_ON_EN_S		13
#define QUP_CONFIG_APP_CLK_ON_EN_S		12
#define QUP_CONFIG_MINI_CORE_SPI_S		8
#define QUP_CONFIG_NO_INPUT_S			7
#define QUP_CONFIG_NO_OUTPUT_S			6
#define QUP_CONFIG_CORE_CLK_ON_EN		(0 << QUP_CONFIG_CORE_CLK_ON_EN_S)
#define QUP_CONFIG_APP_CLK_ON_EN		(0 << QUP_CONFIG_APP_CLK_ON_EN_S)
#define QUP_CONFIG_NO_INPUT				(0 << QUP_CONFIG_NO_INPUT_S) //Bit 7, 8 if 0, indicate full duplex mode.
#define QUP_CONFIG_NO_OUTPUT			(0 << QUP_CONFIG_NO_OUTPUT_S)
#define QUP_CONFIG_MINI_CORE_SPI		(1 << QUP_CONFIG_MINI_CORE_SPI_S) //Bits 9-12 in QUP_CONFIG are set //(QUP_CONFIG(id) + 0x0001)
#define QUP_CONFIG_N					0x3 //Bits 1-4 in QUP_CONFIG are set

#define QUP_STATE						(QUP_BASE(id) + 0x0004)
#define QUP_STATE_SPI_GEN_S				3
#define QUP_STATE_STATE_VALID_S			2
#define QUP_STATE_STATE					0x11
#define QUP_STATE_STATE_RESET_STATE		0x00
#define QUP_STATE_STATE_RUN_STATE		0x01
#define QUP_STATE_STATE_PAUSE_STATE		0x11
#define QUP_STATE_STATE_CLEAR_STATE		0x10

#define QUP_IO_MODES					(QUP_BASE(id) + 0x0008)
#define QUP_IO_MODES_OUTPUT_BITSHIFT_EN 16
#define QUP_IO_MODES_PACK_EN 			15
#define QUP_IO_MODES_UNPACK_EN 			14
#define QUP_IO_MODES_INPUT_MODE			13
#define QUP_IO_MODES_OUTPUT_MODE		10
#define QUP_IO_MODES_INPUT_FIFO_SIZE	7
#define QUP_IO_MODES_INPUT_BLOCK_SIZE	5
#define QUP_IO_MODES_OUTPUT_FIFO_SIZE	2
#define QUP_IO_MODES_OUTPUT_BLOCK_SIZE	0

#define QUP_SW_RESET					(QUP_BASE(id) + 0x000C)

#define QUP_ERROR_FLAGS					(QUP_BASE(id) + 0x001C)
#define QUP_ERROR_FLAGS_TIMEOUT			6
#define QUP_ERROR_FLAGS_OUTPUT_OVERRUN	5
#define QUP_ERROR_FLAGS_INPUT_UNDERRUN	4
#define QUP_ERROR_FLAGS_OUTPUT_UNDERRUN	3
#define QUP_ERROR_FLAGS_INPUT_OVERRUN	2

#define QUP_ERROR_FLAGS_EN					(QUP_BASE(id) + 0x0020)
#define QUP_ERROR_FLAGS_EN_TIMEOUT			6
#define QUP_ERROR_FLAGS_EN_OUTPUT_OVERRUN	5
#define QUP_ERROR_FLAGS_EN_INPUT_UNDERRUN	4
#define QUP_ERROR_FLAGS_EN_OUTPUT_UNDERRUN	3
#define QUP_ERROR_FLAGS_EN_INPUT_OVERRUN	2

#define QUP_MX_OUTPUT_COUNT				(QUP_BASE(id) + 0x0100)
#define QUP_MX_WRITE_COUNT				(QUP_BASE(id) + 0x0150)
#define QUP_MX_INPUT_COUNT				(QUP_BASE(id) + 0x0200)
#define QUP_MX_READ_COUNT				(QUP_BASE(id) + 0x0208)

//Output buffer
#define QUP_OUTPUT_FIFO					(QUP_BASE(id) + 0x0110 + 4*id)

//Input buffer
#define QUP_INPUT_FIFO					(QUP_BASE(id) + 0x0218 + 4*id)

#define SPI_CONFIG						(QUP_BASE(id) + 0x0300)
#define SPI_CONFIG_SLAVE_OPER_S			5
#define SPI_CONFIG_SLAVE_OPER			(1 << SPI_CONFIG_SLAVE_OPER_S)
#define SPI_CONFIG_MASTER_OPER			(0 << SPI_CONFIG_SLAVE_OPER_S)
#define SPI_CONFIG_HS_MODE_S			10
#define SPI_CONFIG_NON_HS_MODE			(0 << SPI_CONFIG_HS_MODE_S)
#define SPI_CONFIG_HS_MODE				(1 << SPI_CONFIG_HS_MODE_S)

#define SPI_IO_CONTROL					(QUP_BASE(id) + 0x0304)
#define SPI_IO_CONTROL_CLK_IDLE_HIGH	(1 << 11)
#define SPI_IO_CONTROL_CLK_ALWAYS_ON	(1 << 10)
#define SPI_IO_CONTROL_CS0_N_POLARITY	(1 << 4)
#define SPI_IO_CONTROL_CS1_N_POLARITY	(1 << 5)
#define SPI_IO_CONTROL_CS2_N_POLARITY	(1 << 6)
#define SPI_IO_CONTROL_CS3_N_POLARITY	(1 << 7)
#define SPI_IO_CONTROL_CS0_N_SELECT		00
#define SPI_IO_CONTROL_CS1_N_SELECT		01
#define SPI_IO_CONTROL_CS2_N_SELECT		10
#define SPI_IO_CONTROL_CS3_N_SELECT		11

#define SPI_ERROR_FLAGS					(QUP_BASE(id) + 0x0308)
#define SPI_ERROR_FLAGS_EN				(QUP_BASE(id) + 0x030C)
#define SPI_DEASSERT_WAIT				(QUP_BASE(id) + 0x0310)
#define DEASSERT_WAIT					(0 << 5)		//SPI_CS#_N remains deasserted for one clock tick.

#define SPI_GPIO_CLK					58
#define SPI_GPIO_CS						55
#define SPI_GPIO_MOSI					52
#define SPI_GPIO_MISO					53


//From 80-N1622-3 MSM8960   -MSM8960    PRO-MSM8X60A   -MSM8X60A    PRO MOBILE STATION MODEM SOFTWARE INTERFACE.pdf.qck (section 4.2.3)
#if 0
#define TLMM_BASE_ADDR					0x00800000
#define GPIO_CONFIG_ADDR(x)				(TLMM_BASE_ADDR + 0x1000 + (x)*0x10)
#define GPIO_IN_OUT_ADDR(x)				(TLMM_BASE_ADDR + 0x1004 + (x)*0x10)
#endif


struct SPI_config
{
	UINT32		   gsbi_id;
	UINT32		   IO_mode;					// Can be one of FIFO_Mode, Block_Mode or Data_Mover_Mode
	UINT32		   clock_rate;				// Can be either Non_HS_MODE (less than 26MHz) or HS_MODE (between 26 MHz and 52 MHz)
	UINT8		   master_slave_mode;
	UINT32         SPI_mod;
};

enum SPI_Errors
{
	Invalid_Device_ID,
	Initialization_Error,
	Invalid_QUP_State,
	Invalid_SPI_Mode
};

enum QUP_States
{
	RESET_STATE,
	RUN_STATE,
	CLEAR_STATE,
	PAUSE_STATE
};

enum QUP_IO_Modes
{
	FIFO_Mode,
	Block_Mode,
	Data_Mover_Mode
};

enum SPI_Clock_rate
{
	Non_HS_MODE,
	HS_MODE
};

enum master_slave_mode
{
	Master,
	Slave
};

enum SPI_CPOL_CPHA_mode
{
	CPOL_LOW_CPHA_LOW,
	CPOL_LOW_CPHA_HIGH,
	CPOL_HIGH_CPHA_LOW,
	CPOL_HIGH_CPHA_HIGH
};


class MSM8960_SPI_Driver
{
private:
	//SPI_CONFIGURATION &config;
	SPI_config &config;

public:
	////BOOL Initialize(SPI_CONFIGURATION &config, int gsbi_id, int SPI_mode, int IO_mode, int master_slave_mode, int clock_rate);
	BOOL Initialize(int gsbi_id, int SPI_mode, int IO_mode, int master_slave_mode, int clock_rate);
	void Uninitialize(const SPI_config &config);
	void ISR(void*);
	BOOL nWrite16_nRead16( const SPI_config& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
	BOOL nWrite8_nRead8( const SPI_config& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset );
	BOOL Xaction_Start( const SPI_config& Configuration );
	BOOL Xaction_Stop( const SPI_config& Configuration );
	BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
	BOOL Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction );
	void GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );
	void spi_write_byte(UINT32 data);
	UINT32* spi_read_byte();
	void spi_write_bit(UINT8 data);
	UINT8* spi_read_bit();
};
