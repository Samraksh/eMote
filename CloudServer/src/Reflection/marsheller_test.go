package Marshaller

import (
	//"encoding/binary"
	"encoding/hex"
	"fmt"
	//"reflect"
	"testing"
)

type TestStruct struct {
	A uint64
	B bool
	//C []byte
}

func TestMarshelling(t *testing.T) {

	x := TestStruct{
		A: 239403482348,
		B: true,
		//C: make([]byte, 10),
	}

	data, err := MarshalLE(x)

	if !err {
		t.Error("Failed Marshelling")
	}

	fmt.Printf("Marshalled data of size %d: \n %s ", len(data), hex.Dump(data))
}
