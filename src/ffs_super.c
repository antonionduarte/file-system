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

void super_debug(const struct super *sb) {
  printf("Superblock:\n");
  // May be printing the names when developing the application?
}

struct super_operations super_ops = {
    .read = &super_read, .print = &super_print, .debug = &super_debug};
