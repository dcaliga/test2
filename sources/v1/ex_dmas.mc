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


    read_timer (&t0);

    buffered_dma_cpu (CM2OBM, PATH_0, AL, MAP_OBM_stripe (1,"A"), I0, 1, num*8);
    buffered_dma_cpu (CM2OBM, PATH_0, BL, MAP_OBM_stripe (1,"B"), I1, 1, num*8);

    read_timer (&t1);

    for (i=0; i<num; i++) {
	v = AL[i] + BL[i];
	if (AL[i] > BL[i])
	    v = v + 42;
	CL[i] = v * 3;
	}

    read_timer (&t2);

    buffered_dma_cpu (OBM2CM, PATH_0, CL, MAP_OBM_stripe (1,"C"), Out, 1, num*8);

    read_timer (&t3);

    *down_time = t1 - t0;
    *compute_time = t2 - t1;
    *up_time = t3 - t2;
}
