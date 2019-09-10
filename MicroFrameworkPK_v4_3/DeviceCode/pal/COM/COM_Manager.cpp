#include <tinyhal.h>
#include <CryptokiPAL.h>
#include <crypto.h>
#include <COM_Manager_decl.h>
#include <Samraksh\MAC_decl.h>
#include <Samraksh\MAC.h>
#include <Samraksh\BluetoothMac_Functions.h>

const int hmacSize=48;
static uint8_t currentMac = NONE;
static MACEventHandler btEventHandler;
static int current_neighbor_count = 0;

// currently we only are tracking one link. We need to expand to tracking multiple neighbors and whether that link is encrypted or not
static bool link_encrypted = false;

// this must match drivers/bluetooth/sf2/btmain.h and drivers\bluetooth\c_code_calling_cpp.h
#define ENCRYPTED_DATA_CHANNEL 0x09
#define UNENCRYPTED_DATA_CHANNEL 0x0C
#define CLOUD_CHANNEL 0x0F

int Message_Encrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *encryptedBuffer);
int Message_Decrypt_Data(uint8_t *origBuffer, uint16_t buffer_size, uint8_t *decryptedBuffer);

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
	}
	hal_printf("\r\n");
}

void COM_Manager_Receive(void* buffer, UINT16 payloadType) {
	CK_BYTE decryptedData[128];
	if (currentMac == BLUETOOTHMAC){
		Message_15_4_t* msg = (Message_15_4_t*)buffer;
		int size = msg->GetHeader()->length - sizeof(IEEE802_15_4_Header_t);
		if (msg->GetHeader()->payloadType == ENCRYPTED_DATA_CHANNEL){
			hal_printf("got encrypted data\r\n");
			link_encrypted = true;
			int dataLen = Message_Decrypt_Data(msg->GetPayload(), size, decryptedData);
			
			// all messages to the CP assume '\n' termination
			CP_SendMsgToCP(decryptedData, dataLen);
		}
		else if (msg->GetHeader()->payloadType == UNENCRYPTED_DATA_CHANNEL){
			hal_printf("got unencrypted data\r\n");
			PrintHex(msg->GetPayload(),size);
			// all messages to the CP assume '\n' termination
			CP_SendMsgToCP(msg->GetPayload(), size);
			/*uint8_t testData[10];
			testData[0] = 'c';
			testData[1] = 'l';
			testData[2] = 'o';
			testData[3] = 'u';
			testData[4] = 'd';
			//CP_SendMsgToCP(&testData[0], 5);
			CPU_Timer_Sleep_MicroSeconds(100000,DEFAULT_TIMER);
			hal_printf("sending back after a delay\r\n");
			MAC_Send(UNENCRYPTED_DATA_CHANNEL, NULL, &testData[0], 5);*/
		}
		else if (msg->GetHeader()->payloadType == CLOUD_CHANNEL){
			hal_printf("got cloud data\r\n");
			PrintHex(msg->GetPayload(),size);
			uint8_t testData[10];
			testData[0] = 'c';
			testData[1] = 'l';
			testData[2] = 'o';
			testData[3] = 'u';
			testData[4] = 'd';
			//CP_SendMsgToCP(&testData[0], 5);
			hal_printf("sending back cloud\r\n");
			MAC_Send(CLOUD_CHANNEL, NULL, &testData[0], 5);
		} else {
			hal_printf("xxxx pt: %d headerPT: %d\r\n", payloadType, msg->GetHeader()->payloadType);
		}
		// UINT8* unaligned_payload = msg->GetPayload();
	}
}

void COM_Manager_Neighbor_Change(INT16 neighbors) {
	hal_printf("*** neighbor change %d ****\r\n", neighbors);
	current_neighbor_count = neighbors;
}

void COM_Manager_Initialization(uint8_t bluetoothParam){
	btEventHandler.SetReceiveHandler(&COM_Manager_Receive);
	btEventHandler.SetNeighborChangeHandler(&COM_Manager_Neighbor_Change);
	MAC_Initialize(&btEventHandler, bluetoothParam, NULL, NULL, NULL);
	currentMac = bluetoothParam;		

	current_neighbor_count = 0;
}

int COM_Manager_Get_Neighbor_Count(){
	return current_neighbor_count;
}

void COM_Manager_Uninitialize(){
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

void Message_Receive_From_CP( uint8_t *buffer, uint16_t buffer_size){
	CK_BYTE encryptedData[128];

		hal_printf("sending data: ");
		PrintHex(buffer,buffer_size);

	if (currentMac == BLUETOOTHMAC){
		if (link_encrypted == true){
			hal_printf("encrypted link\r\n");
			int encryptSize = Message_Encrypt_Data(buffer, buffer_size, encryptedData);
			MAC_Send(ENCRYPTED_DATA_CHANNEL, NULL, encryptedData, encryptSize);
		} else {
			MAC_Send(UNENCRYPTED_DATA_CHANNEL, NULL, buffer, buffer_size);
		}
	}
}
