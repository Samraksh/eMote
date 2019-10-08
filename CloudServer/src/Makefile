CC=go build
SDIR=./services

all: Core Gateway

core:Core

gtwy: Gateway

Core:
	$(CC) -o $@.exe $(SDIR)/$@/$@.go $(SDIR)/$@/pktHandler.go 

Gateway:
	$(CC) -o $@.exe $(SDIR)/$@/$@.go $(SDIR)/$@/BtGatt.go

clean:
	rm Core Gateway Core.exe Gateway.exe