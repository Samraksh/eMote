combined*.bin is a flash memory dump of a full deployement including:

TinyBooter
TinyCLR
Scrolling Hello World

Applying the image puts the mote into a known state and used in initial deployment.

The image is applied via openOCD with the command:
flash write_image _filename_ 0x8000000