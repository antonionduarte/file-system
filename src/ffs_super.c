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
  memcpy(sb_u.data, sb, sizeof(sb_u.sb));

  return 0;
}

/* Helper functions */

void super_print(const struct super *sb) {
  printf("Superblock:\n");
  printf("fsmagic\t0x%08x", sb->fsmagic);
  printf("nblocks:\t%u", sb->nblocks);
  printf("nbmapblocksinodes:\t%u", sb->nbmapblocksinodes);
  printf("ninodeblocks:\t%u", sb->ninodeblocks);
  printf("ninodes:\t%u", sb->ninodes);
  printf("nbmapblocksdata:\t%u", sb->nbmapblocksdata);
  printf("ndatablocks:\t%u", sb->ndatablocks);
  printf("startInArea:\t%u", sb->startInArea);
  printf("startDtBmap:\t%u", sb->startDtBmap);
  printf("startDtArea:\t%u", sb->startDtArea);
  printf("mounted:\t%u", sb->mounted);
}

void super_debug(const struct super *sb) {
  printf("Superblock:\n");
  // May be printing the names when developing the application?
}

struct super_operations super_ops = {
    .read = &super_read, .print = &super_print, .debug = &super_debug};
