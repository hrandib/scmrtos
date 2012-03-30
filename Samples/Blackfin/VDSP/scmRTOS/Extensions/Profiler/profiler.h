//******************************************************************************
//*
//*     FULLNAME:  Single-Chip Microcontroller Real-Time Operating System
//*
//*     NICKNAME:  scmRTOS
//*
//*     PURPOSE:  Process Profiler definitions
//*
//*     Version: 4.00a
//*
//*     $Revision: 352 $
//*     $Date:: 2011-02-24 #$
//*
//*     Copyright (c) 2003-2011, Harry E. Zhurov
//*
//*     Permission is hereby granted, free of charge, to any person
//*     obtaining  a copy of this software and associated documentation
//*     files (the "Software"), to deal in the Software without restriction,
//*     including without limitation the rights to use, copy, modify, merge,
//*     publish, distribute, sublicense, and/or sell copies of the Software,
//*     and to permit persons to whom the Software is furnished to do so,
//*     subject to the following conditions:
//*
//*     The above copyright notice and this permission notice shall be included
//*     in all copies or substantial portions of the Software.
//*
//*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//*     EXPRESS  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//*     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//*     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//*     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//*     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
//*     THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*
//*     =================================================================
//*     See http://scmrtos.sourceforge.net for documentation, latest
//*     information, license and contact details.
//*     =================================================================
//*
//*****************************************************************************


#ifndef PROFILER_H
#define PROFILER_H

#include <scmRTOS.h>

class TProfiler : public OS::TKernelAgent
{
    uint32_t time_interval();
protected:
    INLINE TProfiler();
public:

    INLINE void advance_counters()
    {
        uint32_t Elapsed = time_interval();
        Sum += Elapsed;
        Counter[ cur_proc_priority() ] += Elapsed;
    }

    INLINE uint16_t get_result(uint_fast8_t index) { return Result[index]; }
    INLINE void     process_data();

protected:
    volatile uint32_t  Sum;
    volatile uint32_t  Counter[OS::PROCESS_COUNT];
             uint16_t  Result [OS::PROCESS_COUNT];
};

// ----------------------------------------------------
TProfiler::TProfiler()
    : Sum     ( 0 )
    , Counter (   )
    , Result  (   )
{
}

void TProfiler::process_data()
{
    // Use cache to make critical section fast as possible
    uint32_t CounterCache[OS::PROCESS_COUNT];
    uint32_t SumCache;

    {
        TCritSect cs;
        for(uint_fast8_t i = 0; i < OS::PROCESS_COUNT; ++i)
        {
            CounterCache[i] = Counter[i];
            Counter[i]       = 0;
        }
        SumCache = Sum;
        Sum       = 0;
    }

    const uint32_t K = 10000;
    for(uint_fast8_t i = 0; i < OS::PROCESS_COUNT; ++i)
    {
        Result[i]  = CounterCache[i] * K / SumCache;
    }
}

#endif  // PROFILER_H
