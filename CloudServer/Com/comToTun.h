#ifndef _COM_TO_TUN_H
#define _COM_TO_TUN_H

#include <stdio.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>      // Error number definitions
#include <termios.h>
#include <locale.h>
#include <stdbool.h>

//Tun Methods


//Com Methods
int OpenCom(char* devname);
int SetComAttribs (int fd, int speed, int parity);
void SetBlocking (int fd, int should_block);
int ReadComPkt(int dev, char* buf, int buf_size, bool verbose);
int WriteToCom(int dev, char * buf, int size);

//Utils
void PrintMem(char *buf, int size);

#endif //_COM_TO_TUN_H