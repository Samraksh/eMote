using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.DSP
{
    public class Transforms
    {
        /**********************************************FFT******************************/
        /// <summary>
        /// Returns the Fast Fourier Transfrom of the byte array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a byte array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte[] FFT(byte[] X, UInt16 transformSize);


        /// <summary>
        /// Returns the Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a UInt16 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16[] FFT(UInt16[] X, UInt16 transformSize);

        /// <summary>
        /// Returns the Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a UInt32 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32[] FFT(UInt32[] X, UInt16 transformSize);

        /**********************************************Inverse FFT******************************/
        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the byte array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a byte array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte[] IFFT(byte[] X, UInt16 transformSize);


        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a UInt16 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16[] IFFT(UInt16[] X, UInt16 transformSize);

        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="X">The input array</param>
        /// <returns>Returns a UInt32 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32[] IFFT(UInt32[] X, UInt16 transformSize);








    }
}
