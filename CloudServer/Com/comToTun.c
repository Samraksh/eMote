/*
Summary:COM port to TUN interface bridge.
Author: Mukundan Sridharan

This program acts as a bridge between a com port and a TUN interface. The TUN interface should already be setup.
The assumption is that the COM port receives and send raw IP packets and this packets needs to be forwarded to the 
TUN interface so that a "regular" desktop application can then process it.
This is meant for testing IP based applications on embedded devices based on Samraksh's eMote OS. 
If you dont use IP stack on the eMote, you should not be using this program.
*/


#include <stdio.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TUN_MTU 1500


char readBuffer[1500];


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

int ReadTun(int tun_fd, char* tun_name)
{
    int nread;
    // Now read data coming from the kernel 
    while(1) {
        // Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes 
        nread = read(tun_fd,readBuffer,sizeof(readBuffer));
        if(nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }
    }

    // Do whatever with the data 
    printf("Read %d bytes from device %s\n", nread, tun_name);
    return 1;
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

    printf("Wrote %d bytes to Tun fd %d\n", nwrote, tun_fd);
    return 1;
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
    }

    // try to create the device 
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        close(fd);
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

//main starts here
int main(){
    int tunfd=-1;
    char tunName[4]="tun0";
    tunfd=OpenTunDev(tunName);

    //fork a thread and read COM in it

    //fork a thread and read Tun in it
    if(tunfd){
        ReadTun(tunfd, tunName);
        return 0;
    }
    return -1;
}