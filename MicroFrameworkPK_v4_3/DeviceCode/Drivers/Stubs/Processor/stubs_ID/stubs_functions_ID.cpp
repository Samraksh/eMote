/**
 * @file:    stubs_functions_ID.cpp
 * @author:  Michael Andrew McGrath
 * @license: Copyright The Samraksh Company, 2015
 * @date:    April 27, 2015
 */

#include <tinyhal.h>
/**
 * @details: Design Goal: fetch unique IDs stored in the QFPROM and share them
 *  via the config region as the OEM serial number.
 */

unsigned int ID_GetSerialNumbers(struct OEM_SERIAL_NUMBERS& OemSerialNumbers)
{
    //TODO:return device-unique serial number
    return 0;
}
