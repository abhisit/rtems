/* 
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"
#include <sched.h>

void *POSIX_Init(
  void *argument
)
{
  int             status;
  int             priority;
  pthread_t       thread_id;
  time_t          seconds;
  time_t          seconds1;
  time_t          remaining;
  struct tm       tm;
  struct timespec tv;
  struct timespec tr;

  puts( "\n\n*** POSIX TEST 1 ***" );

  /* set the time of day, and print our buffer in multiple ways */

  build_time( &tm, TM_FRIDAY, TM_MAY, 24, 96, 11, 5, 0 );

  tv.tv_sec = mktime( &tm );
  assert( tv.tv_sec != -1 );

  tv.tv_nsec = 0;

  status = clock_settime( CLOCK_REALTIME, &tv );
  assert( !status );

  printf( asctime( &tm ) );
  printf( ctime( &tv.tv_sec ) );

  /* use sleep to delay */

  remaining = sleep( 3 );
  assert( !remaining );

  /* print new times to make sure it has changed and we can get the realtime */

  status = clock_gettime( CLOCK_REALTIME, &tv );
  assert( !status );

  printf( ctime( &tv.tv_sec ) );

  seconds = time( NULL );
  printf( ctime( &seconds ) );

  /*  just to have the value copied out through the parameter */
  
  seconds = time( &seconds1 );
  assert( seconds == seconds1 );

  /* check the time remaining */

  printf( "Init: seconds remaining (%d)\n", (int)remaining );
  assert( !remaining );

  /* use nanosleep to delay */

  tv.tv_sec = 3; 
  tv.tv_nsec = 500000; 

  status = nanosleep ( &tv, &tr );
  assert( !status );

  /* print the current real time again */

  status = clock_gettime( CLOCK_REALTIME, &tv );
  assert( !status );
 
  printf( ctime( &tv.tv_sec ) );

  /* check the time remaining */

  printf( "Init: sec (%ld), nsec (%ld) remaining\n", tr.tv_sec, tr.tv_nsec );
  assert( !tr.tv_sec && !tr.tv_nsec );

  /* get id of this thread */

  Init_id = pthread_self();
  printf( "Init: ID is 0x%08x\n", Init_id );

  /* exercise get minimum priority */

  priority = sched_get_priority_min( SCHED_FIFO );
  printf( "Init: sched_get_priority_min (SCHED_FIFO) -- %d\n", priority );
  assert( priority != -1 );

  puts( "Init: sched_get_priority_min -- EINVAL (invalid policy)" );
  priority = sched_get_priority_min( -1 );
  assert( priority == -1 );
  assert( errno == EINVAL );

  /* exercise get maximum priority */
 
  priority = sched_get_priority_max( SCHED_FIFO );
  printf( "Init: sched_get_priority_max (SCHED_FIFO) -- %d\n", priority );
  assert( priority != -1 );

  puts( "Init: sched_get_priority_min -- EINVAL (invalid policy)" );
  priority = sched_get_priority_min( -1 );
  assert( priority == -1 );
  assert( errno == EINVAL );

  /* print the round robin time quantum */
 
  status = sched_rr_get_interval( getpid(), &tr );
  printf(
    "Init: Round Robin quantum is %ld seconds, %ld nanoseconds\n",
    tr.tv_sec,
    tr.tv_nsec
  );
  assert( !status );
  
  /* create a thread */

  puts( "Init: pthread_create - SUCCESSFUL" );
  status = pthread_create( &thread_id, NULL, Task_1_through_3, NULL );
  assert( !status );

  /* too may threads error */

  puts( "Init: pthread_create - EINVAL (too many threads)" );
  status = pthread_create( &thread_id, NULL, Task_1_through_3, NULL );
  assert( status == EINVAL );

  puts( "Init: sched_yield to Task_1" );
  status = sched_yield();
  assert( !status );

    /* switch to Task_1 */

  /* exit this thread */

  puts( "Init: pthread_exit" );
  pthread_exit( NULL );

    /* switch to Task_1 */

  return NULL; /* just so the compiler thinks we returned something */
}
