/* Copyright: The Samaraksh Company (All rights reserved)
 * Author: Mukundan Sridharan
 * Date: Feb 2013
 * Function: Provides access to Samraksh NVM DataStore filesystem
 */

using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;


namespace Samraksh.SPOT.NonVolatileMemory
{
    //using DATAID = UInt16;

    /*public class DataID
    {
        public DataID(UInt16 value)
        {
            DATAID = value;
        }

        private static UInt16 DATAID;

        public static implicit operator DataID(UInt16 value)
        {
            return new DataID(value);
        }

        public static explicit operator UInt16(DataID Data)
        {
            return DATAID;
        }
    }*/

    /*struct DataID
    {
        private UInt16 value;

        public DataID(UInt16 value)
        {
            this.value = value;
        }

        public static implicit operator DataID(UInt16 value)
        {
            return new DataID(value);
        }

        public static explicit operator UInt16(DataID did)
        {
            //DataID did = new DataID(d);
            return did.value;
        }
    }*/

    public enum StorageType
    {
        NOR,
        SD
    };

    /* Error values */
    public enum DATASTORE_ERROR
    {
        DATASTORE_ERROR_NONE,
        DATASTORE_ERROR_INVALID_PARAM,
        DATASTORE_ERROR_OUT_OF_MEMORY,
        DATASTORE_ERROR_INVALID_DATA_ID,
        DATASTORE_ERROR_INVALID_GIVEN_ADDR,
        DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
        DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
        DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS,
        DATASTORE_ERROR_UNEXPECTED_ERROR
    };

    public enum DataStatus
    {
        Success,
        Failure,
        AlreadyExists,
        InvalidParameter,
        InvalidPointer
    };

    /*public class DataRefList
    {
        public List<Data> dataObj;
    }*/

    public class Data
    {
        // Record Id unique to a pointer
        static private UInt32 dataId;
        private UInt32 m_Size = 0;
        private byte[] buffer;
        private Int32 dataPointer;
        private Type dataType;
        
        DataStore dStore;
        ////private List<Data> dataObj = new List<Data>();
        ////DataRefList dRefList = new DataRefList();
        ////dRefList.dataObj = new List<Data>();

        public Data(DataStore dStore, Type dataType)
        {
            this.dStore = dStore;
            this.dataType = dataType;
        }

        public Data(DataStore dStore, UInt32 m_Size, Type dataType)
        {
            this.dStore = dStore;
            this.m_Size = m_Size;
            this.dataType = dataType;
            ++dataId;
            //this.dataPointer = 0x0;
            this.dataPointer = dStore.CreateData(dataId, m_Size);
            
            //dataObj.Add(this);
            //dRefList.dataObj.Add(this);
            //dStore.addDataReference(dataId);
        }

        /// <summary>
        /// Returns a data object corresponding to the input parameter dataId.
        /// </summary>
        /// <param name="StartdataId">Data ID of the first data record to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        public Data(DataStore dStore, UInt32 dataId)
        {
            this.dStore = dStore;
            this.dataPointer = LookupData(dataId);
            //dataId = dId;
            //buffer = new byte[MaxDataSize];
            //m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }

        /*public DataStatus Create()
        {
            dataPointer = dStore.CreateData(dataId, m_Size);
            if (dataPointer != 0x0)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }*/

        public DataStatus GetStatus()
        {
            if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS)
                return DataStatus.AlreadyExists;
            else if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                return DataStatus.Success;

            return DataStatus.Failure;
        }

        /// <summary>
        /// Create a data element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*public Data(UInt32 MaxDataSize)
        {
            dataId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /// <summary>
        /// Write an integer to the data store
        /// </summary>
        /// <param name="data">Integer to be stored</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        /*public int Write(int data, UInt32 dataId)
        {
            byte[] buffer = new byte[sizeof(int)];
            return dStore.Write(dataId, buffer, dataId);
        }*/

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public DataStatus Write(byte[] data, UInt32 offset, UInt32 numBytes)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            //return dStore.Write(dataId, data, offset, numBytes);
            bool retVal = dStore.Write((uint)dataPointer, data, numBytes);
            if (retVal == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        public DataStatus Write(byte[] data, UInt32 numBytes)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            //return dStore.Write(dataId, data, 0, numBytes);
            bool retVal = dStore.Write((uint) dataPointer, data, numBytes);
            if (retVal == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Read a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public DataStatus Read(byte[] data)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (dStore.Read((uint)dataPointer, data) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Get a dataId from the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        /*private Data LookupDataId(UInt32 dataId)
        {
            return LookupData(dataId);
        }*/

        /// <summary>
        /// Delete a data/record from the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public DataStatus Delete()
        {
            if(DeleteData(dataId) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /*public bool ReadAllDataReferences(Data[] dataArray, UInt32 CountOfData)
        {
            for (UInt32 index = CountOfData; index < dataArray.Length; ++index)
            {
                dataArray = dataObj.ToArray();
                dataObj[index];
            }
            return true;
        }*/

        /*public UInt32 CountOfDataReferences()
        {
            return (UInt32)dataObj.Count;
        }*/

        ~Data()
        {
            DisposeNativeMemoryPointer(dataId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Get a dataId from the data store
        /// </summary>
        /// <param name="dataID">ID of the data/record to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private Int32 LookupData(UInt32 dataId);

        /// <summary>
        /// Delete the data represented by the dataId from data store.
        /// </summary>
        /// <param name="dataID">ID of the data/record to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DeleteData(UInt32 dataId);

        //Returns the size of the NVM, if 0 operation failed
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        // native call that destroys record created on the flash
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DisposeNativeMemoryPointer(UInt32 dataId);
        
    }


    public class DataStore
    {

        private enum DATASTORE_ERROR
        {
            DATASTORE_ERROR_NONE,
            DATASTORE_ERROR_INVALID_PARAM,
            DATASTORE_ERROR_OUT_OF_MEMORY,
            DATASTORE_ERROR_INVALID_DATA_ID,
            DATASTORE_ERROR_INVALID_GIVEN_ADDR,
            DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
            DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
            DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED,
            DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS,
            DATASTORE_ERROR_UNEXPECTED_ERROR
        }

        private UInt32 m_Size = 0;
        //private UInt16 Blocks=0;
        //private float UsedKiloBytes=0;
        //private float FreeKiloBytes=0;

        // Stores the current record id
        //private static UInt32 currentdataId = 0;

        ////private byte[] buffer;
        /// <summary>
        /// The block storage device type
        /// </summary>
        private int storageType;
        DataStore dStore;
        ////List<Data> dataObj = new List<Data>();

        public DataStore(int storageType)
        {
            this.storageType = storageType;
            bool retVal = CreateDataStore();
            /*if( retVal == false )
                throw new SystemException();*/
        }

        //////////////////////////public properties and methods/////////////////////

        /// <summary>
        /// Size of the DataStore in Kilo Bytes
        /// </summary>
        public UInt32 Size
        {
            get
            {
                return m_Size;
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space already occupied by data
        /// </summary>
        public float UsedKBytes
        {
            get
            {
                return GetUsedKBytes();
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space available to store data
        /// </summary>
        public float FreeKBytes
        {
            get
            {
                return GetFreeKBytes();
            }
        }

        /*public int TempCreateRecord(UInt32 dataId, UInt32 Size)
        {
            return CreateRecord(dataId, Size);
        }*/

        /*public void addDataReference(UInt32 dataId)
        {
            dataObj.Add(dataId);
        }*/

        //public bool ReadAllDataIds(UInt32 dataId, UInt32 Size)
        public DataStatus ReadAllDataIds(int[] dataIdArray)
        {
            if (GetReadAllDataIds(dataIdArray) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        public UInt32 CountOfDataIds()
        {
            return GetCountOfDataIds();
        }

        public DataStatus ReadAllDataReferences(DataStore dStore, Data[] dataRefArray, UInt16 dataIdOffset)
        {
            UInt16 dataId;
            int[] dataIdArray = new int[GetCountOfDataIds()];
            if (GetReadAllDataIds(dataIdArray) == false)
                return DataStatus.Failure;
            else
            {
                for (UInt16 arrayIndex = 0; arrayIndex < dataRefArray.Length; ++arrayIndex)
                {
                    dataId = (UInt16)dataIdArray[dataIdOffset];
                    dataRefArray[arrayIndex] = new Data(dStore, dataId);
                    ++dataIdOffset;
                }
            }
            return DataStatus.Success;
        }

        /*public UInt32 CountOfDataReferences()
        {
            return (UInt32)dataObj.Count;
        }*/

        public static DataStatus DeleteAllData()
        {
            if (DeleteAll() == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        public static DataStatus GC()
        {
            if(DataStoreGC() == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        public static DataStatus EraseAll()
        {
            if(DataStore.DeleteAllData() != DataStatus.Success)
                return DataStatus.Failure;
            
            if(DataStore.GC() != DataStatus.Success)
                return DataStatus.Failure;

            return DataStatus.Success;
        }

        public bool ReadWriteStatus()
        {
            return GetReadWriteStatus();
        }

        public int LastErrorStatus()
        {
            return GetLastErrorStatus();
        }
    

        /// <summary>
        /// Create a data store element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*private DataStore(UInt32 MaxDataSize)
        {
            dataId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /// <summary>
        /// Create a data store element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartdataId">Data ID of the first data recrod to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*private DataStore(UInt32 StartDataId, UInt32 MaxDataSize)
        {
            dataId = StartDataId;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/     

        /// <summary>
        /// Write an integer to the data store
        /// </summary>
        /// <param name="data">Integer to be stored</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        public bool Write(UInt32 address, byte[] data)
        {
            //byte[] buffer = new byte[sizeof(int)];
            return Write(address, data, (UInt32)data.Length, storageType);
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public bool Write(UInt32 address, byte[] data, UInt32 numBytes)
        {
            return Write(address, data, numBytes, storageType);
        }

        /// <summary>
        /// Read a byte array from the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public bool Read(UInt32 address, byte[] data)
        {
            return Read(address, data, storageType);
        }

        // destroys the record on the flash
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(dataId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateData(UInt32 dataId, UInt32 Size);

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool CreateDataStore();

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetUsedKBytes();

        // get amount of free space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetFreeKBytes();

        /// <summary>
        /// Returns valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns array with valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadAllDataIds(int[] dataIdArray);

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadAllDataReferences(int[] dataArray, UInt32 CountOfData);*/

        /// <summary>
        /// Get the number of valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns a the number of valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetCountOfDataIds();

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetCountOfDataReferences();*/

        /// <summary>
        /// Delete all records in the Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DeleteAll();

        /// <summary>
        /// Garbage collects inactive records in Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DataStoreGC();

        /// <summary>
        /// Gets read/write status of the Data Store
        /// </summary>
        /// <returns>Returns status of read/write on data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool GetReadWriteStatus();

        /// <summary>
        /// Gets error status of data Store
        /// </summary>
        /// <returns>Returns error status of data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static int GetLastErrorStatus();

        /// <summary>
        /// Read data the data record represented by the dataId, into the buffer
        /// </summary>
        /// <param name="dataId">ID of the record to be read</param>
        /// <param name="buffer">Byte buffer into which the data will be read</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Read(UInt32 address, byte[] buffer, int storageType);

        /// <summary>
        /// Read the data record IDs currently stored in the NVM into the unsigned integer array provided
        /// </summary>
        /// <param name="buffer">Buffer into which the data record IDs will be read into</param>
        /// <returns>Returns the number of data iDs read</returns>
        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt16 ReadAllDataIDs(UInt32[] buffer);*/
        
        /// <summary>
        /// Delete the data represented by the dataID from data store.
        /// </summary>
        /// <param name="dataID">ID of the data to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Delete(UInt32 dataId);

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 numBytes, int storageType);       
        
        // native call that destroys record created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 dataId);
        
    }
}
