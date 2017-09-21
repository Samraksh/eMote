#include "mxc_config.h"
#include "clkman.h"
#include "ioman.h"
#include "netmf_spi.h"

#include "platform_selector.h"


#undef DEBUG_SPI
#undef FIRST_BIT_LSB 
#undef NSS_SOFTWARE_MODE 

/*#define MX25_BAUD           48000000    // 48 MHz maximum, 20 kHz minimum
#define MX25_ADDR           0x0
#define MX25_SPIM_WIDTH     SPIM_WIDTH_4
#define MX25_EXP_ID         0xc22538
*/

//--------- Macros -----------

#define IOMAN_F_SPIM0(cfg,io, ss0, ss1, ss2, ss3, ss4, q, f) {                            \
        cfg.req_reg = &MXC_IOMAN->spim0_req;                                           \
        cfg.ack_reg = &MXC_IOMAN->spim0_ack;                                           \
        cfg.req_val.spim0.core_io_req = io;                                       \
        cfg.req_val.spim0.ss0_io_req = ss0;                                       \
        cfg.req_val.spim0.ss1_io_req = ss1;		\
        cfg.req_val.spim0.ss2_io_req = ss2;		\
        cfg.req_val.spim0.ss3_io_req = ss3;		\
        cfg.req_val.spim0.ss4_io_req = ss4;      \
        cfg.req_val.spim0.quad_io_req = q;       \
        cfg.req_val.spim0.fast_mode = f; 	\
}

#define IOMAN_F_SPIM1(cfg,io, ss0, ss1, ss2, q, f) {                                      \
        cfg.req_reg = &MXC_IOMAN->spim1_req;	\
        cfg.ack_reg = &MXC_IOMAN->spim1_ack;	\
        cfg.req_val.spim1.core_io_req = io;	\
        cfg.req_val.spim1.ss0_io_req = ss0;	\
        cfg.req_val.spim1.ss1_io_req = ss1;	\
        cfg.req_val.spim1.ss2_io_req = ss2;	\
        cfg.req_val.spim1.quad_io_req = q;	\
        cfg.req_val.spim1.fast_mode = f;	\
	}

#define IOMAN_F_SPIM2(cfg,m, io, ss0, ss1, ss2, sr0, sr1, q, f) {                         \
        cfg.req_reg = &MXC_IOMAN->spim2_req;	\
        cfg.ack_reg = &MXC_IOMAN->spim2_ack;	\
        cfg.req_val.spim2.mapping_req = m;	\
        cfg.req_val.spim2.core_io_req = io;	\
        cfg.req_val.spim2.ss0_io_req = ss0;	\
        cfg.req_val.spim2.ss1_io_req = ss1;	\
		cfg.req_val.spim2.ss2_io_req = ss2;	\
		cfg.req_val.spim2.sr0_io_req = sr0;	\
		cfg.req_val.spim2.sr1_io_req = sr1;	\
		cfg.req_val.spim2.quad_io_req = q;	\
		cfg.req_val.spim2.fast_mode = f;	\
	}

//-----------------------------------------------------//

/*--------- Internal Functions ----------- */
bool spiDriverInit=false;
bool SPI_Initialized[SPI_MAX_PORTS];
UINT32 SPI_SLAVES_GPIO[SPI_SLAVE_COUNT];

inline BOOL CheckSlaves(GPIO_PIN pin){
	BOOL ret=FALSE;
	for (uint i=0 ; i<SPI_SLAVE_COUNT; i++) {
		if(SPI_SLAVES_GPIO[i]==pin){
			return TRUE;
		}
	}

	return ret;
}

inline mxc_spim_regs_t* GetSPIReg (uint8_t _mport){
	mxc_spim_regs_t* ret;
	switch(_mport){
		case SPIPort_M0: ret= MXC_SPIM0; break;
		case SPIPort_M1: ret= MXC_SPIM1; break;
		case SPIPort_M2A: ret= MXC_SPIM2; break;
		case SPIPort_M2B: ret= MXC_SPIM2; break;
		default:break;
	}
	return ret;
}

inline uint32_t GetSSel(uint32_t slaveNo, bool high){
	uint32_t ret;
	if(high){
		if(slaveNo==0) ret=SPIM_SSEL0_HIGH;
		if(slaveNo==1) ret=SPIM_SSEL1_HIGH;
		if(slaveNo==2) ret=SPIM_SSEL2_HIGH;
		if(slaveNo==3) ret=SPIM_SSEL3_HIGH;
		if(slaveNo==4) ret=SPIM_SSEL4_HIGH;
	}else {
		if(slaveNo==0) ret=SPIM_SSEL0_LOW;
		if(slaveNo==1) ret=SPIM_SSEL1_LOW;
		if(slaveNo==2) ret=SPIM_SSEL2_LOW;
		if(slaveNo==3) ret=SPIM_SSEL3_LOW;
		if(slaveNo==4) ret=SPIM_SSEL4_LOW;
	}
	return ret;
}

inline int8_t GetSPIPortForSlave(uint32_t chipsel){
	if (chipsel==GPIO_SLAVE0_CHIPSELECT){
		return SPI_PORT_SLAVE0;
	}
	if (chipsel==GPIO_SLAVE1_CHIPSELECT){
		//return SPI_PORT_SLAVE0;
	}
	if (chipsel==GPIO_SLAVE2_CHIPSELECT){
			//return SPI_PORT_SLAVE0;
	}
	if (chipsel==GPIO_SLAVE3_CHIPSELECT){
			//return SPI_PORT_SLAVE0;
	}
	return -1;
}

mxc_spim_regs_t* GetSPIRegForSlave (uint32_t chipsel){
	int8_t port=GetSPIPortForSlave(chipsel);
	if(port) return GetSPIReg(port);
}


inline spim_width_t GetSPIWidthForSlave(uint32_t chipsel){
	int8_t port=GetSPIPortForSlave(chipsel);
	spim_width_t ret=SPIM_WIDTH_1;
	switch(port){
			case SPIPort_M0: ret= (spim_width_t)PortM0_Width; break;
			case SPIPort_M1: ret= (spim_width_t)PortM1_Width; break;
			case SPIPort_M2A: ret= (spim_width_t)PortM2_Width; break;
			case SPIPort_M2B: ret= (spim_width_t)PortM2_Width; break;
			//case SPIPort_B: ret= MXC_SPIB; break;
			default:break;
		}
	return ret;
}

/*--------- SPI Interface Implementations ----------- */

BOOL CPU_SPI_Initialize ()
{
#if defined(DEBUG_SPI)
	hal_printf("SPI Initialize\n");
#endif

	NATIVE_PROFILE_HAL_PROCESSOR_SPI();

	switch(SPI_SLAVE_COUNT){
		case 4: SPI_SLAVES_GPIO[3]=GPIO_SLAVE3_CHIPSELECT;
		case 3: SPI_SLAVES_GPIO[2]=GPIO_SLAVE2_CHIPSELECT;
		case 2: SPI_SLAVES_GPIO[1]=GPIO_SLAVE1_CHIPSELECT;
		case 1: SPI_SLAVES_GPIO[0]=GPIO_SLAVE0_CHIPSELECT;
		default: break;
	}
	spiDriverInit=TRUE;
}

void CPU_SPI_Uninitialize()
{
#if defined(DEBUG_SPI)
	hal_printf("SPI Uninitialize\n");
#endif
	
	//shutdown all SPI masters
	SPIM_Shutdown(MXC_SPIM0);
	SPIM_Shutdown(MXC_SPIM1);
	SPIM_Shutdown(MXC_SPIM2);
	//SPIM_Shutdown(MXC_SPIB);

	spiDriverInit=FALSE;
}

void CPU_SPI_Uninitialize(SPI_CONFIGURATION config)
{
#if defined(DEBUG_SPI)
			hal_printf("SPI_Uninitialize: \n");
#endif
	mxc_spim_regs_t *spi_reg=GetSPIRegForSlave(config.DeviceCS);
	if(spi_reg)SPIM_Shutdown(spi_reg);
	SPI_Initialized[GetSPIPortForSlave(config.DeviceCS) -1 ]=FALSE;
}

BOOL CPU_SPI_Enable(SPI_CONFIGURATION config)
{
	if(!spiDriverInit) CPU_SPI_Initialize();

	uint8_t _mport= GetSPIPortForSlave(config.DeviceCS);
	//Initialize the SPIM
	spim_cfg_t cfg;
	cfg.mode =config.SPI_mod;
	cfg.ssel_pol = GetSSel(0,config.CS_Active) ;	//We will always use slave 0 on a master for now.
	cfg.baud = config.Clock_RateKHz * 1000;

	sys_cfg_spim_t sys_cfg;
	//set clock to auto
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;

	mxc_spim_regs_t *spi_reg;
	//Write the below IOMAN_SPI carefully based on how many slaves are connected to each spi master
	//right now we simply request the slave 0, assuming atleast one slave is connected

	switch (_mport) {
		case SPIPort_M0:{
			spi_reg= MXC_SPIM0;
													//io, ss0, ss1, ss2, ss3, ss4, q, f
			//sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIM0(1,   1,  	0,  0,   0,    0,  0, 0);
			IOMAN_F_SPIM0(sys_cfg.io_cfg, 1,   1,  	0,  0,   0,    0,  0, 0);

			break;
		}
		case SPIPort_M1: {
			spi_reg= MXC_SPIM1;
													//io, ss0, ss1, ss2, q, f
			//sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIM1(1,   1,  	0,  0,   0, 0);
			IOMAN_F_SPIM1(sys_cfg.io_cfg, 1,   1,  	0,  0,   0, 0);
			break;
		}
		case SPIPort_M2A:{
			spi_reg= MXC_SPIM2;
					//m, 			io, ss0, ss1, ss2, sr0, sr1, q, f
			IOMAN_F_SPIM2(sys_cfg.io_cfg, IOMAN_MAP_A, 1, 1, 0, 0, 0, 0, 0, 0)
			break;
		}
		case SPIPort_M2B:{
			spi_reg= MXC_SPIM2;
					//m, 			io, ss0, ss1, ss2, sr0, sr1, q, f
			IOMAN_F_SPIM2(sys_cfg.io_cfg, IOMAN_MAP_B, 1, 1, 0, 0, 0, 0, 0, 0)
			break;
		}
		default:{
#if defined(DEBUG_SPI)
			hal_printf("SPI_Enable: Unknown SPI Port/Master #\n");
#endif
			return FALSE;
		}
	}

	int error;
	if((error = SPIM_Init(spi_reg, &cfg, &sys_cfg)) != E_NO_ERROR) {
		hal_printf("Error initializing SPIM %d\n", error);
		return FALSE;
	}

	hal_printf("SPIM Initialized\n");
	SPI_Initialized[_mport -1 ]=TRUE;
	return TRUE;
}


void CPU_SPI_GetPins (UINT32 _mport, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi)
{
	/* Return the GPIO pin for selected SPI */

#if defined(DEBUG_SPI)
	hal_printf("SPI GetPins\n");
#endif

	switch (_mport) {
		case SPIPort_M0:
			msk = 4;
			miso = 6;
			mosi = 5;
			break;
		case SPIPort_M1:
			msk = 8;
			miso = 10;
			mosi = 9;
			break;
		case SPIPort_M2A:
			msk = 20;
			miso = 22;
			mosi = 21;
			break;
		case SPIPort_M2B:
			msk = 40;
			miso = 42;
			mosi = 41;
			break;
		default:
			break;

	}
}

UINT32 CPU_SPI_PortsCount()
{

#if defined(DEBUG_SPI)
	hal_printf("SPI PortsCount\n");
#endif
	// Returns the total no. of SPI peripherals
	return (UINT32)SPI_MAX_PORTS;
}


// Nived.Sivadas - Scenarios where you want to just write a single byte and leave the reading part out
BOOL CPU_SPI_WriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));
	return false;

}

UINT8 CPU_SPI_ReadByte(const SPI_CONFIGURATION&  Configuration)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));
	return false;
}

UINT8 CPU_SPI_WriteReadByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));
	return false;
}

UINT8 CPU_SPI_ReadWriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	UINT8 read_data = 0;
	ASSERT(CheckSlaves(Configuration.DeviceCS));
	return read_data;
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
	//Performs a read/write operation on 16-bit word data.
	
#if defined(DEBUG_SPI)
	hal_printf("SPI nWrite16_nRead16\n");
#endif
	return FALSE;
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
#if defined(DEBUG_SPI)
	hal_printf("SPI nWrite8_nRead8\n");
#endif

	uint8_t port= GetSPIPortForSlave(Configuration.DeviceCS);
	//Initialize corresponding port
	CPU_SPI_Enable(Configuration);

	if(SPI_Initialized[port -1 ] == 0)
	{
#if defined(DEBUG_SPI)
		hal_printf("SPI peripheral uninitialized\n");
#endif
		return false;
	}

	spim_req_t req;
	req.callback=NULL;
	req.deass= 1;
	req.ssel= Configuration.DeviceCS;
	req.read_num=0;
	req.write_num = 0;
	req.width=GetSPIWidthForSlave(Configuration.DeviceCS);


	uint8_t readarray[256];
	req.tx_data = Write8;
	req.rx_data = readarray;

	if(WriteCount>ReadCount+ReadStartOffset) { //this is write operation
		req.len=WriteCount;
	}
	else {
		req.len=ReadCount+ReadStartOffset;
	}

	//call transaction
	CPU_GPIO_EnableOutputPin(Configuration.DeviceCS, Configuration.CS_Active);
	CPU_GPIO_SetPinState(Configuration.DeviceCS, Configuration.CS_Active);
	mxc_spim_regs_t* spim = GetSPIRegForSlave(Configuration.DeviceCS);
	int xactionResult = SPIM_Trans(spim, &req);
	while(SPIM_Busy(spim) != E_NO_ERROR) {}
	CPU_GPIO_SetPinState(Configuration.DeviceCS, !Configuration.CS_Active);

	memcpy(Read8, &readarray[ReadStartOffset],ReadCount );

	if(xactionResult < E_NO_ERROR){
		return false;
	}


	//Read or wrote something, but not the whole thing
	if(xactionResult == WriteCount || xactionResult == req.len){
		return true;
	}else {
		return false;
	}
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
	//Sets up and starts a read/write operation using the specified configuration.
	
	//first make sure, it is one of the devices we know and support
	ASSERT(CheckSlaves(Configuration.DeviceCS));

	//Initialize corresponding port
	CPU_SPI_Enable(Configuration);

	return false;
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));
	return false;
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
	return false;
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
	return false;
}
