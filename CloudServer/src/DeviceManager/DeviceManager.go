package DeviceManager

type DeviceStatus uint8

//DeviceStatus
const (
	DS_UnKnown DeviceStatus = iota + 1 //we dont know whats going on
	DS_UnInit                          //New Device
	DS_SecComm                         //ECDH is done, Secure comm is enables
	DS_Good                            //Everything is working well
)

/*type struct DeviceManager {}

func (dm* DeviceManager) Init () {
	InitDB()
}*/
