////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System.Threading;
//using Microsoft.SPOT.IO;
using Microsoft.SPOT;
using Samraksh.eMote.DotNow;

namespace System.IO
{

    public class FileStream : Stream
    {
        // Driver data

        private NativeFileStream _nativeFileStream;
        private FileSystemManager.FileRecord _fileRecord;
        private String _fileName;
        private bool _canRead;
        private bool _canWrite;
        private bool _canSeek;

        private long _seekLimit;

        private bool _disposed;

        //--//

        public FileStream(String path, FileMode mode)
            : this(path, mode, (mode == FileMode.Append ? FileAccess.Write : FileAccess.ReadWrite), FileShare.Read, NativeFileStream.BufferSizeDefault)
        {
        }

        public FileStream(String path, FileMode mode, FileAccess access)
            : this(path, mode, access, FileShare.Read, NativeFileStream.BufferSizeDefault)
        {
        }

        public FileStream(String path, FileMode mode, FileAccess access, FileShare share)
            : this(path, mode, access, share, NativeFileStream.BufferSizeDefault)
        {
        }

        public FileStream(String path, FileMode mode, FileAccess access, FileShare share, int bufferSize)
        {
            //Debug.Print("Entering FileStream constructor");
            // This will perform validation on path
            _fileName = Path.GetFullPath(path);
            //Debug.Print("FileStream constructor - step 1");

            // make sure mode, access, and share are within range
            if (mode < FileMode.CreateNew || mode > FileMode.Append ||
                access < FileAccess.Read || access > FileAccess.ReadWrite ||
                share < FileShare.None || share > FileShare.ReadWrite)
            {
                throw new ArgumentOutOfRangeException();
            }

            //Debug.Print("FileStream constructor - step 2");

            // Get wantsRead and wantsWrite from access, note that they cannot both be false
            bool wantsRead = (access & FileAccess.Read) == FileAccess.Read;
            bool wantsWrite = (access & FileAccess.Write) == FileAccess.Write;

            //Debug.Print("FileStream constructor - wantsRead " + wantsRead.ToString());
            //Debug.Print("FileStream constructor - wantsWrite " + wantsWrite.ToString());

            // You can't open for readonly access (wantsWrite == false) when
            // mode is CreateNew, Create, Truncate or Append (when it's not Open or OpenOrCreate)
            if (mode != FileMode.Open && mode != FileMode.OpenOrCreate && !wantsWrite)
            {
                throw new ArgumentException();
            }

            //Debug.Print("FileStream constructor - step 3");

            // We need to register the share information prior to the actual file open call (the NativeFileStream ctor)
            // so subsequent file operation on the same file will behave correctly
            _fileRecord = FileSystemManager.AddToOpenList(_fileName, (int)access, (int)share);

            //Debug.Print("FileStream constructor - step 4a");

            try
            {
                //Debug.Print("FileStream constructor - step 4b");
                uint attributes = NativeIO.GetAttributes(_fileName);
                //Debug.Print("FileStream constructor - step 4c");
                bool exists = (attributes != 0xFFFFFFFF);
                //Debug.Print("FileStream constructor - step 4d");
                bool isReadOnly = (exists) ? (((FileAttributes)attributes) & FileAttributes.ReadOnly) == FileAttributes.ReadOnly : false;
                /*Debug.Print("FileStream constructor - step 4e exists : " + exists.ToString());
                Debug.Print("FileStream constructor - step 4e attributes : " + attributes.ToString());
                Debug.Print("FileStream constructor - step 4e isReadOnly : " + isReadOnly.ToString());*/
                //Removing below debug statement causes a crash. Not sure what the reason is, as crash happens even when I add a sleep(1) surrounded by while for a total looping time of 10 ms.
                Debug.Print("FileStream constructor - step 4e (FileAttributes)attributes : " + (((FileAttributes)attributes) & FileAttributes.Directory).ToString());
                
                
                // If the path specified is an existing directory, fail
                if (exists && ((((FileAttributes)attributes) & FileAttributes.Directory) == FileAttributes.Directory))
                {
                    //Debug.Print("FileStream constructor - step 4f");
                    throw new IOException("", (int)IOException.IOExceptionErrorCode.UnauthorizedAccess);
                }

                //Debug.Print("FileStream constructor - step 4g");

                // The seek limit is 0 (the beginning of the file) for all modes except Append
                _seekLimit = 0;

                //Debug.Print("FileStream constructor - step 4h");

                switch (mode)
                {
                    case FileMode.CreateNew: // if the file exists, IOException is thrown
                        //Debug.Print("FileStream constructor - step 5a");
                        if (exists) throw new IOException("", (int)IOException.IOExceptionErrorCode.PathAlreadyExists);
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        break;

                    case FileMode.Create: // if the file exists, it should be overwritten
                        //Debug.Print("FileStream constructor - step 5b");
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        if (exists) _nativeFileStream.SetLength(0);
                        break;

                    case FileMode.Open: // if the file does not exist, IOException/FileNotFound is thrown
                        //Debug.Print("FileStream constructor - step 5c");
                        if (!exists) throw new IOException("", (int)IOException.IOExceptionErrorCode.FileNotFound);
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        break;

                    case FileMode.OpenOrCreate: // if the file does not exist, it is created
                        //Debug.Print("FileStream constructor - step 5d - enter");
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        //Debug.Print("FileStream constructor - step 5d - exit");
                        break;

                    case FileMode.Truncate: // the file would be overwritten. if the file does not exist, IOException/FileNotFound is thrown
                        //Debug.Print("FileStream constructor - step 5e");
                        if (!exists) throw new IOException("", (int)IOException.IOExceptionErrorCode.FileNotFound);
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        _nativeFileStream.SetLength(0);
                        break;

                    case FileMode.Append: // Opens the file if it exists and seeks to the end of the file. Append can only be used in conjunction with FileAccess.Write
                        // Attempting to seek to a position before the end of the file will throw an IOException and any attempt to read fails and throws an NotSupportedException
                        //Debug.Print("FileStream constructor - step 5f");
                        if (access != FileAccess.Write) throw new ArgumentException();
                        _nativeFileStream = new NativeFileStream(_fileName, bufferSize);
                        _seekLimit = _nativeFileStream.Seek(0, (uint)SeekOrigin.End);
                        break;

                    // We've already checked the mode value previously, so no need for default
                    //default:
                    //    throw new ArgumentOutOfRangeException();
                }

                // Now that we have a valid NativeFileStream, we add it to the FileRecord, so it could gets clean up
                // in case an eject or force format
                //Debug.Print("FileStream constructor - step 5g");
                _fileRecord.NativeFileStream = _nativeFileStream;

                //Debug.Print("FileStream constructor - step 6");

                // Retrive the filesystem capabilities
                _nativeFileStream.GetStreamProperties(out _canRead, out _canWrite, out _canSeek);
                _canRead = true;
                _canWrite = true;
                _canSeek = true;

                //Debug.Print("FileStream constructor - step 7");

                // If the file is readonly, regardless of the filesystem capability, we'll turn off write
                if (isReadOnly)
                {
                    //Debug.Print("FileStream constructor - step 8");
                    _canWrite = false;
                }

                // Make sure the requests (wantsRead / wantsWrite) matches the filesystem capabilities (canRead / canWrite)
                if ((wantsRead && !_canRead) || (wantsWrite && !_canWrite))
                {
                    //Debug.Print("FileStream constructor - step 9");
                    Debug.Print("FileStream constructor - _canRead " + _canRead.ToString());
                    Debug.Print("FileStream constructor - _canWrite " + _canWrite.ToString());
                    throw new IOException("", (int)IOException.IOExceptionErrorCode.UnauthorizedAccess);
                }

                // finally, adjust the _canRead / _canWrite to match the requests
                if (!wantsWrite)
                {
                    //Debug.Print("FileStream constructor - step 10");
                    _canWrite = false;
                }
                else if (!wantsRead)
                {
                    //Debug.Print("FileStream constructor - step 11");
                    _canRead = false;
                }
            }
            catch
            {
                //Debug.Print("Catching exception in FileStream constructor");
                // something went wrong, clean up and re-throw the exception
                if (_nativeFileStream != null)
                {
                    _nativeFileStream.Close();
                }

                FileSystemManager.RemoveFromOpenList(_fileRecord);

                throw;
            }
            //Debug.Print("Exiting FileStream constructor");
        }

        protected override void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                try
                {
                    if (disposing)
                    {
                        _canRead = false;
                        _canWrite = false;
                        _canSeek = false;
                    }

                    if (_nativeFileStream != null)
                    {
                        _nativeFileStream.Close();
                    }
                }
                finally
                {
                    if (_fileRecord != null)
                    {
                        FileSystemManager.RemoveFromOpenList(_fileRecord);
                        _fileRecord = null;
                    }

                    _nativeFileStream = null;
                    _disposed = true;
                }
            }
        }

        ~FileStream()
        {
            Dispose(false);
        }

        // This is for internal use to support proper atomic CopyAndDelete
        internal void DisposeAndDelete()
        {
            _nativeFileStream.Close();
            _nativeFileStream = null; // so Dispose(true) won't close the stream again
            NativeIO.Delete(_fileName);

            Dispose(true);
        }

        public override void Flush()
        {
            if (_disposed) throw new ObjectDisposedException();
            _nativeFileStream.Flush();
        }

        public override void SetLength(long value)
        {
            if (_disposed) throw new ObjectDisposedException();
            if (!_canWrite || !_canSeek) throw new NotSupportedException();

            // argument validation in interop layer
            _nativeFileStream.SetLength(value);
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            if (_disposed) throw new ObjectDisposedException();
            if (!_canRead) throw new NotSupportedException();

            lock (_nativeFileStream)
            {
                // argument validation in interop layer
                return _nativeFileStream.Read(buffer, offset, count, NativeFileStream.TimeoutDefault);
            }
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            if (_disposed) throw new ObjectDisposedException();
            if (!_canSeek) throw new NotSupportedException();

            long oldPosition = this.Position;
            long newPosition = _nativeFileStream.Seek(offset, (uint)origin);

            if (newPosition < _seekLimit)
            {
                this.Position = oldPosition;
                throw new IOException();
            }

            return newPosition;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            //Debug.Print("Entering FileStream Write");
            if (_disposed) throw new ObjectDisposedException();
            if (!_canWrite) throw new NotSupportedException();
            
            // argument validation in interop layer
            int bytesWritten;

            //Debug.Print("FileStream Write step 2");
            lock (_nativeFileStream)
            {
                //Debug.Print("FileStream Write step 3");
                // we check for count being != 0 because we want to handle negative cases
                // as well in the interop layer
                while (count != 0)
                {
                    //Debug.Print("FileStream Write step 4");
                    int i = 0;
                    while (i < 1)
                    {
                        System.Threading.Thread.Sleep(1);
                        i++;
                    }
                    bytesWritten = _nativeFileStream.Write(buffer, offset, count, NativeFileStream.TimeoutDefault);
                    //Debug.Print("FileStream Write step 5");

                    if (bytesWritten == 0) throw new IOException();

                    offset += bytesWritten;
                    count -= bytesWritten;
                }
            }
            //Debug.Print("Exiting FileStream Write");
        }

        public override bool CanRead
        {
            get { return _canRead; }
        }

        public override bool CanWrite
        {
            get { return _canWrite; }
        }

        public override bool CanSeek
        {
            get { return _canSeek; }
        }

        public virtual bool IsAsync
        {
            get { return false; }
        }

        public override long Length
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException();
                if (!_canSeek) throw new NotSupportedException();

                return _nativeFileStream.GetLength();
            }
        }

        public String Name
        {
            get { return _fileName; }
        }

        public override long Position
        {
            get
            {
                if (_disposed) throw new ObjectDisposedException();
                if (!_canSeek) throw new NotSupportedException();

                // argument validation in interop layer
                return _nativeFileStream.Seek(0, (uint)SeekOrigin.Current);
            }

            set
            {
                if (_disposed) throw new ObjectDisposedException();
                if (!_canSeek) throw new NotSupportedException();
                if (value < _seekLimit) throw new IOException();

                // argument validation in interop layer
                _nativeFileStream.Seek(value, (uint)SeekOrigin.Begin);
            }
        }
    }
}


