/*
 * Copyright (c) 2016 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <rtems/score/armv7m.h>
#include <rtems/counter.h>
#include <rtems/sysinit.h>

#include <bsp.h>
#include <bsp/fatal.h>

uint32_t _CPU_Counter_frequency(void)
{
#ifdef BSP_ARMV7M_SYSTICK_FREQUENCY
  return = BSP_ARMV7M_SYSTICK_FREQUENCY;
#else
  volatile ARMV7M_Systick *systick = _ARMV7M_Systick;
  return ARMV7M_SYSTICK_CALIB_TENMS_GET(systick->calib) * 100;
#endif
}

CPU_Counter_ticks _CPU_Counter_read(void)
{
  volatile ARMV7M_DWT *dwt = _ARMV7M_DWT;

  return dwt->cyccnt;
}

static void armv7m_cpu_counter_initialize(void)
{
  bool cyccnt_enabled;

  cyccnt_enabled = _ARMV7M_DWT_Enable_CYCCNT();

  if (!cyccnt_enabled) {
    bsp_fatal(BSP_ARM_ARMV7M_CPU_COUNTER_INIT);
  }
}

RTEMS_SYSINIT_ITEM(
  armv7m_cpu_counter_initialize,
  RTEMS_SYSINIT_CPU_COUNTER,
  RTEMS_SYSINIT_ORDER_FIRST
);
