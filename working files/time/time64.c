/*******************
 *
 * Copyright 1998-2017 IAR Systems AB.
 *
 * This is the default implementation of the "time" function of the
 * standard library.  It can be replaced with a system-specific
 * implementation.
 *
 * The "time" function returns the current calendar time.  (time_t)-1
 * should be returned if the calendar time is not available.  The time
 * is measured in seconds since the first of January 1970.
 *
 ********************/

#include <time64.h>
extern clock_t clk_count;
extern time_t time_dat_copy;
extern unsigned int copying_time_dat;

#pragma module_name = "?time"

#if _DLIB_TIME_ALLOW_64
  __time64_t __time64(__time64_t *t)
  {
  copying_time_dat = 1;
  __time64_t time_tmp = time_dat_copy;
  copying_time_dat = 0;
  
    if (t)
    {
      *t = time_tmp;
    }
    return time_tmp;
  }
#endif
