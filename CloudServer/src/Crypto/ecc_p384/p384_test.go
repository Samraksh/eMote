package Crypto

//Based on ecdh implementation at https://github.com/aead/ecdh
//need to checkout this repo as ecdh using go get github.com/aead/ecdh

import (
	"bytes"
	"crypto/elliptic"
	"crypto/rand"
	"encoding/hex"
	"fmt"
	"github.com/aead/ecdh"
	"testing"
)

func TestP384(t *testing.T) {
	p384 := ecdh.Generic(elliptic.P384())

	privateAlice, publicAlice, err := p384.GenerateKey(rand.Reader)
	if err != nil {
		fmt.Printf("Failed to generate Alice's private/public key pair: %s\n", err)
	}
	privateBob, publicBob, err := p384.GenerateKey(rand.Reader)
	if err != nil {
		fmt.Printf("Failed to generate Bob's private/public key pair: %s\n", err)
	}

	if err := p384.Check(publicBob); err != nil {
		fmt.Printf("Bob's public key is not on the curve: %s\n", err)
	}
	secretAlice := p384.ComputeSecret(privateAlice, publicBob)
	fmt.Printf("Alice's Secret key\n %s\n", hex.Dump(secretAlice))

	if err := p384.Check(publicAlice); err != nil {
		fmt.Printf("Alice's public key is not on the curve: %s\n", err)
	}
	secretBob := p384.ComputeSecret(privateBob, publicAlice)
	fmt.Printf("Bob's Secret key\n %s\n", hex.Dump(secretBob))

	if !bytes.Equal(secretAlice, secretBob) {
		fmt.Printf("key exchange failed - secret X coordinates not equal\n")
	}
	// Output:
}

/*func TestP384_1(t *testing.T) {
	fmt.Println("PAss : 1")
}*/
