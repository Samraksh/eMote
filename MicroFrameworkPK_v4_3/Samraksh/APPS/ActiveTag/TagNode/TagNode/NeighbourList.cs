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

        bool neighbourLost = false;

        UInt16 numberOfLostNeighbours = 0;

        public bool Insert(UInt16[] newList)
        {
            foreach (DictionaryEntry entry in neighbourList)
            {
                entry.Value = false;
            }

            for (int i = 0; i < newList.Length; i++)
            {
                neighbourList[newList[i]] = true;
            }

            foreach (DictionaryEntry entry in neighbourList)
            {
                bool neighbourActive = (bool)entry.Value;
                if (!neighbourActive)
                {
                    lostNeighbours[numberOfLostNeighbours++] = (UInt16) entry.Key;
                    neighbourLost = true;
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

    }
}
