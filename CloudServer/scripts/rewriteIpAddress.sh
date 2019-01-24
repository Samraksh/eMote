#!/bin/bash

if (($# != 2)); then
	echo "$0 input_pcap ouput_pcap"
	exit
fi
#tcpprep --port --pcap=$1 --cachefile=$1.cache
#tcprewrite --cachefile=$1.cache --endpoints=10.2.100.1:10.2.100.100 --infile=$1 --outfile=$2
tcprewrite -b --srcipmap=10.1.10.15:10.2.10.15 --dstipmap=10.1.10.15:10.2.10.15 --infile=$1 --outfile=$2
rm $1.cache
