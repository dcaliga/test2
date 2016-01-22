/* $Id: ex02.mc,v 2.1 2005/06/14 22:16:46 jls Exp $ */

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>


void subr (int64_t I0[], int64_t I1[], int64_t Out[], int num,
	int64_t *down_time, int64_t *compute_time, int64_t *up_time, int mapnum) {

    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_B (BL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_C (CL, int64_t, MAX_OBM_SIZE)
    int64_t t0, t1, t2, t3, v;
    int i;

    Stream_64 SA,SB,SC;

    read_timer (&t0);

    buffered_dma_cpu (CM2OBM, PATH_0, AL, MAP_OBM_stripe (1,"A"), I0, 1, num*8);

    read_timer (&t1);

#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SB, PORT_TO_STREAM, I1, num*sizeof(int64_t));
}
#pragma src section
{
    int i;
    int64_t bv,i64;


    for (i=0; i<num; i++) {
      get_stream_64 (&SB, &bv);

      v = AL[i] + bv;

      if (AL[i] > bv)
	    v = v + 42;

	  i64 = v * 3;
      put_stream_64 (&SC, i64, 1);
	}

    read_timer (&t2);
}
#pragma src section
{
    streamed_dma_cpu_64 (&SC, STREAM_TO_PORT, Out, num*sizeof(int64_t));
}
}

    read_timer (&t3);

    *down_time = t1 - t0;
    *compute_time = t2 - t1;
    *up_time = t2 - t1;
}
