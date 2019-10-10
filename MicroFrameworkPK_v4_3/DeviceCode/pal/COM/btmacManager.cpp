#include <tinyhal.h>
#include <CryptokiPAL.h>
#include <crypto.h>
#include <COM_Manager_decl.h>
#include <Samraksh\MAC_decl.h>
#include <Samraksh\MAC.h>
#include <Samraksh\BluetoothMac_Functions.h>
#include <Samraksh/VirtualTimer.h>
#include <TinyCLR_Types.h> 
#include <PAK_Bluetooth_Interface\PAK_Bluetooth_Interface.h>
#include <eNVM/eNVM.h>

extern Buffer_15_4_t g_send_buffer;
const int hmacSize=48;
static uint8_t currentMac = NONE;
static MACEventHandler btEventHandler;
static int current_neighbor_count = 0;

#define MARKER_DEBUGGER_V1 "MSdbgV1" // Used to identify the debugger at boot time.
#define MARKER_PACKET_V1   "MSpktV1" // Used to identify the start of a packet.
#define SIGNATURE_SIZE 7

static HAL_CONTINUATION cloudMessageContinuation;

MACReceiveFuncPtrType secureRcvCB, openRcvCB;

// currently we only are tracking one link. We need to expand to tracking multiple neighbors and whether that link is encrypted or not
static bool link_encrypted = false;

// this must match drivers/bluetooth/sf2/btmain.h and drivers\bluetooth\c_code_calling_cpp.h
#define ENCRYPTED_DATA_CHANNEL 0x09
#define UNENCRYPTED_DATA_CHANNEL 0x0C
#define CLOUD_CHANNEL 0x0F

CK_BYTE key1[hmacSize] = {
		0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08
};

const CK_BYTE hmac1[hmacSize] = {
		0, 103, 74, 155, 97, 125, 27, 130,
		83, 111, 216, 226, 156, 45, 100, 50,
		59, 61, 228, 144, 127, 39, 150, 29,
		253, 74, 92, 188, 247, 200, 88, 195
};

CK_BYTE ddata[128];

	CK_BYTE data[128];
	CK_BYTE digest[32];
	CK_BYTE IV[48];
	CK_BYTE_PTR  pData;
	CK_ULONG ulDataLen;
	CK_BYTE  pCryptText[128];
	CK_ULONG ulCryptLen;
	CK_BYTE_PTR pDigest;
	CK_MECHANISM_TYPE mtype;
	CK_KEY_TYPE kt;
	CK_BYTE_PTR pkey;

void PrintHex(CK_BYTE_PTR sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
		if ((j%16)==0) hal_printf("\r\n");
	}
	hal_printf("\r\n");
}

void Cloud_Bluetooth_Response_Call(void *arg)
{
	if (MAC_GetPendingPacketsCount_Send() > 0){
		Message_15_4_t** tempPtr = g_send_buffer.GetOldestPtr();
		Message_15_4_t* msgPtr = *tempPtr;
		int size = msgPtr->GetHeader()->length - sizeof(IEEE802_15_4_Header_t);
		uint8_t* payload = (uint8_t*)msgPtr->GetPayload();

		if ((memcmp( payload, MARKER_PACKET_V1, SIGNATURE_SIZE) ) == 0) {
			g_Samraksh_Emote_Update.Receive(payload, size);
		} else {
			hal_printf("no MARKER\r\n");
		}

		//hal_printf("delayed response data size: %d\r\n", size);
		//PrintHex(msgPtr->GetPayload(),size);
	} else {
		hal_printf("no messages in buffer!!\r\n");
	}
	
}

void BTMAC_Manager_Receive(void* buffer, UINT16 payloadType) {
	CPU_GPIO_SetPinState(15, TRUE);
	CPU_GPIO_SetPinState(15, FALSE);
	//CPU_Timer_Sleep_MicroSeconds(100000,DEFAULT_TIMER);
	CK_BYTE decryptedData[128];
	if (currentMac == BLUETOOTHMAC){
		Message_15_4_t* msg = (Message_15_4_t*)buffer;
		int size = msg->GetHeader()->length - sizeof(IEEE802_15_4_Header_t);
		Message_15_4_t msg_carrier;
		IEEE802_15_4_Header_t* header = msg_carrier.GetHeader();	
		uint8_t* payload = msg->GetPayload();
		if (msg->GetHeader()->payloadType == ENCRYPTED_DATA_CHANNEL){
			hal_printf("got encrypted data\r\n");
			link_encrypted = true;
			//int dataLen = Message_Decrypt_Data(msg->GetPayload(), size, decryptedData);
			
			// *** important ***
			// The Bluetooth driver is currently in the middle of calling the Rx callback and this must exit
			// before any more bluetooth packets can be sent or received			
			int dataLen = Message_Decrypt_Data(msg->GetPayload(), size, decryptedData);
			
			// sending the packet to the CP allows the bluetooth driver to exit before any bluetooth packets 
			// are sent or received again
			// all messages to the CP assume '\n' termination
			//CP_SendMsgToCP(decryptedData, dataLen);
			secureRcvCB(msg->GetPayload(), size);
		}
		else if (msg->GetHeader()->payloadType == UNENCRYPTED_DATA_CHANNEL){
			hal_printf("got unencrypted data\r\n");
			//PrintHex(msg->GetPayload(),size);
			openRcvCB(msg->GetPayload(), size);

			// *** important ***
			// The Bluetooth driver is currently in the middle of calling the Rx callback and this must exit
			// before any more bluetooth packets can be sent or received
			// sending the packet to the CP allows the bluetooth driver to exit before any bluetooth packets 
			// are sent or received again
			// all messages to the CP assume '\n' termination
			CP_SendMsgToCP(msg->GetPayload(), size);
		}
		else if (msg->GetHeader()->payloadType == CLOUD_CHANNEL){
			//hal_printf("got cloud data size: %d\r\n", size);
			//PrintHex(msg->GetPayload(),size);
			memcpy((void*)&msg_carrier, (void*)msg, msg->GetHeader()->length);

			if ((memcmp( payload, MARKER_PACKET_V1, SIGNATURE_SIZE) ) == 0) {
				// *** important ***			
				// The Bluetooth driver is currently in the middle of calling the Rx callback and this must exit
				// before any more bluetooth packets can be sent or received
				// we can change this to a continuation if needed
				if(!g_send_buffer.Store((void *) &msg_carrier, header->length)){
					hal_printf("error storing msg\r\n");
				}

				// Queueing up message processing in continuation
				cloudMessageContinuation.Enqueue();
			}
			openRcvCB(msg->GetPayload(), size);
			

		} else {
			hal_printf("xxxx pt: %d headerPT: %d\r\n", payloadType, msg->GetHeader()->payloadType);
		}
		// UINT8* unaligned_payload = msg->GetPayload();
	}
	
}


bool BTMAC_Manager_Send(void* buffer, UINT16 size, UINT16 channel ){
	if (channel== CLOUD_CHANNEL || channel== UNENCRYPTED_DATA_CHANNEL || channel== ENCRYPTED_DATA_CHANNEL){
		MAC_Send(channel, NULL, buffer, size);
		return TRUE;
	}
	return FALSE;
}

void BTMAC_Manager_Neighbor_Change(INT16 neighbors) {
	hal_printf("*** neighbor change %d ****\r\n", neighbors);
	current_neighbor_count = neighbors;
}

void BTMAC_Manager_Initialization(uint8_t bluetoothParam, MACReceiveFuncPtrType _openCB, MACReceiveFuncPtrType _secureCB){
	btEventHandler.SetReceiveHandler(&BTMAC_Manager_Receive);
	btEventHandler.SetNeighborChangeHandler(&BTMAC_Manager_Neighbor_Change);

	secureRcvCB=_secureCB;
	openRcvCB = _openCB;

	MAC_Initialize(&btEventHandler, bluetoothParam, NULL, NULL, NULL);
	currentMac = bluetoothParam;		

	current_neighbor_count = 0;

	g_Samraksh_Emote_Update.UpdateInit(NULL, NULL);
	cloudMessageContinuation.InitializeCallback(Cloud_Bluetooth_Response_Call, NULL);
	/*CPU_GPIO_EnableOutputPin(15,FALSE);
	CPU_GPIO_EnableOutputPin(16,FALSE);
	CPU_GPIO_EnableOutputPin(17,FALSE);
	CPU_GPIO_SetPinState(15, TRUE);
	CPU_GPIO_SetPinState(15, FALSE);
	CPU_GPIO_SetPinState(15, TRUE);
	CPU_GPIO_SetPinState(15, FALSE);
	CPU_GPIO_SetPinState(16, TRUE);
	CPU_GPIO_SetPinState(16, FALSE);
	CPU_GPIO_SetPinState(16, TRUE);
	CPU_GPIO_SetPinState(16, FALSE);
	CPU_GPIO_SetPinState(17, TRUE);
	CPU_GPIO_SetPinState(17, FALSE);
	CPU_GPIO_SetPinState(17, TRUE);
	CPU_GPIO_SetPinState(17, FALSE);*/
}

int BTMAC_Manager_Get_Neighbor_Count(){
	return current_neighbor_count;
}

void BTMAC_Manager_Uninitialize(){
	MAC_UnInitialize();
	current_neighbor_count = 0;
}

int Message_Encrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *encryptedBuffer){
	memset(pDigest,0,hmacSize);
	int i;
	CK_BYTE dataToEncrypt[128];

	// The current encrypt / decrypt algorithm always pads data to 16 byte boundaries
	// currently the first byte of encrypted text is the actual message length
	dataToEncrypt[0] = buffer_size;
	for (i=0; i < buffer_size; i++){
		dataToEncrypt[i+1] = (CK_BYTE)origBuffer[i];
	}
	CK_ULONG dataToEncryptLen = buffer_size + 1;
	//Crypto_GetRandomBytes(IV, 48);
	for (int i=0; i<48; i++){
		key1[i] = i;
	}
	for (int i=0; i<48; i++){
		IV[i] = i;
	}

	hal_printf("IV : ");
	PrintHex(IV,48);
	hal_printf("\r\n");
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;

	hal_printf("Encrypting data\r\n");
	hal_printf(" ***** encryption locking up *****\r\n");
	bool ret= Crypto_Encrypt(pkey,32,IV, 48, dataToEncrypt, dataToEncryptLen, encryptedBuffer, ulCryptLen);

	if(!ret){hal_printf("\r\nEncryption Failed\r\n");}
	hal_printf("Encrypted Text: ");PrintHex(encryptedBuffer,ulCryptLen);
	hal_printf("\r\n");

	return ulCryptLen;
}

int Message_Decrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *decryptedBuffer){
	memset(pDigest,0,hmacSize);
	int i;
	for (i=0; i<buffer_size; i++){
		pCryptText[i] = (CK_BYTE)origBuffer[i];
	}
	ulDataLen=128; // should be overwritten
	ulCryptLen=buffer_size;
	Crypto_GetRandomBytes(IV, 48);
	for (int i=0; i<48; i++){
		key1[i] = i;
	}
	for (int i=0; i<48; i++){
		IV[i] = i;
	}

	hal_printf("IV : ");
	PrintHex(IV,48);
	hal_printf("\r\n");
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;

	hal_printf("\r\nreceived encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	bool ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, decryptedBuffer, &ulDataLen);
	if(!ret){hal_printf("Decryption Failed\n");}
	hal_printf("Decrypted Text (%d): ", ulDataLen);PrintHex(decryptedBuffer,ulDataLen);
	hal_printf("\r\n");
	hal_printf("\r\n");

	// The current encrypt / decrypt algorithm always pads data to 16 byte boundaries
	// currently the first byte of decrypted text is the actual message length
	int dataLen = decryptedBuffer[0];
	for (int i=0; i<dataLen; i++){
		decryptedBuffer[i] = decryptedBuffer[i+1];
	}

	hal_printf("Decrypted Text (%d): ", dataLen);
	for (int j=0; j< dataLen; j++){
		hal_printf("%c",(char)decryptedBuffer[j]);
	}
	hal_printf("\r\n");
	PrintHex(decryptedBuffer,dataLen);
	hal_printf("\r\n");

	return dataLen;
}


