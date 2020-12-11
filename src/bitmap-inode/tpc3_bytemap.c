#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef TPC3_BYTEMAP_H
#include "tpc3_bytemap.h"
#endif

/* Global variables */
unsigned int max_bm_entries = -1;  // stores the number of entries for the
				   // current test session (commands T and S)
				   // if this was a filesystem, it would be
				   // stored in the superblock

/* Helper functions */

int bytemap_print_table(unsigned int max_entries) {
  int ercode;
  unsigned char bmap[DISK_BLOCK_SIZE];
  int left = max_entries, entry = 0;

  // read in the bytemap
	ercode = disk_ops.read(BYTEMAP_OFFSET, bmap);
  if (ercode < 0) return ercode;

  // **TODO** DO NOT FORGET TO COMMENT THE NEXT LINE when submitting to Mooshak
  // printf("Printing the bytemap ----------\n");
  // prints 16 entries per line
  while (left) {
    if ((entry + 1) % 16)
      printf("%u ", bmap[entry]);
    else
      printf("%u\n", bmap[entry]);
    left--;
    entry++;
  }
  if (entry % 16) printf("\n");	 // last NL for general case

  return 0;
}

/* bytemap operations */

static int bytemap_allocate(unsigned int entry) {
  int ercode;
  unsigned char bmap[DISK_BLOCK_SIZE];

  if (entry >= max_bm_entries) return -EFBIG;

  // read in the bytemap
	ercode = disk_ops.read(BYTEMAP_OFFSET, bmap);
	if (ercode < 0) return ercode;

  if (bmap[entry] == 1) // bytemap entry already allocated
    return -EBUSY;
  else
    bmap[entry] = 1;

  // update (i.e., write) the bytemap
  ercode = disk_ops.write(BYTEMAP_OFFSET, bmap);
	if (ercode < 0) return ercode;

  return entry;
}

static int bytemap_deallocate(unsigned int entry) {
  int ercode;
  unsigned char bmap[DISK_BLOCK_SIZE];

  if (entry >= max_bm_entries) return -EFBIG;

  // read in the bytemap
  ercode = disk_ops.read(BYTEMAP_OFFSET, bmap);
	if (ercode < 0) return ercode;

  if (bmap[entry] == 0)
    return -EINVAL;
  else
    bmap[entry] = 0;

  // update (i.e., write) the bytemap
	ercode = disk_ops.write(BYTEMAP_OFFSET, bmap);
	if (ercode < 0) return ercode;
	
  return entry;
}

static int bytemap_getfree() {
  int ercode;
  int entriesLeft = max_bm_entries;
  unsigned char bmap[DISK_BLOCK_SIZE];

  // read in the bytemap
  ercode = disk_ops.read(BYTEMAP_OFFSET, bmap);
	if (ercode < 0) return ercode;

  // Scan the bytemap, look for the first free entry
  // TODO: if there's an error check here
	int j = 0;
	while (entriesLeft != 0) {
		if (bmap[j] == 0) return j;
		j++;
		entriesLeft--;
	}

  return -ENOSPC;
}

struct bytemap_operations bmap_ops = {.allocate = bytemap_allocate,
				      .deallocate = bytemap_deallocate,
				      .getfree = bytemap_getfree};
