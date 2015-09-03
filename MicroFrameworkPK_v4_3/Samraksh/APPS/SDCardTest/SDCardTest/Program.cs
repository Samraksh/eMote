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
            try
            {
                //Debug.Print("----Initializing SD----\n");
                //SD.Initialize();
                //Debug.Print("----After initializing SD----\n");
                Program p = new Program();

                /*if (!SD.Initialize())
                {
                    throw new System.InvalidOperationException("SD storage failed to initialize");
                }*/

                /*ulong totalSize = SD.TotalSize();
                ulong totalFreeSpace = SD.TotalFreeSpace();
                Debug.Print("total size of SD (bytes) is " + totalSize.ToString());
                Debug.Print("total free space of SD (bytes) is " + totalFreeSpace.ToString());
                Debug.Print("total size of SD (GB) is " + ((double)(totalSize/1e9)).ToString());
                Debug.Print("total free space of SD (GB) is " + ((double)(totalFreeSpace/1e9)).ToString());*/

                Debug.Print("----Getting root directory of SD----\n");
                VolumeInfo vInfo = new VolumeInfo();
                string rootDirectory = vInfo.RootDirectory;
                Debug.Print("----rootDirectory is " + rootDirectory + "----\n");
                //string rootDirectory = VolumeInfo.GetVolumes()[0].RootDirectory;
                /*string volumeLabel = vInfo.VolumeLabel;
                Debug.Print("----volumeLabel is " + volumeLabel + "----\n");*/

                //------------------------//
                Debug.Print("----Opening file in SD----\n");
                FileStream fileHandle = new FileStream(rootDirectory + @"\hello.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.ReadWrite, 50);
                Debug.Print("----Writing to SD----\n");
                byte[] writeData = Encoding.UTF8.GetBytes("This is a test string. ");
                fileHandle.Write(writeData, 0, writeData.Length);

                Debug.Print("----Reading from SD----\n");
                byte[] readData = new byte[writeData.Length];
                fileHandle.Read(readData, 0, readData.Length);
                char[] readDataStr = Encoding.UTF8.GetChars(readData);
                for (int i = 0; i < readData.Length; i++)
                {
                    Debug.Print(readDataStr[i].ToString());
                }


                writeData = Encoding.UTF8.GetBytes("Writing somemore.");
                fileHandle.Write(writeData, 0, writeData.Length);
                readData = new byte[writeData.Length];
                fileHandle.Read(readData, 0, readData.Length);
                readDataStr = Encoding.UTF8.GetChars(readData);
                for (int i = 0; i < readData.Length; i++)
                {
                    Debug.Print(readDataStr[i].ToString());
                }

                Debug.Print("----Closing file handle----\n");
                fileHandle.Close();
                //------------------------//

                //------------------------//
                int j = 0;
                while (j < 1)
                {
                    System.Threading.Thread.Sleep(1);
                    j++;
                }
                Debug.Print("----Opening file in SD----\n");
                FileStream fileHandle1 = new FileStream(rootDirectory + @"\TEST\helloWorld.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.ReadWrite, 50);
                Debug.Print("----Writing to SD----\n");
                byte[] writeData1 = Encoding.UTF8.GetBytes("Hello world. This is a test. 123. Is anyone there?");
                fileHandle1.Write(writeData1, 0, writeData1.Length);

                Debug.Print("----Reading from SD----\n");
                byte[] readData1 = new byte[writeData1.Length];
                fileHandle1.Read(readData1, 0, readData1.Length);
                char[] readDataStr1 = Encoding.UTF8.GetChars(readData1);
                for (int i = 0; i < readData1.Length; i++)
                {
                    Debug.Print(readDataStr1[i].ToString());
                }
                Debug.Print("----Closing file handle----\n");
                fileHandle1.Close();
                //------------------------//
                /*while (true)
                {
                    System.Threading.Thread.Sleep(1);
                }*/
                //p.writeToSD();
            }
            catch (Exception ex)
            {
                Debug.Print(ex.StackTrace);
            }
        }

    }
}
