using System;
using Microsoft.SPOT;
using System.Collections;

namespace Samraksh.APPS.ActiveTag
{
    public class NeighbourList
    {
        // Assuming the current app can not support more than 20 neighbours at a time 
        Hashtable neighbourList = new Hashtable();

        UInt16[] lostNeighbours = new UInt16[20];

        UInt16[] activeNeighbours = new UInt16[20];

        bool neighbourLost = false;

        public UInt16 numberOfLostNeighbours = 0;

        public UInt16 numberOfActiveNeighbours = 0;

        public void Flush()
        {
            for(UInt16 i = 0; i < 20; i++)
            {
                lostNeighbours[i] = 0;
                activeNeighbours[i] = 0;
            }
        }

        // O(n2) to O(3n)
        public bool Insert(UInt16[] newList)
        {
            Flush();

            foreach (DictionaryEntry entry in neighbourList)
            {
                entry.Value = false;
            }

            Debug.Print("My Active Neighbours are : ");

            if (newList != null)
            {
                for (int i = 0; i < 20; i++)
                {
                    if (newList[i] != 0)
                    {
                        Debug.Print(newList[i].ToString() + " ");
                        neighbourList[newList[i]] = true;
                    }
                }
            }

            Debug.Print("\n");

            foreach (DictionaryEntry entry in neighbourList)
            {
                bool neighbourActive = (bool)entry.Value;

                if (!neighbourActive)
                {
                    lostNeighbours[numberOfLostNeighbours++] = (UInt16)entry.Key;
                    neighbourLost = true;
                }
                else
                {
                    activeNeighbours[numberOfActiveNeighbours++] = (UInt16)entry.Key;
                }
                
            }

            if (numberOfLostNeighbours > 0)
                return false;

            return true;
        }

        public UInt16[] GetLostNeighbours()
        {
            return lostNeighbours;
        }

        public UInt16[] GetActiveNeighbours()
        {
            return activeNeighbours;
        }

    }
}
