package main

import (
	Def "Definitions"
	"crypto/aes"
	"crypto/cipher"
	"crypto/hmac"
	"crypto/sha256"
	"fmt"
	"io/ioutil"
	"log"
)

//key, _ := hex.DecodeString("6368616e676520746869732070617373776f726420746f206120736563726574")
var exkey = []byte{0xb8, 0xd4, 0x23, 0x98, 0x20, 0x1b, 0x53, 0xbd,
	0x47, 0xe2, 0x7e, 0x93, 0x3c, 0xb5, 0x91, 0xb6,
	0xac, 0xa8, 0x86, 0x36, 0x51, 0x93, 0xa8, 0x8d,
	0x5f, 0x02, 0x30, 0x6f, 0xb9, 0xea, 0x23, 0xab}

var key1 = []byte{0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
	0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
	0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
	0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08}

//12 bytes or 96 bits
var exiv = []byte{0x15, 0xde, 0x34, 0xb4, 0x78, 0xc9, 0xf4, 0x13, 0x14, 0xa3, 0xcb, 0x2a}

//MSUpdate constats
const totalBufferSize int = 522
const markerSize int = 7
const opCode int = 4
const param int = 4
const maxDataSize int = totalBufferSize - markerSize - opCode - param

//global variables
var fileLength int = 0
var sendArrayLines int = 0
var sendPacketNum int = 0

type BufferState int

const (
	BS_Start byte = iota + 1
	BS_Idle
	BS_Sending
	BS_End
	BS_Indiv
)

type SecureComs struct {
	inFileXferMap   map[string]bool
	sendBufferState byte
	outMsgChan      *chan Def.GenMsg
	sendArray       [][]byte
}

/*
func (sc *SecureComs) SetFileXferMap(device string) {
	if sc.inFileXferMap[device] == nil {
		sc.inFileXferMap[device] = make(bool, true)
	}
}
*/

func NewSecureComs(outMsgChan *chan Def.GenMsg) *SecureComs {
	sc := new(SecureComs)
	sc.inFileXferMap = make(map[string]bool)
	sc.outMsgChan = outMsgChan
	//sc.sendArray = make([]byte, 1)
	return sc
}

func (sc *SecureComs) HandleIncoming(msg []byte, addr string) {
	switch msg[0] {
	//device is requesting new binary
	case Def.M_SEC_BIN_RQ:
		sc.StartSecureFile(msg, addr)
		break
	//ongoing binary update message
	case Def.M_SEC_BIN_RES:
		sc.binaryUpdateHandler(msg, addr)
		break
	//This is secure data message, need to forward to application
	case Def.M_SEC_D_CH:
		sc.ForwardToApp(msg, addr)
		break
	// We should be not be getting this, the core sends status request
	case Def.M_SEC_STATUS_RQ:
		log.Println("SecureComs: Why is device ", addr, " sending a status req to the core?")
		break

	//Response/periodic update from device to a status req
	case Def.M_SEC_STATUS_RES:
		sc.StatusHandler(msg, addr)
		break

	case Def.M_SEC_DETECT
		fmt.Println("*** Remote detect from 'IoT Device #1'!")
		break;
	default:
		fmt.Println("PktHndlr::RouteMsg: Received an unknown message type, droping it.")
	}
}

//AddTo2DArray: Adds a byte array from the 2D byte array sendArray
func (sc *SecureComs) addTo2DSendArray(newbytes []byte) {
	toAdd := [][]byte{newbytes}
	sc.sendArray = append(sc.sendArray, toAdd...)
	/*if sc.sendArray == nil {
		sc.sendArray = newbytes
	} else {
		sc.sendArray = append(sc.sendArray, newbytes...)
	}*/
}

func (sc *SecureComs) ForwardToApp(msg []byte, device string) {
	//Decrypt message and forward to app
}

func (sc *SecureComs) StatusHandler(msg []byte, device string) {
	//Decrypt message and forward to app
}

func (sc *SecureComs) binaryUpdateHandler(msg []byte, device string) {
	log.Println("Receive a binary update protocol message: ")
	//byte[] data;
	//Decrypt message

	for i := 0; i < len(msg); i++ {
		fmt.Printf("%X ", msg[i])
	}
	fmt.Println("")

	if sc.sendBufferState == BS_Sending {
		log.Println("for now we just advance the state machine")
		sc.sendStateMachine(device)
	}
}

//remove a byte array from the 2D byte array sendArray
func (sc *SecureComs) extractFrom2DSendArray(index int) []byte {
	if len(sc.sendArray)-1 > index {
		panic("Index not found")
	}
	ret := sc.sendArray[index]
	return ret
}

func (sc *SecureComs) StartSecureFile(msg []byte, addr string) {
	sc.inFileXferMap[addr] = true
	log.Println("Device ", addr, " has requested new binary.")
	openfile, err := sc.ReadFileFromStorage(addr)
	CheckError(err)
	var secureFile []byte
	secureFile, err = sc.Encrypt(openfile, addr)
	CheckError(err)
	hash := sc.HMAC(openfile, key1, addr)
	log.Println("=========================================")
	log.Println("HMAC of the file update sent to device ", addr, " is: \n", hash)
	log.Println("=========================================")
	log.Println(" Starting the statemachine...")
	//sc.sendBufferState=BS_Idle
	sc.BeginFileXfer(secureFile, addr)
}

func (sc *SecureComs) BeginFileXfer(filebytes []byte, addr string) {
	log.Println("Starting transfer of encrypted file of length: ", len(filebytes))
	//if sc.sendBufferState == BS_Idle {
	fileLength = len(filebytes)
	sendArrayLines = int((fileLength / maxDataSize) + 1)
	log.Println("File length: ", fileLength, " lines: ", sendArrayLines)
	//sc.sendArray = make([][]byte, sendArrayLines)

	for j := 0; j < sendArrayLines; j++ {
		//var dataB = new byte[maxTransferSize];
		start := j * maxDataSize
		var dataB []byte
		if j == sendArrayLines-1 {
			dataB = filebytes[start:]
		} else {
			dataB = filebytes[start : start+maxDataSize]
		}
		var dataLine = make([]byte, maxDataSize+markerSize+opCode+param)
		dataLine[0] = byte('M')
		dataLine[1] = byte('S')
		dataLine[2] = byte('p')
		dataLine[3] = byte('k')
		dataLine[4] = byte('t')
		dataLine[5] = byte('V')
		dataLine[6] = byte('1')

		log.Println("processing line: ", j, " total size: ", len(dataLine))

		for k := 0; k < len(dataB); k++ {
			dataLine[markerSize+opCode+param+k] = dataB[k]
		}
		sc.addTo2DSendArray(dataLine)
	}

	/*for k := 0; k < sendArrayLines; k++ {
		log.Println("printing line: ", k)
		//extractedData := sc.RemoveFromArray(k)
		extractedData := sc.sendArray[k]
		for m := 0; m < len(extractedData); m++ {
			log.Print(extractedData[m], " ")
		}
		log.Println("")
	}*/

	sc.sendBufferState = BS_Start
	sendPacketNum = 0
	sc.sendStateMachine(addr)
	//} else {
	//	log.Println("State is not idle")
	//	sc.sendStateMachine(addr)
	//}

}

func (sc *SecureComs) FinishSecureFile(msg []byte, addr string) {
	sc.inFileXferMap[addr] = false
}

func (sc *SecureComs) HMAC(msg []byte, key []byte, deviceId string) (hash []byte) {
	mac := hmac.New(sha256.New, key)

	//fmt.Println("creating hash of msg of len: ", len(m), ", MSG:", m)
	//create hmac
	mac.Write(msg)
	hash = mac.Sum(nil)
	return hash
}

func (sc *SecureComs) Encrypt(plaintext []byte, deviceId string) (ciphertext []byte, err error) {
	//key, _ := hex.DecodeString("6368616e676520746869732070617373776f726420746f206120736563726574")
	//plaintext := []byte("exampleplaintext")

	//key is 32 bytes. Using aes256
	block, err := aes.NewCipher(key1)
	if err != nil {
		panic(err.Error())
	}

	// Never use more than 2^32 random nonces with a given key because of the risk of a repeat.
	/*iv := make([]byte, 12)
	if _, err := io.ReadFull(rand.Reader, nonce); err != nil {
		panic(err.Error())
	}
	*/
	iv := exiv
	aesgcm, err := cipher.NewGCM(block)
	if err != nil {
		panic(err.Error())
	}

	ciphertext = aesgcm.Seal(nil, iv, plaintext, nil)
	//fmt.Printf("%x\n", ciphertext)
	return ciphertext, err
}

func (sc *SecureComs) Decrypt(ciphertext []byte, addr string) (plaintext []byte, err error) {
	iv := exiv
	block, err := aes.NewCipher(key1)
	if err != nil {
		panic(err.Error())
	}

	aesgcm, err := cipher.NewGCM(block)
	if err != nil {
		panic(err.Error())
	}

	plaintext, err = aesgcm.Open(nil, iv, ciphertext, nil)
	if err != nil {
		panic(err.Error())
	}

	//fmt.Printf("%s\n", plaintext)
	// Output: exampleplaintext
	return plaintext, err
}

func CheckError(e error) {
	if e != nil {
		log.Println(e.Error())
		panic(e)
	}
}

func (sc *SecureComs) ReadFileFromStorage(deviceId string) (filebytes []byte, err error) {
	log.Println("Reading binary file for device: ", deviceId)
	dat, err := ioutil.ReadFile("DeviceManager/db/files/cp.bin")
	CheckError(err)
	//fmt.Print(string(dat))
	return dat, err
}

func (sc *SecureComs) sendStateMachine(device string) {
	if sc.sendBufferState == BS_Start {
		sc.sendBufferState = BS_Sending
		log.Println("sending start command")
		startParam := 8
		startPacket := make([]byte, markerSize+opCode+startParam)
		startPacket[0] = byte('M')
		startPacket[1] = byte('S')
		startPacket[2] = byte('p')
		startPacket[3] = byte('k')
		startPacket[4] = byte('t')
		startPacket[5] = byte('V')
		startPacket[6] = byte('1')
		startPacket[7] = 0x00
		startPacket[8] = 0x02
		startPacket[9] = 0x00
		startPacket[10] = 0x56
		startPacket[11] = byte((fileLength & 0xff000000) >> 24)     // size of transfer byte0
		startPacket[12] = byte((fileLength & 0xff0000) >> 16)       // size of transfer byte1
		startPacket[13] = byte((fileLength & 0xff00) >> 8)          // size of transfer byte2
		startPacket[14] = byte((fileLength & 0xff))                 // size of transfer byte3
		startPacket[15] = byte((sendArrayLines & 0xff000000) >> 24) // array size byte0
		startPacket[16] = byte((sendArrayLines & 0xff0000) >> 16)   // array size byte1
		startPacket[17] = byte((sendArrayLines & 0xff00) >> 8)      // array size byte2
		startPacket[18] = byte((sendArrayLines & 0xff))             // array size byte3
		sc.dataSend(device, startPacket)
	} else if sc.sendBufferState == BS_Sending {
		if sendPacketNum < sendArrayLines {
			log.Println("add packet ", sendPacketNum, " array line number of ", sendArrayLines)
			extractedData := sc.sendArray[sendPacketNum]
			log.Println("Extracted data")
			extractedData[7] = 0x00
			extractedData[8] = 0x02
			extractedData[9] = 0x00
			extractedData[10] = 0x57
			extractedData[11] = (byte)(sendPacketNum & 0xff)
			sc.dataSend(device, extractedData)
			sendPacketNum++
		} else {
			sc.sendBufferState = BS_End
			log.Println("send install")
			installPacket := make([]byte, markerSize+opCode+param)
			installPacket[0] = byte('M')
			installPacket[1] = byte('S')
			installPacket[2] = byte('p')
			installPacket[3] = byte('k')
			installPacket[4] = byte('t')
			installPacket[5] = byte('V')
			installPacket[6] = byte('1')
			installPacket[7] = 0x00
			installPacket[8] = 0x02
			installPacket[9] = 0x00
			installPacket[10] = 0x58
			sc.dataSend(device, installPacket)
		}
	}
}

func (sc *SecureComs) dataSend(device string, msg []byte) {
	if sc.outMsgChan == nil {
		panic("No outgoing message channel present")
	}
	if msg != nil {
		*sc.outMsgChan <- Def.GenMsg{
			Addr: device,
			Msg:  msg,
		}
	}
}
