//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_SamrkashBT_Bluetooth.h"

#include "../DeviceCode/Targets/Native/TIAM3517/DeviceCode/TIAM3517.h"
#include "../DeviceCode/Targets/Native/TIAM3517/DeviceCode/TIAM3517_SCM_PinMux/TIAM3517_PinMux.h"
#include "../DeviceCode/Targets/Native/TIAM3517/DeviceCode/TIAM3517_GPIO/TIAM3517__GPIO.h"

using namespace SamrkashBT;

int IS_CONNECTED=0;

void Bluetooth::Init( UINT32 param0, UINT32 param1, HRESULT &hr ) {
	MUX_VAL( 0x0190, (IDIS | PTU | EN  | M4) ); // gpio_158 --> radio LED
	MUX_VAL( 0x0192, (IDIS | PTU | EN  | M4) ); // gpio_159 --> radio IS_CONNECTED
	MUX_VAL( 0x217C, (IDIS | PTU | EN  | M0) ); // UART1 TX
	MUX_VAL( 0x2180, (IDIS | PTU | EN  | M0) ); // UART1 RX
	
	// Init UART @ 115200
	TIAM3517_USART_Driver::Initialize(1, 115200, 0, 8, 1, 0);
}

INT8 Bluetooth::Send( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr ) {
	volatile int i;
	UINT8 *mybuf;
	
	if (IS_CONNECTED == 0) {
		omap_set_gpio_direction(158, 0); // Set LED as output
		// Strobe LED line to connect
		omap_set_gpio_dataout(158, 1);
		for(i=0; i<32000; i++) { ; } // spin
		omap_set_gpio_dataout(158, 0);
		for(i=0; i<32000; i++) { ; } // spin
		omap_set_gpio_dataout(158, 1);
		for(i=0; i<32000; i++) { ; } // spin
		IS_CONNECTED = 1;
	}
	
	mybuf = param0.GetBuffer();
	
	// Accepts array but we're just sending one byte.
	TIAM3517_USART_Driver::WriteCharToTxBuffer( 1, mybuf[0] );
	
	// Strobe LED line to send
	omap_set_gpio_dataout(158, 0);
	for(i=0; i<32000; i++) { ; } // spin
	omap_set_gpio_dataout(158, 1);
	for(i=0; i<32000; i++) { ; } // spin
	
    return 0;
}

INT8 Bluetooth::Recieve( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr ) {
	IS_CONNECTED = omap_get_gpio_input(159);
	if (IS_CONNECTED == 0) return 0xFF;
	return TIAM3517_USART_Driver::Read( 1 );
}

