#ifndef NETMF_RADIO
#define NETMF_RADIO

#include <Tinyhal_types.h>
#include <tinyhal.h>
#include <CPU_SPI_decl.h>

BOOL mfhal_init();

//SPI
BOOL configure_spi();
//GPIO
BOOL configure_gpio();
//EXTI
BOOL configure_exti();
BOOL read_exti();
//Timers
BOOL configure_timers();
BOOL timer_delay(int duration);

//SPI Read Write Command
BOOL spi_write(UINT8 address, UINT8 data);
UINT8 spi_read(UINT8 address);

//SPI read write Frames
UINT8* spi_frame_read(UINT8* data, UINT8 length);
BOOL spi_frame_write(UINT8* data, UINT8 length);

//Radio API's
BOOL slp_tr(BOOL state);
BOOL rst(BOOL state);
//Impelment wrapper functions for the radio




#endif