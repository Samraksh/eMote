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
        
        byte[] Key128 = new byte[]
                {
                    0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
                    0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
                    0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
                    0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08,
                    0x46, 0x9A, 0xA9, 0xBA, 0xAE, 0x77, 0x20, 0x28,
                    0xED, 0x51, 0x43, 0x8C, 0x28, 0x6F, 0x99, 0x5B,
                    0x6B, 0x0C, 0x08, 0x7C, 0x4C, 0x7D, 0x6F, 0xCF,
                    0xD0, 0xF0, 0xAC, 0x2A, 0x9B, 0x28, 0x28, 0x62,
                    0x52, 0x3F, 0x56, 0x3B, 0x6F, 0x49, 0x10, 0x11,
                    0x48, 0x45, 0x36, 0x51, 0x62, 0xAE, 0x8C, 0x66,
                    0xE8, 0x53, 0x8D, 0x18, 0xDF, 0x21, 0x12, 0x30,
                    0x35, 0x79, 0xAD, 0x41, 0x0F, 0xED, 0x50, 0x41,
                    0x26, 0xC3, 0x3E, 0xFE, 0x88, 0xEB, 0xA8, 0x7C,
                    0xF2, 0x48, 0x13, 0x84, 0x27, 0xCE, 0x19, 0x86,
                    0x33, 0x14, 0x89, 0xEB, 0x7A, 0x90, 0x21, 0x46,
                    0x5C, 0xC2, 0x22, 0x23, 0x96, 0x06, 0x85, 0xF7,
                };

        byte[] Key256 = new byte[]
                {
                    0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
                    0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
                    0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
                    0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08,
                    0x46, 0x9A, 0xA9, 0xBA, 0xAE, 0x77, 0x20, 0x28,
                    0xED, 0x51, 0x43, 0x8C, 0x28, 0x6F, 0x99, 0x5B,
                    0x6B, 0x0C, 0x08, 0x7C, 0x4C, 0x7D, 0x6F, 0xCF,
                    0xD0, 0xF0, 0xAC, 0x2A, 0x9B, 0x28, 0x28, 0x62,
                    0x52, 0x3F, 0x56, 0x3B, 0x6F, 0x49, 0x10, 0x11,
                    0x48, 0x45, 0x36, 0x51, 0x62, 0xAE, 0x8C, 0x66,
                    0xE8, 0x53, 0x8D, 0x18, 0xDF, 0x21, 0x12, 0x30,
                    0x35, 0x79, 0xAD, 0x41, 0x0F, 0xED, 0x50, 0x41,
                    0x26, 0xC3, 0x3E, 0xFE, 0x88, 0xEB, 0xA8, 0x7C,
                    0xF2, 0x48, 0x13, 0x84, 0x27, 0xCE, 0x19, 0x86,
                    0x33, 0x14, 0x89, 0xEB, 0x7A, 0x90, 0x21, 0x46,
                    0x5C, 0xC2, 0x22, 0x23, 0x96, 0x06, 0x85, 0xF7,
                    0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
                    0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
                    0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
                    0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08,
                    0x46, 0x9A, 0xA9, 0xBA, 0xAE, 0x77, 0x20, 0x28,
                    0xED, 0x51, 0x43, 0x8C, 0x28, 0x6F, 0x99, 0x5B,
                    0x6B, 0x0C, 0x08, 0x7C, 0x4C, 0x7D, 0x6F, 0xCF,
                    0xD0, 0xF0, 0xAC, 0x2A, 0x9B, 0x28, 0x28, 0x62,
                    0x52, 0x3F, 0x56, 0x3B, 0x6F, 0x49, 0x10, 0x11,
                    0x48, 0x45, 0x36, 0x51, 0x62, 0xAE, 0x8C, 0x66,
                    0xE8, 0x53, 0x8D, 0x18, 0xDF, 0x21, 0x12, 0x30,
                    0x35, 0x79, 0xAD, 0x41, 0x0F, 0xED, 0x50, 0x41,
                    0x26, 0xC3, 0x3E, 0xFE, 0x88, 0xEB, 0xA8, 0x7C,
                    0xF2, 0x48, 0x13, 0x84, 0x27, 0xCE, 0x19, 0x86,
                    0x33, 0x14, 0x89, 0xEB, 0x7A, 0x90, 0x21, 0x46,
                    0x5C, 0xC2, 0x22, 0x23, 0x96, 0x06, 0x85, 0xF7,
                };

        byte[] IV16 = new byte[]
            {
                0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80,
                0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00,
            };

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

        public CryptoKey GetAESKey(byte[] key, AesCryptoServiceProvider csp)
        {
            return CryptoKey.ImportKey(csp.Session, key,
                CryptoKey.KeyClass.Secret, CryptoKey.KeyType.AES, true);
        }

        public void GenerateAESKey(byte[] key, AesCryptoServiceProvider csp)
        {
            csp.Key = CryptoKey.ImportKey(csp.Session, key,
                CryptoKey.KeyClass.Secret, CryptoKey.KeyType.AES, true);
        }
        /*public void SetAttribute {
            CryptokiAttribute[] aesKeyAttrb = new CryptokiAttribute[]
            {
                new CryptokiAttribute(CryptokiAttribute.CryptokiType.Class , Utility.ConvertToBytes((int)CryptokiClass.SECRET_KEY)),
                new CryptokiAttribute(CryptokiAttribute.CryptokiType.KeyType, Utility.ConvertToBytes((int)CryptoKey.KeyType.AES)),
                new CryptokiAttribute(CryptokiAttribute.CryptokiType.Modulus, new byte[]
                    {
                        0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
                        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
                        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
                        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08,
                        0x46, 0x9A, 0xA9, 0xBA, 0xAE, 0x77, 0x20, 0x28,
                        0xED, 0x51, 0x43, 0x8C, 0x28, 0x6F, 0x99, 0x5B,
                        0x6B, 0x0C, 0x08, 0x7C, 0x4C, 0x7D, 0x6F, 0xCF,
                        0xD0, 0xF0, 0xAC, 0x2A, 0x9B, 0x28, 0x28, 0x62,
                        0x52, 0x3F, 0x56, 0x3B, 0x6F, 0x49, 0x10, 0x11,
                        0x48, 0x45, 0x36, 0x51, 0x62, 0xAE, 0x8C, 0x66,
                        0xE8, 0x53, 0x8D, 0x18, 0xDF, 0x21, 0x12, 0x30,
                        0x35, 0x79, 0xAD, 0x41, 0x0F, 0xED, 0x50, 0x41,
                        0x26, 0xC3, 0x3E, 0xFE, 0x88, 0xEB, 0xA8, 0x7C,
                        0xF2, 0x48, 0x13, 0x84, 0x27, 0xCE, 0x19, 0x86,
                        0x33, 0x14, 0x89, 0xEB, 0x7A, 0x90, 0x21, 0x46,
                        0x5C, 0xC2, 0x22, 0x23, 0x96, 0x06, 0x85, 0xF7,
                    }
                ),
                //new CryptokiAttribute(CryptokiAttribute.CryptokiType., new byte[]
                 //   {
                  //      0x01, 0x00, 0x01
                  //  }
                //),
            };
        }*/
       

       public static string ByteArrayToString (byte[] bArray)
        {
            //char[] cArray = bArray;
            string s ="";
            for (int i = 0; i < bArray.Length; i++)
            {
                s += bArray[i].ToString();
                s += " ";
            }
            return s;
        }

       public void AES_Example()
       {

           //Specify the key size. Native side will figure out the key to use.
           //AesCryptoServiceProvider aes = new AesCryptoServiceProvider(_keysize);
           AesCryptoServiceProvider aes = new AesCryptoServiceProvider();

           Session aesSession = aes.Session;

           
           /*AesCryptoServiceProvider aes = new AesCryptoServiceProvider
           {
               Padding = _padding, Key = key, Mode = _cipherMode, KeySize = _keysize
           };*/

           
               //GenerateAESKey(Key256, aes);
               CryptoKey ckey = CryptoKey.ImportKey(aesSession, Key128, 
                   CryptoKey.KeyClass.Secret, CryptoKey.KeyType.AES, true);
           

           //Create encryptor and decryptor
           //var encryptor = aes.CreateEncryptor();
           aes.Key = ckey;
           aes.IV = IV16;
           //var encryptor = aes.CreateEncryptor(aes.Key, aes.IV);
           var encryptor = aes.CreateEncryptor();

           //Store the key securely. will need for decrypting latter
           //byte[] aesKey = aes.Key.ExportKey(true);
           //Debug.Print("Key = " + ByteArrayToString(aesKey));

           //string aesKey = aes.Key.ToString(); 
           //Debug.Print("Key = " + aesKey);

            // The data we want to encrypt
            string original_string = "Samraksh eMote Cryptoki Demo!";
            //convert to byte array
            byte[] original_data = UTF8Encoding.UTF8.GetBytes(original_string);
            //Encrypt the data
            byte[] en_bytes = encryptor.TransformFinalBlock(original_data, 0, original_data.Length);
            //string en_s = new string(Encoding.UTF8.GetChars(en_bytes));
            //Debug.Print("Data Size= " + original_string.Length + ", Data= " + original_string);
            //Debug.Print("Encrypted Data: " + ByteArrayToString(en_bytes));
            //Debug.Print("Encrypted Data size= " + en_bytes.Length);       
     
            //Decrypt the data
            var decryptor = aes.CreateDecryptor();
            byte[] de_bytes = decryptor.TransformFinalBlock(en_bytes,0, en_bytes.Length);
            //print the decrypted data

            string decrypted_string = new string(Encoding.UTF8.GetChars(de_bytes));
            //Debug.Print("Decrypted Data= " + ByteArrayToString(de_bytes));
            Debug.Print("Decrypted String= " + decrypted_string);
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
