using System;
using System.IO;            //For FileStream class
using System.Text;          //For Encoding class
using Microsoft.SPOT;
//using Microsoft.SPOT.IO;    //For VolumeInfo class
using Samraksh.eMote.DotNow;


namespace SDCardTest
{
    public class Program
    {
        public bool writeToSD()
        {
            //Block size of SD card is 512 bytes. 
            //The address to which data is written is calculated like this: writeAddress += bufferSize * 2;
            //so if bufferSize is 256, then write happens every 512 bytes. <data><data><data>...
            //If bufferSize is 512, then 1st block is filled up, 2nd block is left empty and 3rd block is written with next data. <data><empty><data><empty><data>...
            byte[] sdBytes1 = new byte[256];
            for (int i = 0; i < sdBytes1.Length; i++)
            {
                sdBytes1[i] = 0x0A;
            }
            /*byte[] sdBytes2 = new byte[256];
            for (int i = 0; i < sdBytes2.Length; i++)
            {
                sdBytes2[i] = 0xFF;
            }
            byte[] sdBytes3 = new byte[512];
            Array.Copy(sdBytes1, sdBytes3, sdBytes1.Length);
            Array.Copy(sdBytes2, 0, sdBytes3, sdBytes1.Length, sdBytes2.Length);*/
            
            for (int i = 0; i < 256; i++)
            {
                Debug.Print("sdBytes1.Length " + sdBytes1.Length);
                if (!SD.Write(sdBytes1, 0, (ushort)sdBytes1.Length))
                {
                    Debug.Print("Could not write to SD\n");
                    return false;
                }
            }
            return true;
        }

        public static void Main()
        {
            //Debug.Print("----Initializing SD----\n");
            //SD.Initialize();
            //Debug.Print("----After initializing SD----\n");
            Program p = new Program();
            
            /*if (!SD.Initialize())
            {
                throw new System.InvalidOperationException("SD storage failed to initialize");
            }*/
            Debug.Print("----Getting root directory of SD----\n");
            VolumeInfo vInfo = new VolumeInfo();
            string rootDirectory = vInfo.RootDirectory;
            Debug.Print("----rootDirectory is " + rootDirectory + "----\n");
            //string rootDirectory = VolumeInfo.GetVolumes()[0].RootDirectory;
            /*string volumeLabel = vInfo.VolumeLabel;
            Debug.Print("----volumeLabel is " + volumeLabel + "----\n");*/

            Debug.Print("----Opening file in SD----\n");
            
            FileStream fileHandle = new FileStream(rootDirectory + @"\hello.txt", FileMode.OpenOrCreate, FileAccess.Write, FileShare.Write, 50);
            byte[] data = Encoding.UTF8.GetBytes("This is a test string");
            Debug.Print("----Writing to SD----\n");
            fileHandle.Write(data, 0, data.Length);
            fileHandle.Close();

            /*FileStream fileHandle1 = new FileStream(rootDirectory + @"\TEST\helloWorld.txt", FileMode.OpenOrCreate, FileAccess.Write, FileShare.Write, 50);
            byte[] data1 = Encoding.UTF8.GetBytes("Hello world. This is a test. 123. Is anyone there?");
            Debug.Print("----Writing to SD----\n");
            fileHandle1.Write(data1, 0, data1.Length);
            fileHandle1.Close();*/

            Debug.Print("----Closing file handle----\n");
            /*while (true)
            {
                System.Threading.Thread.Sleep(1);
            }*/
            //p.writeToSD();
        }

    }
}
