#include <tinyhal.h>
#include <COM_Manager_decl.h>
#include <PKCS11/CryptokiPAL.h>
#include <Crypto.h>
#include <Samraksh\MAC_decl.h>

void Message_Receive_From_CP( uint8_t *buffer, uint16_t buffer_size){
	CK_BYTE encryptedData[128];

		hal_printf("sending data: ");
		//PrintHex(buffer,buffer_size);

	//if (currentMac == BLUETOOTHMAC){
		//if (link_encrypted == true){
			hal_printf("encrypted link\r\n");
			int encryptSize = Message_Encrypt_Data(buffer, buffer_size, encryptedData);
			MAC_Send(ENCRYPTED_DATA_CHANNEL, NULL, encryptedData, encryptSize);
		//} else {
		//	MAC_Send(UNENCRYPTED_DATA_CHANNEL, NULL, buffer, buffer_size);
		//}
	//}
	CPU_GPIO_SetPinState(17, TRUE);
	CPU_GPIO_SetPinState(17, FALSE);
}
