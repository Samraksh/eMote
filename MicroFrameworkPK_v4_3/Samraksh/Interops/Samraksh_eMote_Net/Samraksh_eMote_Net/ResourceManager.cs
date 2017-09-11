using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net
{
    enum ResourceTypesE
    {
        RADIO,
        MAC,
        FRAMEWORK,
        ASNP,
        APP
    }

    enum NetResourcesE
    {
        Si_Radio,
        RF231_Radio,
        LoRA_Radio,
        CSMA_MAC,
        OMAC_MAC,
        Tuscarora,
        CTP_ASNP,
    }

    partial class Lock
    {
        public Lock() { }
        public void GetResource() { } 
    };

    class ResourceManager
    {
        public Lock GetLock(ResourceTypesE type, NetResourcesE res) { return new Lock(); }
        public bool ReleaseLock(ref Lock loc) { return false; }
    };
}
