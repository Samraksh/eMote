using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Messaging;
using System.Security.Cryptography;
using Microsoft.SPOT.Cryptoki;


namespace ecdsa
{
    class Utils
    {
        public static void resizeArray(ref byte[] arr, int newSize)
        {
            if (newSize < 0) throw new ArgumentOutOfRangeException("Array size negative");
            byte[] temp = new byte[newSize];
            Array.Copy(arr, temp, newSize);
            arr = temp;
        }
    }

    public class Program
    {
        // Cryptographically secure random number generator.
        public static CryptokiRNG rng = null;
        static UInt16 myID = 76;

        static string ByteToHex(byte[] bytes)
        {
            char[] c = new char[bytes.Length * 2];
            int b;
            for (int i = 0; i < bytes.Length; i++)
            {
                b = bytes[i] >> 4;
                c[i * 2] = (char)(55 + b + (((b - 10) >> 31) & -7));
                b = bytes[i] & 0xF;
                c[i * 2 + 1] = (char)(55 + b + (((b - 10) >> 31) & -7));
            }
            return new string(c);
        }

        public static void Main()
        {
            // TODO: What mechanisms do we need? What's provider name and what should we use for it?
            Session session = new Session("", MechanismType.ECDSA_KEY_PAIR_GEN, MechanismType.ECDSA, MechanismType.SHA256);
            ECDsaCryptoServiceProvider aliceProvider = new ECDsaCryptoServiceProvider(session, 256);

            //ECDiffieHellmanCryptoServiceProvider alice = new ECDiffieHellmanCryptoServiceProvider(256);

            //ECDiffieHellmanCryptoServiceProvider bob = new ECDiffieHellmanCryptoServiceProvider(256);

            //Send the public keys to the other side over the network
            //var aKey = alice.DeriveKeyMaterial(bob.PublicKey);
            //var bKey = bob.DeriveKeyMaterial(alice.PublicKey);

            //ECDsaCryptoServiceProvider aliceProvider = new ECDsaCryptoServiceProvider(alice.KeyPair);

            // Initialize the cryptographically secure random number generator and seed it
            rng = new CryptokiRNG(aliceProvider.Session);
            rng.SeedRandom(BitConverter.GetBytes(myID));

            // TEST: Generate a random integer
            byte[] test = new byte[4];
            rng.GenerateRandom(test, 0, 4, true);
            Debug.Print("" + test[0] + " " + test[1] + " " + test[2] + " " + test[3]);

            // TEST: Sign the generated random integer
            byte[] signature = aliceProvider.SignData(test);
            Debug.Print("Signature: " + ByteToHex(signature));

            var pubKey = aliceProvider.KeyPair.ExportKey(true);
            //var pubKey = alice.PublicKey ;
            Debug.Print("Pub Key: " + ByteToHex(pubKey));


            // TODO: The following doesn't work
            // TEST: Verify the signature
            Session bobSession = new Session("", MechanismType.ECDSA, MechanismType.SHA256);
            CryptoKey importedKey = CryptoKey.ImportKey(bobSession, pubKey, CryptoKey.KeyClass.Other, CryptoKey.KeyType.DES , true);

            ECDsaCryptoServiceProvider bobProvider = new ECDsaCryptoServiceProvider(importedKey);
            //ECDsaCryptoServiceProvider bobProvider = new ECDsaCryptoServiceProvider(bob.KeyPair);

            bool verified = bobProvider.VerifyData(test, signature);
            if(verified)
                Debug.Print("Signature verified by exported then re-imported key.");
            else
                Debug.Print("Signature verification failed.");
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
