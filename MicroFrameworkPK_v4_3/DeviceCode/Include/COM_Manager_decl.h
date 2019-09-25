#ifndef _COM_MANAGER_DECL_H_
#define _COM_MANAGER_DECL_H_ 1


enum COM_END_POINT
{
	ENCRYPTED = 0,
	UNENCRYPTED = 1,
	GATEWAY = 2
};

enum EBTChannel {
ENCRYPTED_DATA_CHANNEL=0x09,
UNENCRYPTED_DATA_CHANNEL=0x0C,
CLOUD_CHANNEL=0x0F
};


void COM_Manager_Initialization(uint8_t Param);
void COM_Manager_Uninitialize();
void COM_Manager_Message_Data(UINT8 destination, uint8_t *buffer, uint16_t buffer_size);
void Message_Receive_From_CP( uint8_t *buffer, uint16_t buffer_size);

typedef void (*ReceiveFPtrType) (void *msg, UINT16 arg1);

//Bluetooth manager apis
void BTMAC_Manager_Initialization(uint8_t bluetoothParam, ReceiveFPtrType openCB, ReceiveFPtrType secureCB);
void BTMAC_Manager_Uninitialize();
bool BTMAC_Manager_Send(void* buffer, UINT16 size, UINT16 channel );
int Message_Encrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *encryptedBuffer);
int Message_Decrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *decryptedBuffer);

#endif // _COM_MANAGER_DECL_H_

