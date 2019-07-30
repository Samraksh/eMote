/*
Author: Mukundan Sridhran
The Samraksh Company
All Rights Reserved
*/

//A complete generic marshaller/serializer seems to be a lot of work, becuase reflection alone will not work
//relection cannot retrieve the original var/struct in go and hence cant operate on the struct, without knowing the type of the struct

package Reflection

import (
	"encoding/binary"
	//"encoding/hex"
	"fmt"
	"reflect"
)

func MarshalLE(s interface{}) ([]byte, bool) {
	var err bool = false
	var out []byte

	//var strings string
	st := reflect.TypeOf(s)
	sv := reflect.ValueOf(s)

	//depth := 0
	switch st.Kind() {
	case reflect.Struct:

		for i := 0; i < st.NumField(); i++ {
			f := st.Field(i)
			fmt.Println("Field", i+1, "name is", f.Name, "type is", f.Type.Name(), "and kind is", f.Type.Kind(), "and the value  is: ", sv.Field(i))
			fmt.Printf("type of value is %T\n", sv.Field(i))
			o, e := MarshalUnitLE(f.Type, sv.Field(i))
			if e {
				out = append(out, o...)
			} else {
				out = nil
				err = false
				return out, err
			}

			/*if f.Tag != "" {
				fmt.Println(strings.Repeat("\t", depth+2), "Tag is", f.Tag)
				fmt.Println(strings.Repeat("\t", depth+2), "tag1 is", f.Tag.Get("tag1"), "tag2 is", f.Tag.Get("tag2"))
			}*/
		}
	case reflect.Slice:
	default:
		out = nil
		err = false
		return out, err
	}
	return out, err
}

func MarshalUnitLE(t reflect.Type, b interface{}) ([]byte, bool) {

	var err bool = false
	var out []byte
	switch t.Kind() {
	case reflect.Uint64:

		out = make([]byte, 8)
		//data, e := b.(uint64)
		//err = e
		binary.LittleEndian.PutUint64(out, b)
		/*if e {
			binary.LittleEndian.PutUint64(out, data)
			fmt.Println("Got a uint64 of value ", data, " Converted to: ", out)
		} else {
			fmt.Println("Error: Not a uint64")
		}*/

	case reflect.Uint32:
		out := make([]byte, 4)
		data, e := b.(uint32)
		err = e
		binary.LittleEndian.PutUint32(out, data)
	case reflect.Uint8:
		data, e := b.(uint8)
		out = []byte{data}
		err = e
	case reflect.Bool:
		data, e := b.(bool)
		err = e
		out := make([]byte, 1)
		if data {
			out[0] = 1
		}
	default:
		out = nil
	}

	return out, err
}
