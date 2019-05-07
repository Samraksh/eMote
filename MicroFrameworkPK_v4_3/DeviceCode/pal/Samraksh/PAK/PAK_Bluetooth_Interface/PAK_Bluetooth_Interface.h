#ifndef PAK_BLUETOOTH_INTERFACE_H_
#define PAK_BLUETOOTH_INTERFACE_H_


#include <WireProtocol.h>
#include <MFUpdate_decl.h>

bool InitializeInterface();	    
bool InitializeDriversAfterInstall();
bool UnInitializeDriversBeforeInstall();

#endif /* PAK_BLUETOOTH_INTERFACE_H_ */
