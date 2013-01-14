using System;
using Microsoft.SPOT;

namespace System.Runtime.InteropServices
{
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, Inherited = false)]
    public sealed class MarshalAsAttribute : Attribute
    {

        

        public MarshalAsAttribute(short unmanagedType)
        {
        }


    }
}
