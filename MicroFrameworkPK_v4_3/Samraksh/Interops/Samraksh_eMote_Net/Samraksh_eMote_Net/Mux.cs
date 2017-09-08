using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net
{
    //Implements features to handle multiple users on top and demux callbacks to each user.
    class Mux
    {
        static byte userId;
        byte Init()
        {
           return ++userId;
        }
    }
}
