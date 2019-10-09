package main

import (
	"errors"
	"fmt"
	"log"
	"strings"
	"time"

	"github.com/paypal/gatt"
	"github.com/paypal/gatt/examples/option"
)

var done = make(chan struct{})

var cloudServiceId = gatt.MustParseUUID("0000ff1000001000800000805f9b34fb") // 6e400001-b5a3-f393-e0a9-e50e24dcca9e")
//var cloudServiceOpenCharId = gatt.MustParseUUID("0000ff1100001000800000805f9b34fc")
var cloudServiceSecureCharId = gatt.MustParseUUID("0000ff1100001000800000805f9b34fb")
var cloudServiceOpenCharId = gatt.MustParseUUID("0000ff1100001000800000805f9b34fd")

//BtGattRadio instantiates a radio
type BtGattRadio struct {
	clientID   string
	secChan    chan []byte
	openChan   chan []byte
	connected  bool
	periph     gatt.Peripheral
	SecureChar *gatt.Characteristic
	OpenChar   *gatt.Characteristic
}

///Returns the connected status of the radio
func (r *BtGattRadio) IsConnected() bool {
	return r.connected
}

func (r *BtGattRadio) onStateChanged(d gatt.Device, s gatt.State) {
	fmt.Println("State:", s)
	switch s {
	case gatt.StatePoweredOn:
		fmt.Println("Scanning...")
		d.Scan([]gatt.UUID{}, false)
		return
	default:
		d.StopScanning()
	}
}

func (r *BtGattRadio) onPeriphDiscovered(p gatt.Peripheral, a *gatt.Advertisement, rssi int) {

	if strings.ToUpper(p.ID()) != r.clientID {
		log.Printf("Client ID mismatch: Found client %s, looking for %s \n", p.ID(), r.clientID)
		return
	}

	// Stop scanning once we've got the peripheral we're looking for.
	p.Device().StopScanning()

	fmt.Printf("\nPeripheral ID:%s, NAME:(%s)\n", p.ID(), p.Name())
	fmt.Println("  Local Name        =", a.LocalName)
	fmt.Println("  TX Power Level    =", a.TxPowerLevel)
	fmt.Println("  Manufacturer Data =", a.ManufacturerData)
	fmt.Println("  Service Data      =", a.ServiceData)
	fmt.Println("")

	p.Device().Connect(p)
}

func (r *BtGattRadio) onPeriphEnumeration(p gatt.Peripheral, err error) {
	fmt.Println("Connected")
	defer p.Device().CancelConnection(p)

	if err := p.SetMTU(500); err != nil {
		fmt.Printf("Failed to set MTU, err: %s\n", err)
	}

	// Discovery services
	ss, err := p.DiscoverServices(nil)
	if err != nil {
		fmt.Printf("Failed to discover services, err: %s\n", err)
		return
	}

	for _, s := range ss {
		msg := "Service: " + s.UUID().String()
		if len(s.Name()) > 0 {
			msg += " (" + s.Name() + ")"
		}
		fmt.Println(msg)

		// Discovery characteristics
		cs, err := p.DiscoverCharacteristics(nil, s)
		if err != nil {
			fmt.Printf("Failed to discover characteristics, err: %s\n", err)
			continue
		}

		for _, c := range cs {
			msg := "  Characteristic  " + c.UUID().String()
			if len(c.Name()) > 0 {
				msg += " (" + c.Name() + ")"
			}
			msg += "\n    properties    " + c.Properties().String()
			fmt.Println(msg)

			// Read the characteristic, if possible.
			if (c.Properties() & gatt.CharRead) != 0 {
				b, err := p.ReadCharacteristic(c)
				if err != nil {
					fmt.Printf("Failed to read characteristic, err: %s\n", err)
					continue
				}
				fmt.Printf("    value         %x | %q\n", b, b)
			}

			// Discovery descriptors
			ds, err := p.DiscoverDescriptors(nil, c)
			if err != nil {
				fmt.Printf("Failed to discover descriptors, err: %s\n", err)
				continue
			}

			for _, d := range ds {
				msg := "  Descriptor      " + d.UUID().String()
				if len(d.Name()) > 0 {
					msg += " (" + d.Name() + ")"
				}
				fmt.Println(msg)

				// Read descriptor (could fail, if it's not readable)
				b, err := p.ReadDescriptor(d)
				if err != nil {
					fmt.Printf("Failed to read descriptor, err: %s\n", err)
					continue
				}
				fmt.Printf("    value         %x | %q\n", b, b)
			}

			// Subscribe the characteristic, if possible.
			if (c.Properties() & (gatt.CharNotify | gatt.CharIndicate)) != 0 {
				f := func(c *gatt.Characteristic, b []byte, err error) {
					fmt.Printf("notified: % X | %q\n", b, b)
				}
				if err := p.SetNotifyValue(c, f); err != nil {
					fmt.Printf("Failed to subscribe characteristic, err: %s\n", err)
					continue
				}
			}

		}
		fmt.Println()
	}

	fmt.Printf("Waiting for 5 seconds to get some notifiations, if any.\n")
	time.Sleep(5 * time.Second)
}

func (r *BtGattRadio) onOpenNotify(c *gatt.Characteristic, b []byte, e error) {
	log.Printf("Got a secure msg %s\n", string(b))

}

func (r *BtGattRadio) onSecureNotify(c *gatt.Characteristic, b []byte, e error) {
	log.Printf("Got a secure msg %s\n", string(b))
}

func (r *BtGattRadio) onPeriphConnected(p gatt.Peripheral, err error) {
	fmt.Println("Connected")
	defer p.Device().CancelConnection(p)

	if err := p.SetMTU(500); err != nil {
		fmt.Printf("Failed to set MTU, err: %s\n", err)
	}

	// Discovery services
	ss, err := p.DiscoverServices(nil)
	if err != nil {
		fmt.Printf("Failed to discover services, err: %s\n", err)
		return
	}

	for _, s := range ss {
		msg := "Service: " + s.UUID().String()
		if len(s.Name()) > 0 {
			msg += " (" + s.Name() + ")"
		}
		fmt.Println(msg)

		if s.UUID().Equal(cloudServiceId) {
			log.Printf("Cloud Service Found %s\n", s.Name())

			// Discovery characteristics
			cs, err := p.DiscoverCharacteristics(nil, s)
			if err != nil {
				fmt.Printf("Failed to discover characteristics, err: %s\n", err)
				continue
			}

			for _, c := range cs {
				msg := "  Characteristic  " + c.UUID().String()
				if len(c.Name()) > 0 {
					msg += " (" + c.Name() + ")"
				}
				msg += "\n    properties    " + c.Properties().String()
				fmt.Println(msg)

				// Read the characteristic, if possible.
				if (c.Properties() & gatt.CharRead) != 0 {
					b, err := p.ReadCharacteristic(c)
					if err != nil {
						fmt.Printf("Failed to read characteristic, err: %s\n", err)
						continue
					}
					fmt.Printf("    value         %x | %q\n", b, b)
				}

				fmt.Println("Checking ", c.UUID().String())
				if c.UUID().Equal(cloudServiceSecureCharId) {
					fmt.Println("Checking for the secure characteristic ...")

					// Discovery descriptors
					_, err := p.DiscoverDescriptors(nil, c)
					if err != nil {
						fmt.Printf("Failed to discover descriptors, err: %s\n", err)
						continue
					}

					/*val, err := p.ReadCharacteristic(c)
					if err != nil {
						fmt.Printf("Error reading secure characteristic, err: %s\n", err)
					} else {
						fmt.Printf("Read value is: %s\n", val)
					}
					*/

					if (c.Properties() & (gatt.CharNotify)) != 0 {
						err := p.SetNotifyValue(c, r.onSecureNotify)
						if err != nil {
							fmt.Printf("Failed to subscribe  to secue characteristic, err: %s\n", err)
						}
						r.SecureChar = c
					}

					break
				}
				if c.UUID().Equal(cloudServiceOpenCharId) {
					fmt.Println("Checking for the open characteristic ...")

					// Discovery descriptors
					_, err := p.DiscoverDescriptors(nil, c)
					if err != nil {
						fmt.Printf("Failed to discover descriptors, err: %s\n", err)
						continue
					}

					/*val, err := p.ReadCharacteristic(c)
					if err != nil {
						fmt.Printf("Error reading secure characteristic, err: %s\n", err)
					} else {
						fmt.Printf("Read value is: %s\n", val)
					}
					*/

					if (c.Properties() & (gatt.CharNotify)) != 0 {
						err := p.SetNotifyValue(c, r.onOpenNotify)
						if err != nil {
							fmt.Printf("Failed to subscribe  to secue characteristic, err: %s\n", err)
						}
						r.OpenChar = c
					}
					r.connected = true

					break
				}
				fmt.Println("next char")
			}

			fmt.Println("exit secure char")
		} //if cloud secure
		fmt.Println("Next service")
	}

	//fmt.Printf("Waiting for 5 seconds to get some notifiations, if any.\n")
	for {
		time.Sleep(5 * time.Second)
	}
}

func (r *BtGattRadio) WriteCharacteristic(c *gatt.Characteristic, b []byte, size int) (err error) {
	fmt.Printf("Going to write some stuff to radio")
	err = r.periph.WriteCharacteristic(c, b, true)
	if err != nil {
		fmt.Printf("Failed to write to secure characteristic, err: %s\n", err)
	} else {
		fmt.Printf("Successfully wrote to the secure characteristic \n")
	}
	return err
}

func (r *BtGattRadio) SendSecure(b []byte, size int) error {
	if r.connected {
		return r.WriteCharacteristic(r.SecureChar, b, size)
	} else {
		return errors.New("No radio connection")
	}
}

func (r *BtGattRadio) SendOpen(b []byte, size int) error {
	if r.connected {
		return r.WriteCharacteristic(r.OpenChar, b, size)
	} else {
		return errors.New("No radio connection")
	}
}

func (r *BtGattRadio) onPeriphDisconnected(p gatt.Peripheral, err error) {
	fmt.Println("Disconnected")
	close(done)
}

//InitConn Initializes the connection to the bt radio
func (r *BtGattRadio) InitConn() {
	log.Println("Initializing bluetooth radio")
	/*flag.Parse()
	if len(flag.Args()) != 1 {
		log.Fatalf("usage: %s [options] peripheral-id\n", os.Args[0])
	}
	*/
	d, err := gatt.NewDevice(option.DefaultClientOptions...)
	if err != nil {
		log.Fatalf("Failed to open device, err: %s\n", err)
		return
	}

	// Register handlers.
	d.Handle(
		gatt.PeripheralDiscovered(r.onPeriphDiscovered),
		gatt.PeripheralConnected(r.onPeriphConnected),
		//gatt.PeripheralConnected(onPeriphEnumeration),
		//gatt.PeripheralDisconnected(r.onPeriphDisconnected),
	)

	d.Init(r.onStateChanged)
	//<-done
	//fmt.Println("Done")
}

func (r *BtGattRadio) CloseConn() {
	<-done
	log.Println("terminated bluetooth connection")
}

func NewBtGattRadio(clientID string, secChan chan []byte, openChan chan []byte) *BtGattRadio {
	r := new(BtGattRadio)
	r.clientID = clientID
	r.secChan = secChan
	r.openChan = openChan
	return r
}
