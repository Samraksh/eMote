#include "comToTun.h"
#include "netif_parser.h"

UINT8 startstopBuf[NETIF_START_STOP_CHAR_SIZE]= {
    NETIF_START_STOP_CHAR, NETIF_START_STOP_CHAR, NETIF_START_STOP_CHAR
};


//Sandwitch the packet inbetween the start and stop bytes
int WriteToNetIfParser(int dev, char * buf, int size)
{
    if(write( dev, startstopBuf, NETIF_START_STOP_CHAR_SIZE)!=NETIF_START_STOP_CHAR_SIZE){
        perror("WriteToNetIF1: Didnt write all of the start stop chars \n");
    }
    int n_written = 0;
    int i=0;
    int batchSize=4;
    //n_written+=write( dev, buf, size);
    usleep (1000);
    //write char by charector, to enable reading on the other side
    while(size>0){
        if(size >=batchSize){
            n_written+=write( dev, &buf[i], batchSize);
        }else {
            n_written+=write( dev, &buf[i], size);    
        }
        printf("%d ",size);
        i+=batchSize; size-=batchSize;
        usleep (1000);
    }
    printf("Done.");

    if(write( dev, startstopBuf, NETIF_START_STOP_CHAR_SIZE)!=NETIF_START_STOP_CHAR_SIZE){
        perror("WriteToNetIF2: Didnt write all of the start stop chars \n");
    }
 
	usleep (1000); // sleep for 100 usec to let the port transmit the chars
	return n_written;
}

/*
void ParsePkt(int comfd, UINT8 *palBuf, int nChar){
	UINT8 * palBuf;
	UINT32 detectState=0;

	BOOL pktStartDetected=FALSE;
	BOOL pktEndDetected=FALSE;
	//if(ComPortNum == ConvertCOM_ComPort(COM_NETIF)){
	//	size_t nChar= State.ManagedRxQueue.NumberOfElements();
		if (nChar >=NETIF_MIN_PKT_SIZE){
			palBuf=State.ManagedRxQueue.Storage();
			for (int i=0; i < nChar; i++){
				if(palBuf[i]==NETIF_START_STOP_CHAR){
					detectState++;
				}else{
					if(detectState >0) detectState=0; //if you skip a single byte go back to zero.
				}
				if (detectState == NETIF_START_STOP_CHAR_SIZE){
					if(~pktStartDetected){
						//This start of a pkt
						pktStartDetected=TRUE;
						startLoc= i +1;

					}else {
						//This is the end of a pkt
						pktEndDetected=TRUE;
						endLoc= i - NETIF_START_STOP_CHAR_SIZE;
					}
				}
			}

			if(pktStartDetected && pktEndDetected) {
				//Copy pkt to buf and signal
				UINT16 nRead=USART_Driver::ManagedRead( ComPortNum, netifBuf, NETIF_BUF_SIZE);
				if(nRead >= endLoc+2*NETIF_START_STOP_CHAR_SIZE){
					hal_printf("Soft_Assert: LetNetIFHigherUpKnow: Something wrong, didnt read the whole net pkt\n");
				}
				USART_Driver::SetEvent( ComPortNum, USART_EVENT_DATA_NETIF );
			}
		}
	}
}
*/
