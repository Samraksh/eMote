#ifndef NETMF_RADIO
#define NETMF_RADIO

#include <Tinyhal_types.h>
#include <tinyhal.h>
#include <CPU_SPI_decl.h>

BOOL mfhal_init();
BOOL configure_spi();
BOOL configure_gpio();
BOOL configure_exti();
BOOL configure_timers();
BOOL spi_write(UINT8 address, UINT8 data);
UINT8 spi_read(UINT8 address);
BOOL slp_tr(BOOL state);
BOOL rst(BOOL state);
BOOL read_exti();
BOOL timer_delay(int duration);

#endif