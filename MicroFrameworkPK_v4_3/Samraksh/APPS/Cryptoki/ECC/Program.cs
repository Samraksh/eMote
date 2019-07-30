using System;
using Microsoft.SPOT;
using System.Security;
using System.Security.Cryptography;
using Microsoft.SPOT.Cryptoki;
using System.Threading;
using System.Text;


namespace ECC
{
    public class Program
    {
        public void PrintArray(byte[] x, int size)
        {
            for (int i = 0; i < size; i++)
            {
                Debug.Print(x[i] + "");
            }
            Debug.Print ("\n");

        }

        public void DeriveECCKey(ref CryptoKey k1, ref CryptoKey k2, int keysize)
        {
            ECDiffieHellmanCryptoServiceProvider alice = new ECDiffieHellmanCryptoServiceProvider(256);
            ECDiffieHellmanCryptoServiceProvider bob = new ECDiffieHellmanCryptoServiceProvider(256);
            
            //CryptoKey m1 = alice.DeriveKeyMaterial(bob.PublicKey);
            //CryptoKey m2 = bob.DeriveKeyMaterial(alice.PublicKey);
            k1 = alice.DeriveKeyMaterial(bob.PublicKey);
            k2 = bob.DeriveKeyMaterial(alice.PublicKey);
            
            //alice.KeyDerivationFunction = ECDiffieHellmanKeyDerivationFunction(MechanismType.ECDH1_COFACTOR_DERIVE);
        }

        public void ECC_AES(){

            DateTime start = DateTime.Now;
            Debug.Print("Boot up time:"+start.ToString()+":"+start.Millisecond);
            Debug.Print("Lets begin ECDH derived keys..");
            
            AesCryptoServiceProvider alice = new AesCryptoServiceProvider(256);
            AesCryptoServiceProvider bob = new AesCryptoServiceProvider(256);

            var k1 = alice.Key;
            var k2= bob.Key;
            byte[] k1b = k1.ExportKey(false);
            byte[] k2b = k2.ExportKey(false);

            Debug.Print("Null Key K1: "); PrintArray(k1b,k1b.Length);
            Debug.Print("Null Key K2: "); PrintArray(k2b, k2b.Length);

            DeriveECCKey(ref k1, ref k2, 256);
            k1b = k1.ExportKey(false);
            k2b = k2.ExportKey(false);

            Debug.Print("ECC Key K1: "); PrintArray(k1b, k1b.Length);
            Debug.Print("ECC Key K2: "); PrintArray(k2b, k2b.Length);

            /*if( k1b == k2b){
                Debug.Print("Key generation successful.");
            }else {
                Debug.Print("Key generation failed.");
            }*/
        }

        public static void Main()
        {
            Program p = new Program();
            p.ECC_AES();
        }
    }
}
