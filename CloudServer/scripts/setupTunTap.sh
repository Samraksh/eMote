sudo openvpn --mktun --dev tun0 --persist-tun 
sudo ip link set tun0 up
sudo ip addr add 192.168.5.200/24 dev tun0
#sudo ip route add 192.168.5.0/24 via 192.168.5.200 dev tun0
echo 1| sudo tee  /proc/sys/net/ipv4/ip_forward
sudo ip route add 192.168.5.0/24 dev tun0 proto kernel scope link src 192.168.5.200
