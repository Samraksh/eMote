using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{

    public class MemPtr<T>
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        bool write(T data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        T read();
        
    }
}
