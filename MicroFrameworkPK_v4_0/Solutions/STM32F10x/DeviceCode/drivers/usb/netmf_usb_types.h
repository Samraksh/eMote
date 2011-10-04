struct SETUP_PACKET
{
  UINT8 USBbmRequestType;       /* bmRequestType */
  UINT8 USBbRequest;            /* bRequest */
  UINT16 USBwValues;         /* wValue */
  UINT16 USBwIndexs;         /* wIndex */
  UINT16 USBwLengths;        /* wLength */    
};

SETUP_PACKET g_Setup_Packet;