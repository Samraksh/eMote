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
        /// <param name="input">The input array</param>
        /// <returns>Returns a byte array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool FFT(byte[] input, byte[] output, UInt16 transformSize);


        /// <summary>
        /// Returns the Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="input">The input array</param>
        /// <returns>Returns a UInt16 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool FFT(UInt16[] input, UInt16[] output,  UInt16 transformSize);

        /// <summary>
        /// Returns the Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="input">The input array</param>
        /// <returns>Returns a UInt32 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool FFT(UInt32[] input, UInt32[] output, UInt16 transformSize);

        /**********************************************Inverse FFT******************************/
        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the byte array X
        /// </summary>
        /// <param name="input">The input array</param>
        /// <returns>Returns a byte array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool IFFT(byte[] input, byte[] output, UInt16 transformSize);


        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="input">The input array</param>
        /// <returns>Returns a UInt16 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool IFFT(UInt16[] input, UInt16[] output, UInt16 transformSize);

        /// <summary>
        /// Returns the Inverse Fast Fourier Transfrom of the UInt16 array X
        /// </summary>
        /// <param name="input">The input array</param>
        /// <returns>Returns a UInt32 array of size 'transformSize'</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool IFFT(UInt32[] input, UInt32[] output, UInt16 transformSize);








    }
}
