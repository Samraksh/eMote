////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;

namespace Microsoft.SPOT.Input
{

    /// <summary>
    ///     The ButtonState enumeration describes the state that buttons
    ///     can be in.
    /// </summary>
    [Flags]
    public enum ButtonState : byte
    {
        /// <summary>
        ///     No state (same as up).
        /// </summary>
        None = 0,

        /// <summary>
        ///    The button is down.
        /// </summary>
        Down = 1,

        /// <summary>
        ///    The button is held
        /// </summary>
        Held = 2
    }
}


