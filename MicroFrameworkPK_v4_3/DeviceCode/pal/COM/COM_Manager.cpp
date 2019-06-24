#include <tinyhal.h>
#include <..\..\..\..\..\..\pal\PKCS11\CryptokiPAL.h>
#include <crypto.h>

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
	hal_printf("\n");
}


void Message_Encrypt_Data(uint8_t *buffer, uint16_t buffer_size){
	
}

void Message_Decrypt_Data(uint8_t *buffer, uint16_t buffer_size){
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

	//hal_printf("IV : ");
	//PrintHex(IV,48);
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;

	hal_printf("\r\nreceived encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	bool ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, ddata, &ulDataLen);
	if(!ret){hal_printf("Decryption Failed\n");}
	//hal_printf("Decrypted Text (%d): ", ulDataLen);PrintHex(ddata,ulDataLen);
	//hal_printf("\r\n");
	hal_printf("\r\n");

	hal_printf("Decrypted Text: ");
	for (int j=1; j< ddata[0] + 1; j++){
		hal_printf("%c",(char)ddata[j]);
	}
	hal_printf("\r\n");
	hal_printf("\r\n");

	hal_printf("Forwarding data to Compute Processor\r\n\r\n");

	CPU_Timer_Sleep_MicroSeconds(400000,ADVTIMER_32BIT);

	hal_printf("Received data from Compute Processor: ");

	int dataLen = ddata[0];
	data[0] = dataLen;
	for (int j=0; j< dataLen; j++){
		hal_printf("%c",(char)ddata[dataLen - j]);
		data[j + 1] = ddata[dataLen - j];
	}
	hal_printf("\r\n\r\n");

	hal_printf("Encrypting data\r\n");
	ret= Crypto_Encrypt(pkey,32,IV, 48, data, dataLen, pCryptText, ulCryptLen);

	if(!ret){hal_printf("\r\nEncryption Failed\r\n");}
	hal_printf("Encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	hal_printf("\r\n");

	sendBTPacket(&pCryptText[0], ulCryptLen);

	/*CK_BYTE Data[4];
	for (int j=0; j<4; j++){
		Data[j] = (CK_BYTE)(j*2);
	}
	ulDataLen = 4;
	hal_printf("\r\nOriginal Text: ");PrintHex(Data,ulDataLen);
	ret= Crypto_Encrypt(pkey,32,IV, 48, Data, ulDataLen, pCryptText, ulCryptLen);
	if(!ret){hal_printf("\r\nEncryption Failed\r\n");}
	hal_printf("Encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, ddata, &ulDataLen);
	if(!ret){hal_printf("\r\nDecryption Failed\r\n");}
	hal_printf("Decrypted Text: ");PrintHex(ddata,ulDataLen);
	hal_printf("\r\n  ");*/
}

