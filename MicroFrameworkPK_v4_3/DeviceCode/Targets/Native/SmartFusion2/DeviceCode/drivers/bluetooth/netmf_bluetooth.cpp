#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include "c_code_calling_cpp.cpp"
#include <..\..\..\..\..\..\pal\PKCS11\CryptokiPAL.h>
#include <crypto.h>


extern "C" {
//#include "include\SS1BTPS.h"
#include "sf2\btstack_port.h"
};


const int hmacSize=48;

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

	//CK_BYTE data[128];
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
	hal_printf("\n");
}

DeviceStatus CPU_Bluetooth_Initialize( )
{
	DeviceStatus status = DS_Success;
	int Result;
	
	hal_printf("init Bluetooth\r\n");
	// configure hardware

	//configuring and resetting bluetooth module
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);
	// delay
	CPU_Timer_Sleep_MicroSeconds(500000,ADVTIMER_32BIT);
	CPU_GPIO_SetPinState(5,TRUE);

	bluetooth_main();

	return status;
}


BOOL CPU_Bluetooth_Reset()
{
	BOOL result = FALSE;
	
	return result;
}


BOOL CPU_Bluetooth_UnInitialize()
{
	BOOL result = FALSE;
	
	return result;
}

void Bluetooth_Encrypt_Data(uint8_t *buffer, uint16_t buffer_size){
	
}

void Bluetooth_Decrypt_Data(uint8_t *buffer, uint16_t buffer_size){
	memset(pDigest,0,hmacSize);
	//memcpy(data,"Samraksh eMote Cryptoki HMAC Example; Plus the wolf is great, but the fox is grey. The lamb is prey, but its a mountain pro!",124);
	int i;
	for (i=0; i<buffer_size; i++){
		pCryptText[i] = (CK_BYTE)buffer[i];
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
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;

	hal_printf("received encrypted Text: ");PrintHex(pData,ulDataLen);
	bool ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, ddata, &ulDataLen);
	if(!ret){hal_printf("Decryption Failed\n");}
	hal_printf("Decrypted Text (%d): ", ulDataLen);PrintHex(ddata,ulDataLen);
	hal_printf("\n\n");

	//hal_printf("Original Text: ");PrintHex(pData,ulDataLen);
	//bool ret= Crypto_Encrypt(pkey,32,IV, 48, pData, ulDataLen, pCryptText, ulCryptLen);
	//if(!ret){hal_printf("Encryption Failed\n");}
	//hal_printf("Encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	//ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, ddata, ulDataLen);
	//if(!ret){hal_printf("Decryption Failed\n");}
	//hal_printf("Decrypted Text: ");PrintHex(pData,ulDataLen);
	//hal_printf("\n\n  ");
}

