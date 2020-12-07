#define TPC3_INODE_H

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef TPC3_BYTEMAP_H
#include "tpc3_bytemap.h"
#endif



#define INODE_OFFSET		1  // Disk block where inode table starts

// Note that inodes currently fit exactly in one block
#define INODES_PER_BLOCK	(DISK_BLOCK_SIZE / sizeof(struct inode))
#define POINTERS_PER_INODE	6

struct inode {
  unsigned int isvalid;
  unsigned int size;
  unsigned int direct[POINTERS_PER_INODE];
};

union inode_block {
  struct inode ino[INODES_PER_BLOCK];
  unsigned char data[DISK_BLOCK_SIZE];
};


/* operations on inode structures

  allocate: reads the block containing the inode, locates the inode,
	    sets isvalid to 1, writes the disk block
    parameters:
     @in: (absolute) inode number
    errors:
     those resulting from disk operations

  deallocate: reads the block containing the inode, locates the inode,
	    sets isvalid to 0, writes the disk block
    parameters:
     @in: (absolute) inode number
    errors:
     those resulting from disk operations

*/

/* Helper function prototypes

  inode_table_print: prints inode information
    parameters:
     @in: (absolute) inode number

*/

int inode_table_print(unsigned int ninodes);


struct inode_operations {
  int (*allocate)(unsigned int absnumber);
  int (*deallocate)(unsigned int absnumber);
};

