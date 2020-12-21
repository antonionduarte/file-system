#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef FFS_SUPER_H
#include "ffs_super.h"
#endif

#include "ffs_inode.h"

/* #ifndef FFS_INODE_H
#include "ffs_inode.h"
#endif */

static int super_read(struct super *sb) {
  int ercode;
  union sb_block sb_u;

  // read the disk block that contains the superblock
  ercode = disk_ops.read(SB_OFFSET, sb_u.data);
  if (ercode < 0) return ercode;  // an advice: always test the return codes...

  // copy the contents (without the extra garbage :-) to sb
  // memcpy(sb_u.data, sb, sizeof(sb_u.sb));
  *sb = sb_u.sb;

  return 0;
}

static void check_integrity(struct super *sb) {
  int driver_size = disk_ops.stat();
  int sb_size = sb->nblocks;
  int sb_sum_blocks_size = 2 + sb->nbmapblocksdata + sb->nbmapblocksinodes +
			   sb->ndatablocks + sb->ninodeblocks;

  printf("Superblock integrity check:\n");

  // check magic number
  printf("Magic number: ");
  if (sb->fsmagic == FS_MAGIC) {
    printf(CORRECT);
  } else {
  	printf(INCORRECT);
	}

  // check disk size
  printf("Total reported size: ");
  if (sb_size == driver_size) {
    printf(CORRECT);
  } else {
    printf(INCORRECT);
  }

  // check sum of block sizes in relation to superblock
  printf("Sum of blocks (in relation to superblock): ");
  if (sb_size == sb_sum_blocks_size) {
    printf(CORRECT);
  } else {
    printf(INCORRECT);
  }
	
	// check sum of block sizes in relation to driver
	printf("Sum of blocks (in relation to driver): ");
	if (driver_size == sb_sum_blocks_size) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

	// starting area checks
	printf("I-Node starting area: ");
	if (sb->startInArea == 2) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

	printf("Data blocks starting area: ");
	if (sb->startDtArea == (2 + sb->ninodeblocks + 2)) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

	// bmap locations
	printf("I-Node bitmap location: ");
	if (sb->nbmapblocksinodes == 1) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

	printf("Data blocks bitmap location: ");
	if (sb->nbmapblocksdata == 3 + sb->ninodeblocks) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

	// number of inodes
	printf("Number of I-Nodes: ");
	if (sb->ninodes == (INODES_PER_BLOCK * sb->ninodeblocks)) {
		printf(CORRECT);
	}
	else {
		printf(INCORRECT);
	}

}

/* Helper functions */

void super_print(const struct super *sb) {
  printf("Superblock:\n");
  printf("%s\n", (sb->fsmagic == FS_MAGIC) ? "valid" : "invalid");
  printf("%u\n", sb->nblocks);
  printf("%u\n", sb->nbmapblocksinodes);
  printf("%u\n", sb->ninodeblocks);
  printf("%u\n", sb->ninodes);
  printf("%u\n", sb->nbmapblocksdata);
  printf("%u\n", sb->ndatablocks);
  printf("%u\n", sb->startInArea);
  printf("%u\n", sb->startDtBmap);
  printf("%u\n", sb->startDtArea);
  printf("%s\n", (sb->mounted == 0) ? "no" : "yes");
}

struct super_operations super_ops = {
    .read = &super_read, .print = &super_print, .integrity = &check_integrity};
