package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	"github.com/paypal/gatt"
	"github.com/paypal/gatt/examples/option"
)

var done = make(chan struct{})

var cloudServiceId = gatt.MustParseUUID("0000ff1000001000800000805f9b34fb") // 6e400001-b5a3-f393-e0a9-e50e24dcca9e")
var cloudServiceOpenCharId = gatt.MustParseUUID("0000ff1100001000800000805f9b34fc")
var cloudServiceSecureCharId = gatt.MustParseUUID("0000ff1100001000800000805f9b34fb")

func onStateChanged(d gatt.Device, s gatt.State) {
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

func onPeriphDiscovered(p gatt.Peripheral, a *gatt.Advertisement, rssi int) {
	id := strings.ToUpper(flag.Args()[0])
	if strings.ToUpper(p.ID()) != id {
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

func onPeriphEnumeration(p gatt.Peripheral, err error) {
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

func onSecureNotify(c *gatt.Characteristic, b []byte, e error) {
	log.Printf("Got a secure msg %s\n", string(b))
}

func onPeriphConnected(p gatt.Peripheral, err error) {
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
						err := p.SetNotifyValue(c, onSecureNotify)
						if err != nil {
							fmt.Printf("Failed to subscribe  to secue characteristic, err: %s\n", err)
						}
					}

					fmt.Printf("Going to write some stuff")
					m := []byte{'g', 'o', 'l', 'a', 'n', 'g'}
					err = p.WriteCharacteristic(c, m, true)
					if err != nil {
						fmt.Printf("Failed to write to secure characteristic, err: %s\n", err)
					} else {
						fmt.Printf("Successfully wrote to the secure characteristic \n")
					}

					break
				}
				fmt.Println("next char")
			}

			fmt.Println("exit secure char")
		} //if cloud secure
		fmt.Println("Next service")
	}

	fmt.Printf("Waiting for 5 seconds to get some notifiations, if any.\n")
	time.Sleep(5 * time.Second)
}

func onPeriphDisconnected(p gatt.Peripheral, err error) {
	fmt.Println("Disconnected")
	close(done)
}

func main() {
	flag.Parse()
	if len(flag.Args()) != 1 {
		log.Fatalf("usage: %s [options] peripheral-id\n", os.Args[0])
	}

	d, err := gatt.NewDevice(option.DefaultClientOptions...)
	if err != nil {
		log.Fatalf("Failed to open device, err: %s\n", err)
		return
	}

	// Register handlers.
	d.Handle(
		gatt.PeripheralDiscovered(onPeriphDiscovered),
		gatt.PeripheralConnected(onPeriphConnected),
		//gatt.PeripheralConnected(onPeriphEnumeration),
		gatt.PeripheralDisconnected(onPeriphDisconnected),
	)

	d.Init(onStateChanged)
	<-done
	fmt.Println("Done")
}
