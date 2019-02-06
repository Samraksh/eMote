sudo openvpn --mktun --dev tun0 
sudo ip link set tun0 up
sudo ip addr add 192.168.200.5/24 dev tun0
