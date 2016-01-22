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
           int64_t *compute_time, 
           int64_t *up_time, 
           int64_t *total_time, 
           int mapnum);


int main (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i, num, numl;
    int64_t *A, *B, *C;
    int64_t tm0a, tm0b, tm1, tm2, tm3;
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


       num = 128000;
       A = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
       B = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
       C = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));

    srandom (99);

    for (i=0; i<num; i++) {
        A[i] = random ();
        B[i] = random ();
	}

    map_allocate (1);

  numl = 512;
  for (i=0;i<4;i++)  {

  printf ("\n******* num %i ********\n",numl);
    subr (A, B, C, numl, &tm1, &tm2, &tm3, mapnum);

    printf ("   compute:          %8lld clocks\n", tm1);
    printf ("   DMA to CPU:       %8lld clocks\n", tm2);
    printf ("   Total time:       %8lld clocks\n", tm3);

    numl = numl*2;
  }
    for (i=0; i<numl/2; i++) {
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

