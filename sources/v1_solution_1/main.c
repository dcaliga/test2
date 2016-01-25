static char const cvsid[] = "$Id: main.c,v 2.2 2005/07/08 18:02:31 jls Exp $";

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
#include <stdlib.h>


void subr (int64_t I0[], int64_t I1[], int64_t Out[], int num,
	   int64_t *down_time, int64_t *compute_time, int64_t *up_time, 
           int mapnum);
// this is my modified code

int cmain (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i, num;
    int64_t *A, *B, *C;
    int64_t tm0, tm1, tm2;
    int mapnum = 0;
    int snapmem = 0;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    if (argc < 2) {
	fprintf (stderr, "need number of elements as arg\n");
	exit (1);
	}

    if (sscanf (argv[1], "%d", &num) < 1) {
	fprintf (stderr, "need number of elements as arg\n");
	exit (1);
	}

    if ((num < 1) || (num > MAX_OBM_SIZE)) {
        fprintf (stderr, "number of elements must be in the range 1 through "
                 "%d\n", MAX_OBM_SIZE);
	exit (1);
	}

    if (argc ==  3) {
      sscanf (argv[2], "%d", &snapmem);
      printf ("snapmem %i\n",snapmem);
   }
        

       A = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
       B = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
       C = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));

    memset (A, 0, num * sizeof (int64_t));
    memset (B, 0, num * sizeof (int64_t));
    memset (C, 0, num * sizeof (int64_t));

    srandom (99);

    for (i=0; i<num; i++) {
        A[i] = random ();
        B[i] = random ();
	}

    map_allocate (1);

    subr (A, B, C, num, &tm0, &tm1, &tm2, mapnum);

    printf ("DMA to MAP: %8lld clocks\n", tm0);
    printf ("compute:    %8lld clocks\n", tm1);
    printf ("DMA to CPU: %8lld clocks\n", tm2);

    for (i=0; i<num; i++) {
	int64_t v;

        fprintf (res_map, "%lld\n", C[i]);

        v = A[i] + B[i];
        if (A[i] > B[i])
            v = v + 42;
        fprintf (res_cpu, "%lld\n", v*3);
	}

    map_free (1);

    exit(0);
    }

