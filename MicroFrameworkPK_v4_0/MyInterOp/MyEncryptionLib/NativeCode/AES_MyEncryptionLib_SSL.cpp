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


#include <tinyhal.h>
#include "AES.h"
#include "crypto/aes_native.c"
#include "AES_MyEncryptionLib_SSL.h"

int done = 0;

extern uint16_t RxBuffer[];
extern UINT8* AES_Encrypt(const char *str) ;

using namespace MyEncryptionLib;

INT32 SSL::encrypt( CLR_RT_HeapBlock* pMngObj, LPCSTR param0, HRESULT &hr )
{
    INT32 retVal = 0; 
	
	
	int length = 0;
	int count = 0;
	int clen= 0;
	UINT8 encVal[16] = {0};
//	LED_RED();
	//char* text = (char *)param0;
	
	
	
	/*while((char *)param0[length]!='\0') {
	length++;
	} */
	while(1)
	{
		while(!done)
		{
		
		
		}
		done = 0;
	
		AES_Encrypt((char *)param0, encVal,16);

		//hal_printf("%s",(unsigned char*) encVal);
		
		debug_printf("\n");
		debug_printf("##!");
	   for(int i = 0; i < 16; i++)
		{
			debug_printf("%02x",(char *)encVal[i]);
		}
		debug_printf("\n");
   // debug_printf("Done!");
	}
	//AES_Encrypt((char *)param0, encVal,16);
	//debug_printf("%d",length);
/*	while(clen<length) {
	if((char *)text[clen] == "+")
	{
		count++;
		if(count==2)
		 { 
		 int toLength = 16-clen;
		 char *to = (char*) malloc(toLength);
		// for(int j=0;j<=clen;j++)
		//	debug_printf("%c",(char *)text[j]);
		// for(int i=0;i<16-length;i++)
		//	to[i] = text[toLength++];
		 AES_Encrypt(to, encVal,length);	
		
		 }
	}
	clen++;
	}
	*/

	//AES_Encrypt(text,encVal);
	// debug_printf("\n");
	// debug_printf("##!");
	// for(int i = 0; i < 16; i++)
	// {
		// debug_printf("%02x",(char *)encVal[i]);
	// }
	// debug_printf("\n");
   // debug_printf("Done!");
	return retVal;
}

INT32 SSL::decrypt( CLR_RT_HeapBlock* pMngObj, LPCSTR param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

