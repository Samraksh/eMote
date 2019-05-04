Cloud Server Notes:

Setting up Go Server:

packages Dependencies:
 go get github.com/stretchr/testify/assert
 go get golang.org/x/crypto/curve25519




Linux Desktop Server setup:

- Install a ubuntu LTS system
- Install prereqs (script available)
- Change the permissions for dumpcap (script)
- Create virtual interfaces (script available)
- Forward the DotNow/SF2 com ports to the virutal machines
- Forward the comport to veth0 (script available)



To get a simple channel between a socket and usart to:


Forwarding packets from comport to network port
================================================
Note: We are not using either of this, but these could be useful tools


Option1: socat
#forwards the raw bytes from com port to udp:ip:port
socat -s /dev/ttyUSB0,b115200,raw,echo=1 UDP:192.168.5.200:6001

Option2:
another optin is to use ser2net, this is a service and can be started and stoped automatically
apt install ser2net
- installs a config file at /etc/ser2net.conf
- service ser2net start/stop

Useful commands/tools:
=====================

#Listerns to port (-u fo udp) and prints on standard output, nc is short for netcat
nc -u -l 192.168.5.20a0 6001

#If you want to find out what process is running on a port
sudo netstat -ltnp | grep -w ':6001'

