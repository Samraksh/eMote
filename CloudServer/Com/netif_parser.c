#include "comToTun.h"
#include "netif_parser.h"

//extern sem_t mutex;

//globals
char startstopBuf[NETIF_START_STOP_CHAR_SIZE]= {
    NETIF_START_STOP_CHAR, NETIF_START_STOP_CHAR, NETIF_START_STOP_CHAR
};

char netifBuf[NETIF_BUF_SIZE]; 
char tempBuf[NETIF_BUF_SIZE]; 
int startLoc, endLoc=0;
int detectState=0;
bool pktStartDetected=false;
bool pktEndDetected=false;


bool ParsePkt(char *palBuf, int nChar){
	//UINT8 * palBuf;
	//if(ComPortNum == ConvertCOM_ComPort(COM_NETIF)){
	//	size_t nChar= State.ManagedRxQueue.NumberOfElements();
	//printf("ParsePkt: Got %d bytes:", nChar);
	//PrintMem(palBuf, nChar);
	pktStartDetected=false;pktEndDetected=false;
	detectState=0;
	if (nChar >=NETIF_MIN_PKT_SIZE){
		//printf("Looking at stuff\n");
		//palBuf=State.ManagedRxQueue.Storage();
		for (int i=0; i < nChar; i++){
			//printf("B[%d]: %X ",i,palBuf[i]);fflush(stdout);
			if((palBuf[i] & 0xFF)==NETIF_START_STOP_CHAR){
				detectState++;
				//printf("Detected a char ");
			}else{
				if(detectState >0) detectState=0; //if you skip a single byte go back to zero.
			}
			if (detectState == NETIF_START_STOP_CHAR_SIZE){
				if(!pktStartDetected){
					//This start of a pkt
					pktStartDetected=true;
					startLoc= i +1;
					//printf("Detected a start ");

				}else {
					//This is the end of a pkt
					pktEndDetected=true;
					endLoc= i - NETIF_START_STOP_CHAR_SIZE;
					break;
					//printf("Detected a end ");
				}
			}
		}

		if(pktStartDetected && pktEndDetected) {
			//Copy pkt to buf and signal
			//UINT16 nRead=USART_Driver::ManagedRead( ComPortNum, netifBuf, NETIF_BUF_SIZE);
			if(nChar >= endLoc+2*NETIF_START_STOP_CHAR_SIZE){
				//hal_printf("Soft_Assert: LetNetIFHigherUpKnow: Something wrong, didnt read the whole net pkt\n");
			}
			//printf("Found a packet end ");
			return true;			
		}	
	}
	return false;
}
static int nread=0;
int ReadNetIfParser(int dev, char* buf, int buf_size, bool verbose){
	
	bool foundPkt=false;
	int pktSize = -1;
	while (!foundPkt){
		nread+=ReadCom(dev,&netifBuf[nread],NETIF_BUF_SIZE-nread,verbose);		
		foundPkt = ParsePkt(netifBuf, nread);
	}
	if(foundPkt){
		pktSize=(endLoc-startLoc)+1;
		if(pktSize > buf_size) {
			printf("ReadNetIfParser: Pkt too big. This shouldnt be happening...\n");
		}
		memcpy(buf,&netifBuf[startLoc], pktSize);
		//There are more byts in the buffer
		if(nread>pktSize+2*NETIF_START_STOP_CHAR_SIZE){
			printf("Rearange buf:\n");
			int newStart=endLoc+NETIF_START_STOP_CHAR_SIZE+1;
			memcpy(tempBuf,&netifBuf[newStart], nread-newStart);
			memcpy(netifBuf, tempBuf, nread-newStart);
			nread-=newStart;
		}else {
			memset(netifBuf,'\0',NETIF_BUF_SIZE);
			nread=0;
		}
		startLoc=-1;endLoc=-1;
	}

    return pktSize;
}


//Sandwitch the packet inbetween the start and stop bytes
int WriteToNetIfParser(int dev, char * buf, int size)
{
	//sem_wait(&mutex);
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
        usleep (5000);
    }
    //printf("Done.");

    if(write( dev, startstopBuf, NETIF_START_STOP_CHAR_SIZE)!=NETIF_START_STOP_CHAR_SIZE){
        perror("WriteToNetIF2: Didnt write all of the start stop chars \n");
    }
	//sem_post(&mutex);

	usleep (1000); // sleep for 100 usec to let the port transmit the chars
	return n_written;
}


