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


        public void AES_Example()
        {
            AesCryptoServiceProvider aes = new AesCryptoServiceProvider();

            // The data we want to encrypt
            string original_string = "Samraksh eMote Cryptoki Demo!";
            //convert to byte array
            byte[] original_data = UTF8Encoding.UTF8.GetBytes(original_string);
            //Encrypt the data
           /* byte[] encrypted_bytes = aes. Encrypt(original_data);
            //Decrypt the data
            byte[] decrypted_bytes = aes.Decrypt(encrypted_bytes);
            //print the decrypted data
            string decrypted_string = new string(Encoding.UTF8.GetChars(decrypted_bytes));
            Debug.Print("Data Size= " + original_string.Length + ", Data= " + original_string);
            Debug.Print("Encrypted Data: " + encrypted_bytes.ToString());
            Debug.Print("Encrypted Data size= " + encrypted_bytes.Length + ", Decrypted Data= " + decrypted_string);
            */
        }

        public static void Main()
        {
            Program p= new Program();
            p.RSA_Example();
            Debug.Print(Resources.GetString(Resources.StringResources.String1));
        }
    }
}
