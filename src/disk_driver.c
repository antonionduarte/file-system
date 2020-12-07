#include <stdio.h>

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
#endif

static struct {
  int dfd; // only 1 disk at a time supported; if != -1, a disk is "open"
  unsigned int nblocks; // if != -1, number of blocks
} theDisk = {-1, -1};   // i.e., = 0xfffff...

/*
        disk_open: string filename, unsigned int n(umber of blocks)
                if filename exists and n==0 populate the global nblocks var
                if filename exists and n>0 recreate it with size n blocks
                if filename does not exist, create it with size n blocks
                blocks are zeroed;
        errors:
                -EBUSY	A disk is already open
                -ENOENT	Recreate asked but filename does not exist
                other errors returned as -1
*/
static int disk_open_nc(const char *filename, unsigned int n) {
  struct stat s;
  unsigned char buf[DISK_BLOCK_SIZE];

  if (theDisk.dfd != -1)
    return -EBUSY;

  // open existing file
  if (!n) {
    if ((theDisk.dfd = open(filename, O_RDWR)) == -1)
      return -ENOENT;
    if (fstat(theDisk.dfd, &s) == -1)
      return -1;
    theDisk.nblocks = s.st_size / DISK_BLOCK_SIZE;
    return 0;
  }

  // create or re-create
  if ((theDisk.dfd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
    return -1;

  memset(buf, 0, DISK_BLOCK_SIZE);
  for (int i = 0; i < n; i++)
    if (write(theDisk.dfd, buf, DISK_BLOCK_SIZE) == -1)
      return -1;

  theDisk.nblocks = n;

  return 0;
}

/*
        disk_stat:
                returns disk size (number of disk blocks)
        errors:
                -ENODEV	disk not open
*/
static int disk_stat_nc() {
  if (theDisk.dfd == -1)
    return -ENODEV;
  return theDisk.nblocks;
}

/*
        disk_read: read block at offset blknmbr, return data in buf
        errors:
                -ENODEV	disk not open
                -ENOSPC	disk has not that many blocks
                other errors returned as -1
*/
static int disk_read_nc(unsigned int blknmbr, unsigned char *buf) {
  if (theDisk.dfd == -1)
    return -ENODEV;
  if (blknmbr >= theDisk.nblocks)
    return -ENOSPC;
  if (lseek(theDisk.dfd, blknmbr * DISK_BLOCK_SIZE, SEEK_SET) == -1)
    return -1;
  if (read(theDisk.dfd, buf, DISK_BLOCK_SIZE) < DISK_BLOCK_SIZE)
    return -1;

  return 0;
}

/*
        disk_write: write block at offset blknmbr, get data from buf
        errors:
                -ENODEV	disk not open
                -ENOSPC	disk has not that many blocks
                other errors returned as -1
*/
static int disk_write_nc(unsigned int blknmbr, const unsigned char *buf) {
  if (theDisk.dfd == -1)
    return -ENODEV;
  if (blknmbr >= theDisk.nblocks)
    return -ENOSPC;
  if (lseek(theDisk.dfd, blknmbr * DISK_BLOCK_SIZE, SEEK_SET) == -1)
    return -1;
  if (write(theDisk.dfd, buf, DISK_BLOCK_SIZE) == -1)
    return -1;

  return 0;
}

// This is a random comment to prove a random point
// Another random change to test something else

/*
        disk_close: close an open disk
        errors:
                -ENODEV	disk not open
                other errors returned as -1
*/
static int disk_close_nc() {
  if (theDisk.dfd == -1)
    return -ENODEV;
  if (close(theDisk.dfd) == -1)
    return -1;
  theDisk.dfd = -1;
  return 0;
}

struct disk_operations disk_ops = {.open = disk_open_nc,
                                   .stat = disk_stat_nc,
                                   .read = disk_read_nc,
                                   .write = disk_write_nc,
                                   .close = disk_close_nc};
