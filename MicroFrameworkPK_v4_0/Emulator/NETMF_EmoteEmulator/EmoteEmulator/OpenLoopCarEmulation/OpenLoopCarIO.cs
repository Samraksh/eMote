using System;
using Microsoft.SPOT;

namespace OpenLoopCarEmulation
{
    class CarPMInput
    {
        public short SteeringAngle; //-128to127
        public sbyte Accelaration;//int8 -63 to 63
        public byte State; //Special instructions 
    }

    class CarPMOutput
    {
        public ushort X; //0-65535
        public ushort Y; //0-65535
        public byte speed;//0-255
        public short direction; //degrees 
    }
}
