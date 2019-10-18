package main

import (
	"bytes"
	"testing"
)

func TestAES256(t *testing.T) {

	sc := NewSecureComs()

	plainText := []byte("A quick red fox jumps over box.")
	t.Logf("PlainText: %s\n", plainText)
	cipherText, _ := sc.Encrypt(plainText, "doesnt matter")
	t.Logf("CipherText: %x\n", cipherText)
	rplainText, _ := sc.Decrypt(cipherText, "soso")
	//text, _ := hex.DecodeString(string(rplainText))
	t.Logf("PlainText: %s \n", rplainText)

	if bytes.Compare(plainText, rplainText) == 0 {
		t.Error("Encyption/Decryption failed\n")
	}
}
