#include <stdio.h>
#include <string.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef FFS_SUPER_H
#include "ffs_super.h"
extern struct super_operations super_ops;
#endif

#ifndef FFS_BYTEMAP_H
#include "ffs_bytemap.h"
extern struct bytemap_operations bmap_ops;
#endif

#ifndef FFS_INODE_H
#include "ffs_inode.h"
extern struct inode_operations inode_ops;
#endif

int main(int argc, char *argv[]) {
	int ercode;

	struct super sb;
  struct bytemap bmapINODES, bmapDATA;
 
	// open the disk given in the arguments
	ercode = disk_ops.open(argv[1], 0);
  if (ercode < 0) return ercode;

	/* Superblock Error Checking */

	// save the superblock to the sb struct
  ercode = super_ops.read(&sb);
  if (ercode < 0) return ercode;

	// check integrity of superblock
	super_ops.integrity(&sb);

	/* Bytemap Integrity Checking */

	// inode bytemap integrity check
	bmap_ops.checkIntegrity(&bmapINODES, sb.startInArea);

	// data integrity checking
	ercode = bmap_ops.read(&bmapDATA, sb.ndatablocks, sb.startDtBmap);
	if (ercode < 0) return ercode;
	inode_ops.checkData(sb.ninodes, sb.startInArea, &bmapDATA);

}

