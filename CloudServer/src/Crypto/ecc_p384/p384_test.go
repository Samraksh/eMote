package Crypto

//Based on ecdh implementation at https://github.com/aead/ecdh
//need to checkout this repo as ecdh using go get github.com/aead/ecdh

import (
	//"bytes"
	"crypto/elliptic"
	"crypto/hmac"
	"crypto/rand"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"github.com/aead/ecdh"
	"testing"
)

func CreateSecret() []byte {
	p384 := ecdh.Generic(elliptic.P384())

	privateAlice, publicAlice, err := p384.GenerateKey(rand.Reader)
	if err != nil {
		fmt.Printf("Failed to generate Alice's private/public key pair: %s\n", err)
		return nil
	}
	privateBob, publicBob, err := p384.GenerateKey(rand.Reader)
	if err != nil {
		fmt.Printf("Failed to generate Bob's private/public key pair: %s\n", err)
		return nil
	}

	if err := p384.Check(publicBob); err != nil {
		fmt.Printf("Bob's public key is not on the curve: %s\n", err)
		return nil
	}
	secretAlice := p384.ComputeSecret(privateAlice, publicBob)
	fmt.Printf("Alice's Secret key\n %s\n", hex.Dump(secretAlice))

	if err := p384.Check(publicAlice); err != nil {
		fmt.Printf("Alice's public key is not on the curve: %s\n", err)
		return nil
	}
	secretBob := p384.ComputeSecret(privateBob, publicAlice)
	fmt.Printf("Bob's Secret key\n %s\n", hex.Dump(secretBob))
	return secretAlice
}

func TestP384(t *testing.T) {

	secret := CreateSecret()
	if secret == nil {
		fmt.Printf("key exchange failed - secret X coordinates not equal\n")
		t.Failed()
	}
	// Output:
}

/*func TestP384_1(t *testing.T) {
	fmt.Println("PAss : 1")
}*/

func TestHMAC(t *testing.T) {
	key := CreateSecret()
	message := "Hey dude"
	//message, messageMAC, key []byte) bool {
	mac := hmac.New(sha256.New, key)
	mac.Write([]byte(message))
	expectedMAC := mac.Sum(nil)
	fmt.Println("Message: ", message)
	fmt.Println("HMAC: ", hex.Dump(expectedMAC))
}
