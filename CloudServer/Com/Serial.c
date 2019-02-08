#include "comToTun.h"

int errno;

int OpenCom(char* devname){
	int dev=-1;
	printf("Trying to open device %s\n" ,devname );
	dev = open (devname, O_RDWR| O_NOCTTY | O_SYNC);
	if (!dev){
		fprintf(stderr, "Error: Device Open failed: %s \n", strerror(errno));
	}
	return dev;
}

/* Values for Speed are: B115200, B230400, B9600, B19200, B38400, B57600, B1200, B2400, B4800, etc.
 * Values for parity are: 0 (meaning no parity), PARENB|PARODD (enable parity and use odd), 
 * 	   PARENB (enable parity and use even),
 *     PARENB|PARODD|CMSPAR (mark parity), and PARENB|CMSPAR (space parity).
 */
int 
SetComAttribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf(stderr, "error %s from tcgetattr\n", strerror(errno));
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB; //CSTOPB is 2 stop bits, ~CSTOPB is 1 stop bit
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		fprintf(stderr, "error %s from tcsetattr", strerror(errno));
		return -1;
	}
	return 0;
}

void
SetBlocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf (stderr, "Error tcgetattr: %s\n", strerror (errno));
		return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		fprintf(stderr, "error %s setting term attributes", strerror(errno));
	}
}

int 
WriteToCom(int dev, char * buf, int size)
{
	int n_written = write( dev, buf, size);
	usleep (100); // sleep for 100 usec to let the port transmit the chars
	return n_written;
}

int 
ReadCom(int dev, char* buf, int buf_size, bool verbose){
	//char buf [1024];
	int badFileDescriptor=0;
	int totalRead=0, n=0;
	//do {
		memset(buf, '0', buf_size);
		n = read( dev, buf, buf_size);
		if (n < 0) {
			badFileDescriptor++;
			printf("Error reading: %s \n", strerror(errno));
		}
		else if (n == 0) {
			badFileDescriptor=0;
			//printf("Read nothing!");
		}
		else {
			badFileDescriptor=0;
			if(verbose) {printf("%s",buf);}
		}
		totalRead+=n;
	//} while(n > 0);
	return totalRead;
}