#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef TPC3_INODE_H
#include "tpc3_inode.h"
#endif

/* inode (global) number is decomposed into inode block number
   and offset within that block. The inode block number starts at 0 */

static void inode_abs2bo(unsigned int numinode, unsigned int *numblock,
			 unsigned int *offset) {
  *numblock = numinode / INODES_PER_BLOCK;
  *offset = numinode % INODES_PER_BLOCK;
}

int inode_table_print(unsigned int ninodes) {
  unsigned int ninodeblocks = ceil((float)ninodes / INODES_PER_BLOCK);
  int ercode;
  union inode_block in_b;
  int left = ninodes;
  int entry;

  // **TODO** DO NOT FORGET TO COMMENT THE NEXT LINE when submitting to Mooshak
  printf("Printing the inodes -----------\n");

  for (int inoblk = 0; inoblk < ninodeblocks; inoblk++) {
    // Get a full inode block (REMEMBER there is an OFFSET!)
    ercode = disk_ops.read(INODE_OFFSET + inoblk, in_b.data);
    if (ercode < 0) return ercode;

    // **TODO** DO NOT FORGET TO COMMENT THE NEXT LINE when submitting to
    // Mooshak
    printf("Inode Block: %d\n", inoblk);
    // Print each inode in block
    entry = 0;

    while ((left) && (entry < INODES_PER_BLOCK)) {
      if ((entry + 1) % INODES_PER_BLOCK)
	printf("%u ", in_b.ino[entry].isvalid);
      else
	printf("%u\n", in_b.ino[entry].isvalid);
      left--;
      entry++;
    }
    if (entry % INODES_PER_BLOCK) printf("\n");	 // last NL for general case
  }

  return 0;
}

static int inode_allocate(unsigned int absinode) {
  int ercode;
  unsigned int block, offset;
  union inode_block i_b;

  inode_abs2bo(absinode, &block, &offset);
  // read inode block from disk
  ercode = disk_ops.read(INODE_OFFSET + block, i_b.data);
	if (ercode < 0) return ercode;

  // in the block, change the target inode
  i_b.ino[offset].isvalid = 1;

  // write inode block to disk
	disk_ops.write(INODE_OFFSET + block, i_b.data);
  if (ercode < 0) return ercode;

  return 0;
}

static int inode_deallocate(unsigned int absinode) {
  int ercode;
  unsigned int block, offset;
  union inode_block i_b;

  inode_abs2bo(absinode, &block, &offset);
  // read inode block from disk
 	ercode = disk_ops.read(INODE_OFFSET + block, i_b.data);
	if (ercode < 0) return ercode;

  // in the block, clear ALL fields of the target inode
	// clear the direct field
	for (int i = 0; i <= i_b.ino[offset].size ; i++) {
		i_b.ino[offset].direct[i] = 0;
	}

	i_b.ino[offset].isvalid = 0;
	i_b.ino[offset].size = 0;

  // write inode block to disk
  disk_ops.write(INODE_OFFSET + block, i_b.data);
	if (ercode < 0) return ercode;

  return 0;
}

struct inode_operations inode_ops = {.allocate = inode_allocate,
				     .deallocate = inode_deallocate};
