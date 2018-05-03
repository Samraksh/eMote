#-------------------------------------------------------------------------------
# (c) Copyright 2007-2013 Microsemi SoC Products Group. All rights reserved.
# 
# Interrupt disabling/restoration for critical section protection.
#
# SVN $Revision: 6667 $
# SVN $Date: 2014-07-04 11:48:08 +0100 (Fri, 04 Jul 2014) $
#
.section .text
.globl HAL_get_interrupts
.globl HAL_disable_interrupts
.globl HAL_restore_interrupts
#.code 16
#.syntax unified
.type HAL_get_interrupts, function
.type HAL_disable_interrupts, function
.type HAL_restore_interrupts, function
#-------------------------------------------------------------------------------
# 
#
HAL_get_interrupts:
    mrs r0, PRIMASK

#-------------------------------------------------------------------------------
#
#
HAL_disable_interrupts:    
    mrs r0, PRIMASK
    cpsid I
    bx lr

#-------------------------------------------------------------------------------
#
#
HAL_restore_interrupts:    
    msr PRIMASK, r0
    bx lr

.end
