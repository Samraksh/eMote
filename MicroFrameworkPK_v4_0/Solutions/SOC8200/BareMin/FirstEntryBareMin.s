/* 
 * FirstEntry.s startup code for the arm cortex a8 sitara 
 * 
 * This file is modelled from uboots startup source start.s 
 *
 */


.global EntryPoint
.extern myEntry

EntryPoint:
	B myEntry
