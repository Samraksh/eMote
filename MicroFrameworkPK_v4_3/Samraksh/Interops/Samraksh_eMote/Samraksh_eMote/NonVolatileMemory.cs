/* Copyright: The Samaraksh Company (All rights reserved)
 * Author: Mukundan Sridharan, Ananth Muralidharan
 * Date: Feb 2013, Nov 2013
 * Function: Provides access to Samraksh NVM DataStore filesystem
 */

using System;
using System.Runtime.CompilerServices;


// ReSharper disable once CheckNamespace
namespace Samraksh.eMote.NonVolatileMemory
{
    /// <summary>
    /// Block device type
    /// </summary>
    public enum StorageType
    {
        NOR = 1,
        SD
    };

    /// <summary>
    /// DataStore error values 
    /// </summary>
    public enum DataStoreError
    {
        DataStoreErrorNone,
        DataStoreErrorInvalidParam,
        DataStoreErrorOutOfMemory,
        DataStoreErrorInvalidDataId,
        DataStoreErrorInvalidGivenAddr,
        DataStoreErrorOutOfBoundAccess,
        DataStoreErrorOutOfFlashMemory,
        DataStoreErrorDataIdAlreadyExists,
        DataStoreErrorUnexpectedError
    };

    /// <summary>
    /// Data types allowed in DataStore
    /// </summary>
    public enum ReferenceDataType
    {
        BYTE,
        UINT16,
        UINT32
    };

    //AnanthAtSamraksh - to be kept in sync with the one in Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore.cpp
    /// <summary>
    /// DataStore return types
    /// </summary>
    public enum DataStoreReturnStatus
    {
        Success = 0,
        Failure = -1,
        InvalidArgument = -2,
        InvalidReference = -3,
        DataStoreNotInitialized = -4,
        DataAllocationOutOfMemory = -5
        //AlreadyExists,
        //InvalidPointer
    };

    /// <summary>
    /// Different exception classes based on return value from native methods
    /// </summary>
    public class DataStoreInvalidArgumentException : Exception
    {
        public DataStoreInvalidArgumentException()
        {
        }

        public DataStoreInvalidArgumentException(string message)
            : base(message)
        {
        }

        public DataStoreInvalidArgumentException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    public class DataStoreInvalidReferenceException : Exception
    {
        public DataStoreInvalidReferenceException()
        {
        }

        public DataStoreInvalidReferenceException(string message)
            : base(message)
        {
        }

        public DataStoreInvalidReferenceException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    public class DataStoreNotInitializedException : Exception
    {
        public DataStoreNotInitializedException()
        {
        }

        public DataStoreNotInitializedException(string message)
            : base(message)
        {
        }

        public DataStoreNotInitializedException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    public class DataStoreOutOfMemoryException : Exception
    {
        public DataStoreOutOfMemoryException()
        {
        }

        public DataStoreOutOfMemoryException(string message)
            : base(message)
        {
        }

        public DataStoreOutOfMemoryException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    public class DataStoreException : Exception
    {
        public DataStoreException()
        {
        }

        public DataStoreException(string message)
            : base(message)
        {
        }

        public DataStoreException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }
    
    /// <summary>
    /// A data reference is is a pointer to a record in DataStore.
    /// Class methods to read and write from/to DataStore, to delete a data reference, and to get meta data about the reference.
    /// </summary>
    public class DataReference
    {
        private DataStore dStore;
        // Data Id unique to a pointer
        private UInt32 dataId;
        private UInt32 referenceSize = 0;
        private UInt32 dataReference;
        private ReferenceDataType referenceDataType;
        /// <summary>
        /// DataType of reference
        /// </summary>
        private Type dataType;

        private Byte dataTypeByte = new byte();
        private UInt16 dataTypeUInt16 = new UInt16();
        private UInt32 dataTypeUInt32 = new UInt32();


        /*/// <summary>
        /// Data constructor that takes DataStore and the data type as parameters. 
        /// Possible to create max of 256 active data objects.
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="dataType">Type of data to be stored in DataStore</param>
        public DataReference(DataStore dStore, Type dataType)
        {
            this.dStore = dStore;
            this.dataType = dataType;
        }*/

        /// <summary>
        /// Default data class constructor. Possible to create max of 256 active data objects.
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="m_Size">Size of the data object to be stored in DataStore. 
        /// Max size is (2^32 - 1) if type is bytes; (2^31 - 1) if type is uint16; (2^30 - 1) if type is uint32</param>
        /// <param name="referenceDataType">Type of data to be stored in DataStore</param>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        /// <exception caption="Argument Exception" cref="System.ArgumentException">Method invocation has an invalid argument</exception>
        public DataReference(DataStore dStore, int refSize, ReferenceDataType referenceDataType)
        {
            if(dStore == null)
                throw new DataStoreException("DataStore object is NULL. DataStore could be corrupt. Please try erasing the flash while initializing (eraseDataStore = true) and try again.");

            this.dStore = dStore;
            this.referenceDataType = referenceDataType;

            //Size allocation should not be zero or negative
            if (refSize <= 0)
            {
                throw new ArgumentException("refSize cannot be less than or equal to zero", "refSize");
            }

            if (referenceDataType == ReferenceDataType.BYTE)
            {
                refSize = sizeof(byte) * refSize;
                this.dataReference = (UInt32)CreateData((UInt32)refSize, 0);
                this.dataType = typeof(byte);
            }
            else if (referenceDataType == ReferenceDataType.UINT16)
            {
                refSize = sizeof(UInt16) * refSize;
                this.dataReference = (UInt32)CreateData((UInt32)refSize, 1);
                this.dataType = typeof(UInt16);
            }
            else if (referenceDataType == ReferenceDataType.UINT32)
            {
                refSize = sizeof(UInt32) * refSize;
                this.dataReference = (UInt32)CreateData((UInt32)refSize, 2);
                this.dataType = typeof(UInt32);
            }
            else
            {
                throw new ArgumentException("Invalid dataType", "dataType");
            }

            if (this.dataReference == 0)
                throw new DataStoreException("DataPointer is NULL. Data could not be created.");

            int datastoreStatus = GetLastDatastoreStatus();
            if (datastoreStatus == (int)DataStoreReturnStatus.Failure)
                throw new DataStoreException("Failure while creating data reference");
            else if (datastoreStatus == (int)DataStoreReturnStatus.InvalidArgument)
                throw new DataStoreInvalidArgumentException("Failure - Invalid argument");
            else if (datastoreStatus == (int)DataStoreReturnStatus.InvalidReference)
                throw new DataStoreInvalidReferenceException("Failure - Invalid reference");
            else if (datastoreStatus == (int)DataStoreReturnStatus.DataStoreNotInitialized)
                throw new DataStoreNotInitializedException("Failure - datastore not initialized");
            else if (datastoreStatus == (int)DataStoreReturnStatus.DataAllocationOutOfMemory)
                throw new DataStoreOutOfMemoryException("Failure - out of memory during data allocation");

            this.referenceSize = (UInt32)refSize;
            this.dataId = GetDataID();

            if (this.dataId == 0)
                throw new DataStoreException("DataId is zero. Data could not be created.");
        }

        /// <summary>
        /// Returns a data object corresponding to the input parameter data ID.
        /// Possible to create max of 256 active data objects.
        /// </summary>Rea
        /// <param name="dStore">DataStore object</param>
        /// <param name="dataId">ID of data for which the user wants a reference/handle</param>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        /// <exception caption="Argument Exception" cref="System.ArgumentException">Method invocation has an invalid argument</exception>
        public DataReference(DataStore dStore, int dataId)
        {
            if (dStore == null)
                throw new DataStoreException("DataStore object is NULL. DataStore could be corrupt. Please try erasing the flash while initializing (eraseDataStore = true) and try again.");

            //dataId should not be negative
            if (dataId < 0)
                throw new ArgumentException("dataId cannot be less than zero", "dataId");
            
            this.dStore = dStore;
            this.dataId = (UInt32)dataId;
            this.dataReference = (UInt32)GetDataReference((UInt32)dataId);
            if (this.dataReference == 0)
                throw new DataStoreException("Reference to data is NULL. Data was not created.");

            this.referenceSize = (UInt32)LookupDataSize((UInt32)dataId);
            if (this.referenceSize == 0)
                throw new DataStoreException("Data size is zero. Data was not created.");

            int retType = LookupDataType((UInt32)dataId);
            if (retType == 0)
            {
                this.referenceDataType = ReferenceDataType.BYTE;
                this.dataType = typeof(byte);
            }
            else if (retType == 1)
            {
                this.referenceDataType = ReferenceDataType.UINT16;
                this.dataType = typeof(UInt16);
            }
            else if (retType == 2)
            {
                this.referenceDataType = ReferenceDataType.UINT32;
                this.dataType = typeof(UInt32);
            }
            //m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }

        /// <summary>Write a byte array into DataStore, specifying the amount to write and the offset from the start of the record.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="offset">Offset from start of DataStore allocationto write. Even value recommended for byte data type</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(byte[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            //Since the NOR flash cannot write to odd addresses.
            if (offset % 2 == 1)
            {
                //offset = offset + sizeof(byte);
                throw new DataStoreException("Offset should not be odd for byte dataType");
            }

            retVal = dStore.Write((uint)this.dataReference, data, (UInt32)offset, (UInt32)numData, dataTypeByte);
            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;

        }

        /// <summary>Write a ushsort (UInt16) array into DataStore, specifying the amount to write and the offset from the start of the record.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="offset">Offset from start of DataStore allocationto write.</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt16[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt16) + 0] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt16) + 1] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a uint (UInt32) array into DataStore, specifying the amount to write and the offset from the start of the record.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="offset">Offset from start of DataStore allocationto write.</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt32[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt32) + 0] = (byte)(data[objIndex] >> 24);
                byteArray[objIndex * sizeof(UInt32) + 1] = (byte)(data[objIndex] >> 16);
                byteArray[objIndex * sizeof(UInt32) + 2] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt32) + 3] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a byte array into DataStore, specifying the amount to write.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(byte[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            retVal = dStore.Write((uint)this.dataReference, data, offset, (UInt32)numData, dataTypeByte);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a ushsort (UInt16) array into DataStore, specifying the amount to write.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt16[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt16) + 0] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt16) + 1] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a uint (UInt32) array into DataStore, specifying the amount to write.</summary>
        /// <param name="data">Array to write</param>
        /// <param name="numData">Count of data to be written to DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt32[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt32) + 0] = (byte)(data[objIndex] >> 24);
                byteArray[objIndex * sizeof(UInt32) + 1] = (byte)(data[objIndex] >> 16);
                byteArray[objIndex * sizeof(UInt32) + 2] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt32) + 3] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>Write a byte array into DataStore.</summary>
        /// <param name="data">Array to write</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(byte[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            retVal = dStore.Write((uint)this.dataReference, data, offset, (UInt32)data.Length, dataTypeByte);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a ushsort (UInt16) array into DataStore.</summary>
        /// <param name="data">Array to write</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt16[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)data.Length;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < data.Length; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt16) + 0] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt16) + 1] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Write a uint (UInt32) array into DataStore.</summary>
        /// <param name="data">Array to write</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Write(UInt32[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)data.Length;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < data.Length; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt32) + 0] = (byte)(data[objIndex] >> 24);
                byteArray[objIndex * sizeof(UInt32) + 1] = (byte)(data[objIndex] >> 16);
                byteArray[objIndex * sizeof(UInt32) + 2] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt32) + 3] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>Read a byte array from DataStore, specifying the amount to read and the offset from start of allocation.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(byte[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(byte) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeByte);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (byte)(byteArray[objIndex * sizeof(byte)]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a ushort (UInt16) array from DataStore, specifying the amount to read and the offset from start of allocation.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt16[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt16)((byteArray[objIndex * sizeof(UInt16) + 0] << 8) +
                                                        byteArray[objIndex * sizeof(UInt16) + 1]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a uint (UInt16) array from DataStore, specifying the amount to read and the offset from start of allocation.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt32[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt32)((byteArray[objIndex * sizeof(UInt32) + 0] << 24) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 1] << 16) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 2] << 8) +
                                                    byteArray[objIndex * sizeof(UInt32) + 3]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>Read a byte array from DataStore, specifying the amount to read.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(byte[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("Data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("NumData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as that used to create");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            UInt32 numBytes = sizeof(byte) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, offset, numBytes, dataTypeByte);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (byte)(byteArray[objIndex * sizeof(byte)]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a ushort (UInt16) array from DataStore, specifying the amount to read.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt16[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("NumData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as that used to create");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt16)((byteArray[objIndex * sizeof(UInt16) + 0] << 8) +
                                                        byteArray[objIndex * sizeof(UInt16) + 1]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a uint (UInt16) from DataStore, specifying the amount to read.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt32[] data, int numData)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("Data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("NumData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.referenceSize)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt32)((byteArray[objIndex * sizeof(UInt32) + 0] << 24) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 1] << 16) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 2] << 8) +
                                                    byteArray[objIndex * sizeof(UInt32) + 3]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>Read a byte array from DataStore.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(byte[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("Data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.BYTE)
                throw new DataStoreException("DataType is not the same as that used to create");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(byte) * (UInt32)data.Length;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, offset, numBytes, dataTypeByte);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < data.Length; ++objIndex)
                {
                    data[objIndex] = (byte)(byteArray[objIndex * sizeof(byte)]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a ushort (UInt16) array from DataStore.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt16[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("Data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT16)
                throw new DataStoreException("DataType is not the same as that used to create");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)data.Length;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < data.Length; ++objIndex)
                {
                    data[objIndex] = (UInt16)((byteArray[objIndex * sizeof(UInt16) + 0] << 8) +
                                                        byteArray[objIndex * sizeof(UInt16) + 1]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>Read a uint (UInt16) from DataStore, specifying the amount to read and the offset from start of allocation.</summary>
        /// <param name="data">Array to receive data read</param>
        /// <returns>Success / failure status</returns>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus Read(UInt32[] data)
        {
            int retVal = 0;
            UInt32 offset = 0;

            if (data.Length == 0)
                throw new DataStoreException("Data array cannot be of zero length");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.referenceDataType != ReferenceDataType.UINT32)
                throw new DataStoreException("DataType is not the same as that used to create");

            //Amount of data to be written should not be greater than allocation size
            if (data.Length > this.referenceSize)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)data.Length;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < data.Length; ++objIndex)
                {
                    data[objIndex] = (UInt32)((byteArray[objIndex * sizeof(UInt32) + 0] << 24) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 1] << 16) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 2] << 8) +
                                                    byteArray[objIndex * sizeof(UInt32) + 3]);
                }
                return DataStoreReturnStatus.Success;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>
        /// Delete data from DataStore
        /// </summary>
        /// <returns>Success / failure status</returns>
        public DataStoreReturnStatus Delete()
        {
            if (DeleteData(dataId) == true)
                return DataStoreReturnStatus.Success;
            else
                return DataStoreReturnStatus.Failure;
        }


        /// <summary>
        /// Return data reference type 
        /// </summary>
        [Obsolete("Deprecated. Use Type instead.")]
        public Type getDataReferenceType
        {
            get
            {
                return this.dataType;
            }
        }

        /// <summary>
        /// Return data reference type 
        /// </summary>
        public Type Type {
            get {
                return dataType;
            }
        }

        /// <summary>
        /// Return data reference size 
        /// </summary>
        [Obsolete("Deprecated. Use Size instead.")]
        public int getDataReferenceSize
        {
            get
            {
                return (int)this.referenceSize;
            }
        }

        /// <summary>
        /// Return data reference size 
        /// </summary>
        public int Size 
        {
            get {
                return (int)referenceSize;
            }
        }


        /*~Data()
        {
            DisposeNativeMemoryPointer(dataId);
            Debug.Print("Destructor");
        }*/


        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Get data reference for specified data ID from DataStore
        /// </summary>
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Address reference to the dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetDataReference(UInt32 dataId);

        /// <summary>
        /// Get the data type of dataID
        /// </summary>
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Data type</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int LookupDataType(UInt32 dataId);

        /// <summary>
        /// Get the allocation size for dataID
        /// </summary>
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Allocation size</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int LookupDataSize(UInt32 dataId);

        /// <summary>
        /// Delete the data represented by the data id from DataStore.
        /// </summary>
        /// <param name="dataId">ID of the data to be deleted</param>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DeleteData(UInt32 dataId);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeByte">Data type - byte</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, byte dataTypeByte);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt16">Data type - UInt16</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, UInt16 dataTypeUInt16);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt32">Data type - UInt32</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, UInt32 dataTypeUInt32);

        /// <summary>
        /// Returns the current data ID
        /// </summary>
        /// <returns>Current DataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetDataID();

        /// <summary>
        /// Gets error status of DataStore
        /// </summary>
        /// <returns>DataStore error status</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetLastDatastoreStatus();


        /*/// <summary>
        /// 
        /// </summary>
        /// <param name="dataId"></param>
        /// <param name="bytes"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);*/

        /// <summary>
        /// Native call that destroys data created on the flash
        /// </summary>
        /// <param name="dataId"></param>
        /// Write data array to DataStore starting from specified offset
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DisposeNativeMemoryPointer(UInt32 dataId);

    }

    /// <summary>
    /// DataStore class to store data. 
    /// Can be any implemented block storage device.
    /// </summary>
    public class DataStore
    {

        /// <summary>
        /// The block storage device type
        /// </summary>
        private StorageType storageType;

        /// <summary>
        /// The only instance of DataStore.
        /// </summary>
        private static DataStore DSInstance;
        /// <summary>
        /// Counter for multiton pattern 
        /// </summary>
        //private static int counter = 0;
        /// <summary>
        /// For synchronization between multiple threads
        /// </summary>
        private static object syncObject = new Object();
        /// <summary>
        /// HashTable for storing multiple instances of DataStore initialized to different block storage devices
        /// </summary>
        private static System.Collections.Hashtable dataStoreInstances = new System.Collections.Hashtable();

        /// <summary>
        /// Private constructor so that only one instance of DataStore exists.
        /// </summary>
        private DataStore() { }

        /// <summary>
        /// Multiton pattern. This allows DataStore to be initialized with different block storage devices such as NOR, NAND etc.
        /// http://en.wikipedia.org/wiki/Multiton_pattern
        /// </summary>
        public static DataStore Instance(StorageType storageTypeKey, bool eraseDataStore = false)
        {
            if (!dataStoreInstances.Contains(storageTypeKey))
            {
                lock (syncObject)
                {
                    DSInstance = new DataStore();
                    int retVal = DSInstance.InitDataStore(storageTypeKey, eraseDataStore);
                    if (retVal != 0)
                        return null;
                    dataStoreInstances.Add(storageTypeKey, DSInstance);
                }
            }
            return (DataStore)(dataStoreInstances[storageTypeKey]);
        }

        /// <summary>
        /// Initializes DataStore with specified storage type.
        /// </summary>
        /// <param name="storageType">Storage type</param>
        private int InitDataStore(StorageType storageType, bool eraseDataStore)
        {
            this.storageType = storageType;
            return CreateDataStore(eraseDataStore);
        }

        //////////////////////////public properties and methods/////////////////////

        /// <summary>
        /// Maximum allocation size possible in bytes
        /// </summary>
        public int MaxAllocationSize
        {
            get
            {
                return GetMaxAllocationSize();
            }
        }

        /// <summary>
        /// Size of DataStore in bytes
        /// </summary>
        public int Size
        {
            get
            {
                return GetTotalSpace();
            }
        }

        /// <summary>
        /// Number of bytes of space already occupied by data
        /// </summary>
        public int UsedBytes
        {
            get
            {
                return GetUsedBytes();
            }
        }

        /// <summary>
        /// Number of bytes of space available to store data
        /// </summary>
        public int FreeBytes
        {
            get
            {
                return GetFreeBytes();
            }
        }

        
        /// <summary>
        /// Fills an array with data references from specified offset.
        /// </summary>
        /// <param name="dataRefArray">Data reference array</param>
        /// <param name="dataIdOffset">Offset from first data reference in DataStore</param>
        /// <returns>Success / failure status</returns>
        /// <remarks>
        ///     Typically an offset variable. passed as the second argument, is initialized to zero 
        ///     and incremented by the size of the data reference array after each invocation of the method. 
        ///     This will iteratively return all data references. 
        ///     If, after invocation, the array is null then no more data references.
        /// </remarks>
        /// <exception caption="DataStore Exception" cref="DataStoreException">Method invocation has an invalid argument</exception>
        public DataStoreReturnStatus ReadAllDataReferences(DataReference[] dataRefArray, int dataIdOffset)
        {
            UInt32 dataId;
            if (dataRefArray.Length == 0)
                throw new DataStoreException("dataRefArray cannot be of zero length");

            if (dataIdOffset < 0)
                throw new DataStoreException("dataIdOffset should not be negative");

            /* User checks if array contents are null to break out of program. 
                * Clear the contents of array so that null values are returned if there is no data allocation in DataStore. */
            Array.Clear(dataRefArray, 0, dataRefArray.Length);
            int[] dataIdArray = new int[dataRefArray.Length];

            int retVal = GetReadAllDataIds(dataIdArray, dataIdArray.Length, dataIdOffset);

            if (retVal != 0)
                return DataStoreReturnStatus.Failure;
            else
            {
                for (UInt16 arrayIndex = 0; arrayIndex < dataRefArray.Length; ++arrayIndex)
                {
                    dataId = (UInt32)dataIdArray[arrayIndex];
                    if (dataId != 0)
                    {
                        try
                        {
                            dataRefArray[arrayIndex] = new DataReference(this, (int)dataId);
                        }
                        catch (Exception)
                        {
                            return DataStoreReturnStatus.Failure;
                        }
                    }
                }
            }
            return DataStoreReturnStatus.Success;
        }

        /// <summary>
        /// Deletes all data references from DataStore. Does not actually erase the data, but marks them as inactive.
        /// </summary>
        /// <returns>Success / failure status</returns>
        public DataStoreReturnStatus DeleteAllData()
        {
            int retVal = DeleteAll();

            if (retVal == 0)
                return DataStoreReturnStatus.Success;
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>
        /// Performs garbage collection on DataStore. This is done internally by DataStore when there is not enough space to add new data.
        /// GC rearranges data internally so that all active data are grouped together. Then the inactive data are erased one block at a time.
        /// </summary>
        /// <returns>Success / failure status</returns>
        private DataStoreReturnStatus GC()
        {
            if (DataStoreGC() == true)
                return DataStoreReturnStatus.Success;
            else
                return DataStoreReturnStatus.Failure;
        }

        /// <summary>
        /// Deletes all data references from DataStore and erases them.
        /// </summary>
        /// <returns>Success / failure status</returns>
        public DataStoreReturnStatus EraseAllData()
        {
            // Remove contents from AddressTable also before erasing DataStore
            int retVal = DeleteAll();

            if (retVal == 0)
            {
                //DeleteAll was successful
                retVal = EraseAllBlocks();

                if (retVal == 0)
                    return DataStoreReturnStatus.Success;
                else if (retVal == -3)
                    return DataStoreReturnStatus.InvalidReference;
                else
                    return DataStoreReturnStatus.Failure;
            }
            else if (retVal == -3)
                return DataStoreReturnStatus.InvalidReference;
            else
                return DataStoreReturnStatus.Failure;
        }

        
        /// <summary>
        /// Write data array to DataStore starting from specified address
        /// </summary>
        /// <param name="address">Address to write to</param>
        /// <param name="data">Data to be written to DataStore</param>
        /// <param name="offset">Offset from start of DataStore allocationto write.</param>
        /// <param name="numBytes">Count of bytes to be written</param>
        /// <param name="dataType">Data type to be written</param>
        /// <returns>True if success</returns>
        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        /// <summary>
        /// Read data array from DataStore starting from specified address.
        /// </summary>
        /// <param name="address">Address to read from</param>
        /// <param name="data">Byte array to be filled up with data from DataStore</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numBytes">Number of bytes to be read</param>
        /// <param name="dataType">Data type of the data to be read</param>
        /// <returns>True if success</returns>
        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        /// <summary>
        /// Destroys the data in DataStore
        /// </summary>
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(dataId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Initializes DataStore. Performs a scan of the device and builds up the list of active data inside DataStore.
        /// </summary>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateDataStore(bool eraseDataStore);

        /// <summary>
        /// Method that returns maximum allocation size possible in DataStore
        /// </summary>
        /// <returns>Maximum allocation size possible in DataStore</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetMaxAllocationSize();

        /// <summary>
        /// Get total available space 
        /// </summary>
        /// <returns>Amount of total space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetTotalSpace();

        /// <summary>
        /// Get amount of used space 
        /// </summary>
        /// <returns>Amount of used space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetUsedBytes();

        /// <summary>
        /// Get amount of free space
        /// </summary>
        /// <returns>Amount of free space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetFreeBytes();

        /// <summary>
        /// Returns valid data currently stored in DataStore
        /// </summary>
        /// <param name="dataIdArray">Data ID array</param>
        /// <param name="arrayLength">Size of array</param>
        /// <param name="dataIdOffset">Offset</param>
        /// <returns>Array with valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetReadAllDataIds(int[] dataIdArray, int arrayLength, int dataIdOffset);

        /// <summary>
        /// Get the count of valid data currently stored in DataStore
        /// </summary>
        /// <returns>Count of valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetCountOfDataIds();

        /// <summary>
        /// Delete all references to data in DataStore
        /// </summary>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int DeleteAll();

        /// <summary>
        /// Erases all data in DataStore
        /// </summary>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int EraseAllBlocks();

        /// <summary>
        /// Garbage collects inactive data in DataStore
        /// </summary>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DataStoreGC();

        /// <summary>
        /// Gets read/write status of DataStore
        /// </summary>
        /// <returns>True if status indicates success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadWriteStatus();

        /// <summary>
        /// Gets error status of DataStore
        /// </summary>
        /// <returns>DataStore error status</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetLastDatastoreStatus();

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        /// <summary>
        /// Write data present in buffer starting from address and amount equal to numBytes. 
        /// </summary>
        /// <param name="address">Address from which to start writing data</param>
        /// <param name="data">Buffer holding data to be written</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numBytes">Amount of data to be written</param>
        /// <param name="dataType">Data type of the data to be written</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>True if success</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType, int storageType);

        
        /// <summary>
        /// Read from data into the buffer starting from address.
        /// </summary>
        /// <param name="address">Address of data to be read from</param>
        /// <param name="buffer">Buffer into which the data will be read</param>
        /// <param name="offset">Offset from start of DataStore allocationto read.</param>
        /// <param name="numBytes">Number of bytes to be read</param>
        /// <param name="dataType">Data type of the data to be read</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>Number of bytes read; -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, byte dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, UInt16 dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, UInt32 dataType, int storageType);


        // native call that destroys data created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 dataId);

    }
}
