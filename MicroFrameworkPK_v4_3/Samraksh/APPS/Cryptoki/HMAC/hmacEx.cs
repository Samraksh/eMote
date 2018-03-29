using System;
using Microsoft.SPOT;
using System.Security;
using System.Security.Cryptography;
using Microsoft.SPOT.Cryptoki;
using System.Text;
using System.Threading;

namespace HMAC
{
    public class hmacEx
    {
        public static string ByteArrayToString(byte[] bArray)
        {
            //char[] cArray = bArray;
            string s = "";
            for (int i = 0; i < bArray.Length; i++)
            {
                s += bArray[i].ToString();
                s += " ";
            }
            return s;
        }


        byte[] Key1 = new byte[]
               {
                    0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
                    0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
                    0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
                    0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08
               };

        public void hmacExample()
        {         
            KeyedHashAlgorithm hmac = new KeyedHashAlgorithm(KeyedHashAlgorithmType.HMACSHA256);
            Session hmacSession = hmac.Session;
            CryptoKey hkey = CryptoKey.ImportKey(hmacSession, Key1,
              CryptoKey.KeyClass.Secret, CryptoKey.KeyType.GENERIC_SECRET, true);

            string original_string = "Samraksh eMote Cryptoki HMAC Example; Plus the wolf is great, but the fox is grey, the lamb is prey, but its a mountain pro!";
            //convert to byte array
            byte[] original_data = UTF8Encoding.UTF8.GetBytes(original_string);
            hmac.Key = hkey;

            DateTime start = DateTime.Now;
            byte[] hSig = hmac.TransformFinalBlock(original_data, 0, original_data.Length);
            TimeSpan encTime = (DateTime.Now - start);

            Debug.Print("Data Size= " + original_string.Length + ", Data= " + original_string);
            Debug.Print("HMAC Size: " + hSig.Length + ", Encrypte Data= " + ByteArrayToString(hSig));
            Debug.Print("Time for HMAC Sig = " + encTime.ToString());
        }

        public static void Main()
        {
            Debug.Print(Resources.GetString(Resources.StringResources.String1));
            hmacEx p = new hmacEx();
            //p.RSA_Example();
            p.hmacExample();
            Debug.Print("End of HMAC example");
            Thread.Sleep(Timeout.Infinite);

        }
    }
}
