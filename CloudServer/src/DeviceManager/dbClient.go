package DeviceManager

import (
	"encoding/json"
	"fmt"
	"log"

	"github.com/boltdb/bolt"
)

/// Tables are called buckets in boltdb
type Device struct {
	Id     string       `json:"id"`
	Name   string       `json:"name"`
	State  DeviceStatus `json:"state"`
	AESKey string       `json:"aeskey"`
}

///based on example from here https://callistaenterprise.se/blogg/teknik/2017/02/27/go-blog-series-part3/

// Real implementation
type BoltDBClient struct {
	boltDB *bolt.DB
}

//Global db client
var GDBC BoltDBClient = BoltDBClient{}

func (bc *BoltDBClient) OpenBoltDb() {
	var err error
	bc.boltDB, err = bolt.Open("DeviceManager/db/devices.db", 0600, nil)
	if err != nil {
		log.Fatal(err)
	}
}

func (bc *BoltDBClient) CreateDB() {
	//we have a single table db.
	bc.boltDB.Update(func(tx *bolt.Tx) error {
		_, err := tx.CreateBucket([]byte("DeviceBucket"))
		if err != nil {
			return fmt.Errorf("create bucket failed: %s", err)
		}
		return nil
	})
}

// Creates instance and calls the OpenBoltDb and Seed funcs
func InitDB() {
	//open database
	GDBC.OpenBoltDb()
	GDBC.CreateDB()
}

func (bc *BoltDBClient) UpdateDevice(id string, name string, state DeviceStatus, aeskey string) {
	newd := Device{id, name, state, aeskey}
	jsonBytes, _ := json.Marshal(newd)

	// Write the data to the deviceBucket
	bc.boltDB.Update(func(tx *bolt.Tx) error {
		b := tx.Bucket([]byte("DeviceBucket"))
		err := b.Put([]byte(id), jsonBytes)
		return err
	})
}

func (bc *BoltDBClient) QueryDevice(deviceId string) (Device, error) {
	device := Device{}

	// Read an object from the bucket using boltDB.View
	err := bc.boltDB.View(func(tx *bolt.Tx) error {
		// Read the bucket from the DB
		b := tx.Bucket([]byte("DeviceBucket"))

		// Read the value identified by our deviceId supplied as []byte
		deviceBytes := b.Get([]byte(deviceId))
		if deviceBytes == nil {
			return fmt.Errorf("No device found for " + deviceId)
		}
		// Unmarshal the returned bytes into the device struct we created at
		// the top of the function
		json.Unmarshal(deviceBytes, &device)

		// Return nil to indicate nothing went wrong, e.g no error
		return nil
	})
	// If there were an error, return the error
	if err != nil {
		return Device{}, err
	}
	// Return the device struct and nil as error.
	return device, nil
}
