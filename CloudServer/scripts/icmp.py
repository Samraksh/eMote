import os
from fcntl import ioctl
import struct
import time
import random

# pip install pypacker==4.0
from pypacker.layer3.ip import IP
from pypacker.layer3.icmp import ICMP

TUNSETIFF = 0x400454ca
IFF_TUN   = 0x0001
IFF_NO_PI = 0x1000

ftun = os.open("/dev/net/tun", os.O_RDWR)
ioctl(ftun, TUNSETIFF, struct.pack("16sH", b"tun0", IFF_TUN | IFF_NO_PI))

req_nr = 1
req_id = random.randint(1, 65000)
while True:
    icmp_req = IP(src_s="192.168.5.31", dst_s="192.168.5.100", p=1) +\
        ICMP(type=8) +\
        ICMP.Echo(id=req_id, seq=req_nr, body_bytes=b"povilas-test")
    os.write(ftun, icmp_req.bin())
    time.sleep(1)
    req_nr += 1
