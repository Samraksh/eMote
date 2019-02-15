/*
Summary:COM port to TUN interface bridge.
Author: Mukundan Sridharan

This program acts as a bridge between a com port and a TUN interface. The TUN interface should already be setup.
The assumption is that the COM port receives and send raw IP packets and this packets needs to be forwarded to the 
TUN interface so that a "regular" desktop application can then process it.
This is meant for testing TCP/UDP/IP based applications on embedded devices based on Samraksh's eMote OS. 
If you dont use IP stack on the eMote, you should not be using this program.
*/

#include "comToTun.h"
#include <pthread.h>
#include <string.h>

#define TUN_MTU 1500
const bool verboseMode=true;

char tunReadBuf[TUN_MTU];
char comReadBuf[TUN_MTU];
 int tunfd=-1;
 int comfd=-1;



//function prototypes
int AllocTun(char *dev, int flags);

// tunclient.c
int OpenTunDev(char * _tunName)
{
    char tun_name[IFNAMSIZ];
    int tun_fd;

    // Connect to the device 
    strcpy(tun_name, _tunName);
    tun_fd = AllocTun(tun_name, IFF_TUN | IFF_NO_PI);  // tun interface 
    if(tun_fd < 0){
        perror("Allocating new interface");
        exit(1);
    }else {
        printf("Tun interface %s exist, opened handle: %d \n", _tunName,tun_fd);
    }
    return tun_fd;
}

int ReadTun(int tun_fd)
{
    int nread;
    // Now read data coming from the kernel 
    
    // Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes 
    memset(tunReadBuf, '\0', TUN_MTU);
    //printf("ReadTun: reading from device %d\n",tun_fd);
    nread = read(tun_fd,tunReadBuf,sizeof(tunReadBuf));
    if(nread < 0) {
        perror("Reading from interface");
        close(tun_fd);
        exit(1);
    }

    // Do whatever with the data 
    //printf("ReadTun: Read %d bytes from port %d\n", nread, tun_fd);
    return nread;
}

int WriteTun(int tun_fd, char * buf, int nwrite)
{
    int nwrote;
    if(nwrite > TUN_MTU){
        return -1;
    }
    
    nwrote = write(tun_fd,buf,nwrite);
    if(nwrote < 0) {
        perror("Writing to Tun interface, failed");
        //close(tun_fd);
        //exit(1);
    }

    printf("WriteTun: Wrote %d bytes to Tun fd %d\n", nwrote, tun_fd);
    return nwrote;
}


int AllocTun(char *dev, int flags) 
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";

    /* Arguments taken by the function:
    *
    * char *dev: the name of an interface (or '\0'). MUST have enough
    *   space to hold the interface name if '\0' is passed
    * int flags: interface flags (eg, IFF_TUN etc.)
    */

    
    // open the clone device 
    if( (fd = open(clonedev, O_RDWR)) < 0 ) {
        perror("Unable to open clonedev: ");
        return fd;
    }

    // preparation of the struct ifr, of type "struct ifreq"
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;   // IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI 
    if (*dev) {
        /* if a device name was specified, put it in the structure; otherwise,
        * the kernel will try to allocate the "next" device of the
        * specified type */
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
        printf("Opening device %s\n",ifr.ifr_name);
    }

    // try to create the device 
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        close(fd);
        perror("ioctl error on interfce: ");
        return err;
    }

    /* if the operation was successful, write back the name of the
    * interface to the variable "dev", so the caller can know
    * it. Note that the caller MUST reserve space in *dev (see calling
    * code below)  */
    strcpy(dev, ifr.ifr_name);

    // this is the special file descriptor that the caller will use to talk
    // with the virtual interface 
    return fd;
}

//Blocking method, make sure to call from a thread.
void* ReadTunWriteCom(void *_tunfd){
    int tunfd=*(int*)_tunfd;
    printf("tUNRead: Starting up on fd %d...\n",tunfd);
    if(tunfd < 0){
        printf("Couldn't access TUN port(%d).. Exiting.\n",tunfd);
        exit(-1);
    }

    while (tunfd>=0){
        //this is a blocking call
        int nread=ReadTun(tunfd);
        
        if(nread> 0){
            if(verboseMode){
                printf("Tun pkt: ");PrintMem(tunReadBuf, nread);
            }
            if(WriteToCom(comfd, tunReadBuf, nread)!= nread){
                printf("ReadTunWriteCom: Did not write everything to Com that I read from Tun...\n");
            }else {
                 printf("ReadTunWriteCom: Wrote %d bytes to Com\n", nread);
            }
        }
        else {
            printf("ReadTunWriteCom: Did not read anything from tun\n");
        }
        usleep(1000);
    }
}


void* ReadComWriteTun(void *_comfd){
    int comfd=*(int*)_comfd;
    printf("ComRead: Starting up on fd %d...\n",comfd);

    while (comfd>=0){
         //this is a blocking call. T
        int nread=ReadComPkt(comfd, comReadBuf, TUN_MTU, false);

        if(nread> 0){
            if(verboseMode){
                printf("Com pkt: "); PrintMem(comReadBuf,nread);
            }
            if(WriteTun(tunfd, comReadBuf, nread)!= nread)
            {
                printf("ComReadWriteTun: Did not write everything to Tun that I read from Com...\n");
            }else {
                printf("ComReadWriteTun: Wrote %d bytes to Tun\n", nread);
            }
        }
        else {
            printf("ComRead: Did not read anything\n");
        }
        usleep(1000);

    }
}


void Usage(){
	printf("Usage: ./comToTun DeviceName \n\n");
	printf("\tDevice Name: eg. ttyUSB0. /dev will be prepended to it\n");
	/*printf("\t StartupDelay: Number of seconds to wait before starting sampling\n");
	printf("\tRunTime unit is secs. A value of -1 will run for ever.\n");
	printf("\tVerbose: (0 or 1) prints on screen (optional parameter)\n\n");
	printf("\tOutPutFile is the name of the file to store sampled data\n");
    */
}

//bool ParseArgs(int argc, char** argv, string* devname, int *startupDelay, int *runTime, string *filename, bool *verbose){
bool ParseArgs(int argc, char** argv, char* devname){
	/*if(argc<4){
		printf("Error: Not enough arguments.");
		Usage(); exit(-1);
	}*/
    if(argc >= 2){
	    strcpy(devname,argv[1]);
    }else {
        strcpy(devname,"ttyUSB0");
    }
	//*startupDelay=atoi(argv[2]);
	//*runTime=atoi(argv[3]);
	printf("Received arguments : %d \n",argc);
	
	/*if(argc >= 5){
		*verbose=bool(atoi(argv[4]));
		printf("Read verbose :  %d\n" , *verbose);
	}
	
	if(argc==6){
		*filename=argv[5];
	}else {
		*filename= string(*devname).append("_").append(GetDateString()).append(".det");
	}*/

	return true;
}

//main starts here
int main(int argc, char **argv){
   
    char deviceName[16]="/dev/";
    char tmpName[16];
    ParseArgs(argc, argv, tmpName);
    strcat(deviceName,tmpName);

    pthread_t tunThreadId=-1;
    pthread_t comThreadId=-1;

    char tunName[5]="tun0";
    tunName[4]='\0';
    tunfd=OpenTunDev(tunName);
    comfd=OpenCom(deviceName);
    SetComAttribs(comfd, B115200, 0);
	SetBlocking(comfd,1); //let com be blocking

    //Sanity check, make sure both ports are open
    if(tunfd<0 || comfd< 0){
        printf("Couldn't open the COM Port (%d) or the TUN port(%d).. Exiting.\n",comfd,tunfd);
        exit(-1);
    }
    printf("Opened both ports sucessfully, begining to bridge...\n");


    //fork a thread and read COM in it
    int err = pthread_create(&comThreadId, NULL, &ReadComWriteTun, (void*)&comfd);
    if(err) 
    {
        printf("Problem creating the COM thread\n"); exit(-1);
    }

    //fork a thread and read Tun in it
    err = pthread_create(&tunThreadId, NULL, &ReadTunWriteCom, (void*)&tunfd);
    if(err) {
        printf("Problem creating the tun thread\n"); exit(-1);
    }

   
    //our threads never quit unless a error. So need a exception handler method.
    pthread_join(tunThreadId, NULL);
    pthread_join(comThreadId, NULL);

    return -1;
}