////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>


GPIO_PIN VirtualKey_GetPins(UINT32 virtualKey)
{
    GPIO_PIN pin;

    switch (virtualKey)
    {
    case VK_MENU:
        pin = GPIO_PIN_NONE;
        break;
    case VK_SELECT:
        pin = GPIO_PIN_NONE;
        break;
    case VK_UP:
        pin = GPIO_PIN_NONE;
        break;
    case VK_DOWN:
        pin = GPIO_PIN_NONE;
        break;
    case VK_LEFT:
        pin = GPIO_PIN_NONE;
        break;
    case VK_RIGHT:
        pin = GPIO_PIN_NONE;
        break;
    case VK_BACK:
        pin = GPIO_PIN_NONE;
        break;
    case VK_HOME:
        pin = GPIO_PIN_NONE;
        break;
    default:
        pin = GPIO_PIN_NONE;
        break;
    }
   
   return pin;
}

