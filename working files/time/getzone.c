/*******************
 *
 * Copyright 2003-2017 IAR Systems AB.
 *
 * This is the default implementation of the __getzone. It returns
 * the current time-zone, i.e. UTC. Whenever the call to setlocale
 * with _LC_TIME doesn't explicitly set timezone or daylight-savings time
 * __getzone gets called to get the default timezone and daylight-savings
 * time.
 *
 *
 *
 *
 * The return value should be a string on the following form:
 *
 * :[XXX[:YYY[:NNN[:DST[:DST ...]]]]]
 *
 * Where XXX is the standard time-zone name, YYY is the daylight
 * savings time-zone name, NNN is the time zone offset, and the DSTs
 * are the daylight savings time rules. Daylight savings time will add
 * one hour to the normal time. (The names are only used in the 'Z'
 * formatter in the strftime library function.)
 *
 * The time zone offset NNN is specified as a number relative to UTC,
 * possibly negative (east is positive), on the format HHMM, where HH
 * is hours and MM is minutes.
 *
 * The DSTs specifes a set of rules for how daylight savings time is
 * applied. The rules must be sorted in increasing date order starting
 * from the earliest date. The first rule for a specific year will
 * enable DST, the next will disable it, and so on. Each rule is on
 * the following form:
 *
 *   [(YYYY)]MMDD[HH][-W|+W]
 *
 *    (YYYY) is the first year the daylight savings rule was applied.
 *       It is optional. If not specified it will default to the same
 *       year as the previous rule or zero if no previous rule.
 *    MM is the month number (1-12).
 *    DD is the day of the month (1-31).
 *    HH is the hour number in a 24-hour day (optional, defaults to
 *       0).
 *    +/-W specifies the day of the week the rule takes effect (where
 *       Sunday = 0, Monday = 1, etc). +W means that the rule applies
 *       to the first such day on or after the specified date and -W
 *       strictly before the date. If this is not specified, the rule
 *       will take effect on the exact date, regardless of the day of
 *       the week.
 *
 * On the northern hemisphere the DST rules normally comes in pairs, a
 * start, Aprilish, and an end, Octoberish. On the southern hemisphere
 * one normally has to use three rules: enabling DST from start of
 * year, disabling it in Aprilish, and then enabling it again in
 * Octoberish.
 *
 * Example:
 *
 *   :GMT:GMT+1:0060:(1990)040102-0:100102-0
 *
 *   Here, the time zone is GMT and under daylight savings time the
 *   time zone is named GMT+1. The time zone offset is 0060, i.e. 60
 *   minutes from UTC. As of the year 1990, daylight savings time
 *   started on the Sunday before (but not on) 1:st of April at 2am
 *   and ends on the first Sunday before (but not on) the first of
 *   October.
 *
 *   :GMT+10:GMT+11:1000:(1990)010100-0:040102-0:100102-0
 *
 *   Tasmania is on UTC+10 hours, with daylight savings time from
 *   first Sunday in October until first Sunday in April. Note, the
 *   first DST rule is for enabling from start of the year.
 *
 * If __getzone is overrided then _DstMalloc and _Dstfree must be overrided as
 * well. To make it work. _DstMalloc should return a buffer that can hold
 * maximum number of dates for a dst rule. Each date takes up a structure of
 * 5 bytes plus the normal alignment for it. Or it could have a call to malloc.
 *
 ********************/

#include <LowLevelIOInterface.h>
#include "header.h"

char const * __getzone()
{
  int time_zone = current_settings.time_zone;
  
  size_t i = 0;
  char *p_getzone_string = getzone_string[bank_getzone];
  bank_getzone = (bank_getzone + 1) & 0x1;

  p_getzone_string[i++] = ':';
  
  if (!lt_or_utc)
  {
    p_getzone_string[i++] = 'G';
    p_getzone_string[i++] = 'M';
    p_getzone_string[i++] = 'T';
    p_getzone_string[i++] = (time_zone < 0 ) ? '-' : '+';
    int abs_time_zone = abs(time_zone);
    if (abs_time_zone <= 9) p_getzone_string[i++] = abs_time_zone + 0x30;
    else
    {
      p_getzone_string[i++] = abs_time_zone/10 + 0x30;
      p_getzone_string[i++] = abs_time_zone%10 + 0x30;
    }

    p_getzone_string[i++] = ':';
  
    p_getzone_string[i++] = 'G';
    p_getzone_string[i++] = 'M';
    p_getzone_string[i++] = 'T';
    p_getzone_string[i++] = ((time_zone + 1) < 0 ) ? '-' : '+';
    abs_time_zone = abs(time_zone + 1);
    if (abs_time_zone <= 9) p_getzone_string[i++] = abs_time_zone + 0x30;
    else
    {
      p_getzone_string[i++] = abs_time_zone/10 + 0x30;
      p_getzone_string[i++] = abs_time_zone%10 + 0x30;
    }

    p_getzone_string[i++] = ':';
    abs_time_zone = abs(time_zone);
    if (time_zone < 0 ) p_getzone_string[i++] = '-';
    p_getzone_string[i++] = abs_time_zone/10 + 0x30;
    p_getzone_string[i++] = abs_time_zone%10 + 0x30;
    p_getzone_string[i++] = '0';
    p_getzone_string[i++] = '0';

    uint32_t dst_on_rule = current_settings.dst_on_rule;
    uint32_t dst_off_rule = current_settings.dst_off_rule;
    if (
        (current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) &&
        (dst_off_rule != dst_on_rule)  
       )   
    {
      p_getzone_string[i++] = ':';
  
//      char const dst_rul_ukr[] = "(1996)040103-0:110103-0";
//      char const dst_rul_ukr[] = "040103-0:110103-0";
//      for (size_t j = 0; dst_rul_ukr[j] != '\0'; ++j) p_getzone_string[i++] = dst_rul_ukr[j];
      
      uint32_t rule[2];
      if (dst_on_rule < dst_off_rule)
      {
        //Північна півкуля
        rule[0] = dst_on_rule;
        rule[1] = dst_off_rule;
      }
      else
      {
        //Південна півкуля
        static char const dst_rul_begin[] = "010100-0:";
        static char const *ptr_tmp = dst_rul_begin;
        for (; *ptr_tmp != '\0'; ) p_getzone_string[i++] = *ptr_tmp++;
        
        rule[0] = dst_off_rule;
        rule[1] = dst_on_rule;
      }

      for (size_t j = 0; j < 2; j++)
      {
        unsigned int rule_tmp = rule[j];
        
        unsigned int n = (rule_tmp >> POS_WR) & ((1 << SHIFT_WR) - 1);
        unsigned int val;
        if (n == 5)
        {
          //[(MM + 1) % 12]01HH-W
          val = ( ( (rule_tmp >> POS_MM) & ( (1 << SHIFT_MM) - 1 ) ) + 1 ) % 12;
          p_getzone_string[i++] = val/10 + 0x30;
          p_getzone_string[i++] = val%10 + 0x30;
          
          p_getzone_string[i++] = '0';
          p_getzone_string[i++] = '1';

          val = (rule_tmp >> POS_HH) & ((1 << SHIFT_HH) - 1);
          p_getzone_string[i++] = val/10 + 0x30;
          p_getzone_string[i++] = val%10 + 0x30;

          p_getzone_string[i++] = '-';
        }
        else if ((n >= 1) && (n <= 4))
        {
          //MM(1 + n*7)HH+W
          val = (rule_tmp >> POS_MM) & ( (1 << SHIFT_MM) - 1 );
          p_getzone_string[i++] = val/10 + 0x30;
          p_getzone_string[i++] = val%10 + 0x30;
          
          val = 1u + (n - 1)*7u;
          p_getzone_string[i++] = val/10 + 0x30;
          p_getzone_string[i++] = val%10 + 0x30;

          val = (rule_tmp >> POS_HH) & ((1 << SHIFT_HH) - 1);
          p_getzone_string[i++] = val/10 + 0x30;
          p_getzone_string[i++] = val%10 + 0x30;

          p_getzone_string[i++] = '+';
        }
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }

        val = (rule_tmp >> POS_DOW) & ((1 << SHIFT_DOW) - 1);
        p_getzone_string[i++] = val + 0x30;
        
        if (j == 0) p_getzone_string[i++] = ':';
    
      }
    }
  }

  p_getzone_string[i++] = '\0';
  return p_getzone_string;
}

__ATTRIBUTES char *_DstMalloc(size_t);
__ATTRIBUTES void _DstFree(char *);

char *_DstMalloc(size_t s)
{
//  return 0;
  #if 1
    (void)(s);
    static char buffert[55];
    return buffert;
  #endif
//  #if 0
//    return (char *)malloc(s);
//  #endif
}
void _DstFree(char *p)
{
  #if 1
    // Nothing here
    (void)(p);
  #endif
//  #if 0
//    free(p);
//  #endif
}
