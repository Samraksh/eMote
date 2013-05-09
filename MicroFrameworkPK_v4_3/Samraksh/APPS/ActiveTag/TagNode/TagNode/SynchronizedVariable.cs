using System;
using Microsoft.SPOT;

namespace Samraksh.APPS.ActiveTag
{
    public class SynchronizedVariable
    {
        private readonly Object _lock = new Object();
        private bool ready;

        public SynchronizedVariable()
        {
            ready = false;
        }

        public bool IsReady()
        {
            lock (_lock)
            {
                return ready;
            }
        }

        public void SetState(bool state)
        {
            lock (_lock)
            {
                ready = state;
            }
        }

    }
}
