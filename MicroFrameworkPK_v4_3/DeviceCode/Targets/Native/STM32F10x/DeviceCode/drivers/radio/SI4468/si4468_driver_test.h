#ifndef H_SI446X_TEST
#define H_SI446X_TEST
#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Radio.h>

extern void si4468_spi2_init_test(void);
extern int si4468_spi2_tx_test(void);
extern int si4468_spi2_rx_test(void);
extern void si4468_spi2_power_change();
extern void si4468_spi2_chain_tx(void);

#endif // H_SI446X_TEST
