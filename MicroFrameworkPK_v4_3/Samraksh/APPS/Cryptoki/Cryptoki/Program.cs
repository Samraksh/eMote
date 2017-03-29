using System;
using Microsoft.SPOT;
using System.Security;
using System.Security.Cryptography;
using Microsoft.SPOT.Cryptoki;
using System.Threading;
using System.Text;

namespace Cryptoki
{
    public class Program
    {
        const int _keysize = 256;                       // in bits / must be a legal AES key size
       // private const int _iterations = 0x186A0 / 10;   // how many times to run each encryption/decryption run ... 1K by default
        const CipherMode _cipherMode = CipherMode.CBC;
        const PaddingMode _padding = PaddingMode.PKCS7;

        public void RSA_Example()
        {
            try
            {
                RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            
                // The data we want to encrypt
                string original_string = "Samraksh eMote Cryptoki Demo!";
                //convert to byte array
                byte[] original_data = UTF8Encoding.UTF8.GetBytes(original_string);
                //Encrypt the data
                byte[] encrypted_bytes = rsa.Encrypt(original_data);
                //Decrypt the data
                byte[] decrypted_bytes = rsa.Decrypt(encrypted_bytes);
                //print the decrypted data
                string decrypted_string = new string(Encoding.UTF8.GetChars(decrypted_bytes));
                Debug.Print("Data Size= " + original_string.Length + ", Data= " + original_string);
                Debug.Print("Encrypted Data: " + encrypted_bytes.ToString());
                Debug.Print("Encrypted Data size= " + encrypted_bytes.Length + ", Decrypted Data= " + decrypted_string);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
        /*
        public CryptoKey GenerateKey()
        {
           //Lets say you dont have a Key. This is a bit complicated we will not do this now.
           CryptoKey aesKey = SymmetricAlgorithm.Key()
           var r = new RNGCryptoServiceProvider();
           var bkey = new byte[_keysize / 8];
           r.GetBytes(bkey);
       }
       */
       public void AES_Example()
       {

           //Specify the key size. Native side will figure out the key to use.
           AesCryptoServiceProvider aes = new AesCryptoServiceProvider(_keysize);

           /*AesCryptoServiceProvider aes = new AesCryptoServiceProvider
           {
               Padding = _padding, Key = key, Mode = _cipherMode, KeySize = _keysize
           };*/

            //Create encryptor and decryptor
            var encryptor = aes.CreateEncryptor();
            var decryptor = aes.CreateDecryptor();


            // The data we want to encrypt
            string original_string = "Samraksh eMote Cryptoki Demo!";
            //convert to byte array
            byte[] original_data = UTF8Encoding.UTF8.GetBytes(original_string);
            //Encrypt the data
            byte[] encrypted_bytes = encryptor.TransformFinalBlock(original_data, 0, original_data.Length);
            //Decrypt the data
            byte[] decrypted_bytes = decryptor.TransformFinalBlock(encrypted_bytes,0, encrypted_bytes.Length);
            //print the decrypted data
            string decrypted_string = new string(Encoding.UTF8.GetChars(decrypted_bytes));
            Debug.Print("Data Size= " + original_string.Length + ", Data= " + original_string);
            Debug.Print("Encrypted Data: " + encrypted_bytes.ToString());
            Debug.Print("Encrypted Data size= " + encrypted_bytes.Length + ", Decrypted Data= " + decrypted_string);
        }

        public static void Main()
        {
            Program p= new Program();
            //p.RSA_Example();
            p.AES_Example();
            Debug.Print(Resources.GetString(Resources.StringResources.String1));
        }
    }
}
