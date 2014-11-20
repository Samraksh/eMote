/**
 * @file   static_init.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Sep 12, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief  take care of C++ global static initializations on bare metal.
 */

#ifndef STATIC_INIT_H_
#define STATIC_INIT_H_

#if !defined(COMPILE_WIN)
#include <tinyhal.h>
#else
#if defined(DEBUG)
#define HARD_BREAKPOINT()  (while(1))
#else
#define HARD_BREAKPOINT()  
#endif

extern "C"
{
    extern void (**__init_array_start)();
    extern void (**__init_array_end)();

    inline void static_init()
    {
        for (void (**p)() = __init_array_start; p < __init_array_end; ++p)
            (*p)();
    }

    void __cxa_pure_virtual() { HARD_BREAKPOINT(); }

}



#endif /* STATIC_INIT_H_ */
