/**
 * @file:    Krait_ID.cpp
 * @author:  Michael Andrew McGrath
 * @license: Copyright The Samraksh Company, 2014
 * @date:    April 2, 2014
 */

#include <Samraksh\ID_decl.h>
/**
 * @details: Design Goal: fetch unique IDs stored in the QFPROM and share them
 *  via the config region as the OEM serial number.
 */

unsigned int ID_GetSerialNumbers(struct OEM_SERIAL_NUMBERS& OemSerialNumbers)
{
    //TODO: read QFPROM.
#warning "ID driver not implemented"
    return 0;
}

unsigned short ID_GetMF_NODE_ID() {
    //TODO: read QFPROM.
#warning "ID driver not implemented"
    return 0;
}
