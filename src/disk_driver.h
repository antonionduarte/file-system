#define DISK_DRIVER_H

#define DISK_BLOCK_SIZE		512

#ifndef _FFS_ERRNO_H
#include "ffs_errno.h"
#endif

union u_block {
  /* insert here other structures that must be read/writen as disk blocks;
     they probably are defined in separate #includes that should be inserted
     above... */

  unsigned char data[DISK_BLOCK_SIZE];
};


/*
  There are no functions to be exported in this module other than
  those visible in the  disk operations structure. KEEP IT that way :-)
*/

struct disk_operations {
  int (*open)(const char *filename, unsigned int n);
  int (*stat)();
  int (*read)(unsigned int blknmbr, unsigned char *buf);
  int (*write)(unsigned int blknmbr, const unsigned char *buf);
  int (*close)();
};


/* int open(const char *filename, unsigned int n)
  open: string filename, unsigned int number of blocks
    if filename exists and n==0 populate the global nblocks var
    if filename exists and n>0 recreate it with size n blocks
    if filename does not exist, create it with size n blocks
    blocks are zeroed;
  errors:
    -EBUSY      A disk is already open
    -ENOENT     Recreate asked but filename does not exist
    other errors returned as -1
*/


/* int stat()
  stat:
    returns disk size (number of disk blocks)
  errors:
    -ENODEV     disk not open
*/


/* int read(unsigned int blknmbr, unsigned char *buf)
  read: read block at offset blknmbr, return data in buf
  errors:
    -ENODEV     disk not open
    -ENOSPC     disk has not that many blocks
    other errors returned as -1
*/


/* int write (unsigned int blknmbr, const unsigned char *buf)
  write: write block at offset blknmbr, get data from buf
  errors:
    -ENODEV     disk not open
    -ENOSPC     disk has not that many blocks
    other errors returned as -1
*/


/* int close()
  close: close an open disk
  errors:
    -ENODEV     disk not open
    other errors returned as -1
*/
