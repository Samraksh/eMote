Cloud Server Notes:

Linux Desktop Server setup:

- Install a ubuntu LTS system
- Install prereqs (script available)
- Change the permissions for dumpcap (script)
- Create virtual interfaces (script available)
- Forward the DotNow/SF2 com ports to the virutal machines
- Forward the comport to veth0 (script available)



To get a simple channel between a socket and usart to:


#forwards the raw bytes from com port to udp:ip:port
socat -s /dev/ttyUSB0,b115200,raw,echo=1 UDP:192.168.5.200:6001

#Listerns to port (-u fo udp) and prints on standard output
nc -u -l 192.168.5.200 6001
