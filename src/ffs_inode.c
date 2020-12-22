#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef FFS_INODE_H
#include "ffs_inode.h"
#endif

#ifndef FFS_BYTEMAP_H
#include "ffs_bytemap.h"
extern struct bytemap_operations bmap_ops;
#endif

static void inode_print(unsigned int number, struct inode *in) {
  printf("%d:\n", number);
  printf("	%s\n", (in->isvalid) ? "valid" : "invalid");
  if (in->isvalid) {
    // print the i-node information for one i-node
    printf("\t%d\n", in->size);

    int size = in->size;
    int block_count = in->size / DISK_BLOCK_SIZE;
    int occupied_pointers =
	size % DISK_BLOCK_SIZE ? block_count + 1 : block_count;

    for (int i = 0; i < POINTERS_PER_INODE; i++) {
      if (i < occupied_pointers) {
	printf("\t\t%d\n", in->direct[i]);
      } else {
	printf("\t\t%s\n", "NULL");
      }
    }
  }
}

static int inode_printTable(unsigned int ninodeblocks, unsigned int ninodes,
			    unsigned int inodesStartBlock) {
  int ercode;
  union in_block in_b;
  int left = ninodes;
  // more variables...

  printf("i-nodes:\n");

  int j = 0;

  for (int inoblk = 0; inoblk < ninodeblocks; inoblk++) {
    // the table starts at inodesStartBlock:
    // Get a full inode block;
    ercode = disk_ops.read(inodesStartBlock + inoblk, in_b.data);
    if (ercode < 0) return ercode;

    // Print each inode in block
    for (int i = 0; (i < INODES_PER_BLOCK) && (left); i++) {
      inode_print(j, &in_b.ino[i % INODES_PER_BLOCK]);
      j++;
      left--;
    }
  }

  return 0;
}

/* inode (global) number is decomposed into inode block number
   and offset within that block. The inode block number starts at 0 */

static void inode_location(unsigned int numinode, unsigned int *numblock,
			   unsigned int *offset) {
  *numblock = numinode / INODES_PER_BLOCK;
  *offset = numinode % INODES_PER_BLOCK;
}

// read an i-node from disk
static int inode_read(unsigned int startInArea, unsigned int absinode,
		      struct inode *in) {
  int ercode;
  unsigned int block, offset;
  union in_block in_b;

  inode_location(absinode, &block, &offset);
  // read the inode block

  ercode = disk_ops.read(startInArea + block, in_b.data);
  if (ercode < 0) return ercode;

  // extract the inode information from the block into inode *in
  in->isvalid = in_b.ino[offset].isvalid;

  for (int i = 0; i < POINTERS_PER_INODE; i++) {
    in->direct[i] = in_b.ino[offset].direct[i];
  }

  in->size = in_b.ino[offset].size;

  return 0;
}

void f_data_print(unsigned char *buf, int toPrint) {
  int left = toPrint, entry = 0;

  // prints 16 entries per line
  while (left) {
    if ((entry + 1) % 16)
      printf("%c ", buf[entry]);
    else
      printf("%c\n", buf[entry]);
    left--;
    entry++;
  }
  if (entry % 16) printf("\n");	 // last NL for general case
}

static int inode_printFileData(unsigned int startInArea, unsigned int absinode,
			       unsigned int startDtArea) {
  int ercode, size;
  unsigned int block, offset;
  union in_block in_b;
  unsigned char buf[DISK_BLOCK_SIZE];

  inode_location(absinode, &block, &offset);
  // read the data block containing the inode number absinode
  ercode = disk_ops.read(startInArea + block, in_b.data);
  if (ercode < 0) return ercode;

  if (!in_b.ino[offset].isvalid) return 0;

  printf("\nPrinting contents of file(inode) %d\n", absinode);

  size = in_b.ino[offset].size;

  if (!size) {
    printf("** NO DATA **\n");
    return 0;
  }

  int i = 0;
  while (size > 0) {
    ercode = disk_ops.read(startDtArea + in_b.ino[offset].direct[i++], buf);

    if (ercode < 0) {
      return ercode;
    }

    if (size >= DISK_BLOCK_SIZE) {
      f_data_print(buf, DISK_BLOCK_SIZE);
    } else {
      f_data_print(buf, size);
    }

    size -= DISK_BLOCK_SIZE;
  }

  return ercode;
}

void inode_checkData(int ninodes, int startInArea, struct bytemap *data) {
  struct bytemap bmap;
  struct inode ino;
  unsigned int nduplicates = 0;

	printf("--------------------------------------------------------------------------------\n");
  printf(
      "Integrity checking for data, trying to find duplicate pointers within "
      "inodes ...\n");
	printf("--------------------------------------------------------------------------------\n");

  for (int i = 0; i < DISK_BLOCK_SIZE; i++) {
    bmap.bmap[i] = 0;
  }

  for (int i = 0; i < ninodes; i++) {
    inode_read(startInArea, i, &ino);

    int size = ino.size;
    int block_count = ino.size / DISK_BLOCK_SIZE;
    int occupied_pointers =
	size % DISK_BLOCK_SIZE ? block_count + 1 : block_count;

    for (int j = 0; j < occupied_pointers; j++) {
      int k = ino.direct[j];

      if (bmap.bmap[k] == 1) {
	nduplicates++;
      } else {
	bmap.bmap[k] = 1;
      }
    }
  }

  printf("Number of inconsistencies found: %u\n\n", nduplicates);
	
	printf("---------------------------------------------------------------------------------------\n");
  printf(
      "Integrity check for data bmap, trying to find differences between the "
      "generated one ...\n");
	printf("---------------------------------------------------------------------------------------\n");

	int ninconsistencies = 0;

  for (int i = 0; i < data->size; i++) {
    if (bmap.bmap[i] != data->bmap[i]) {
      ninconsistencies++;
    }
  }

  printf("Number of differences found: %u\n\n", ninconsistencies);
}

struct inode_operations inode_ops = {.read = inode_read,
				     .printFileData = inode_printFileData,
				     .printTable = inode_printTable,
				     .checkData = inode_checkData};
