#define TPC3_BYTEMAP_H

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#define BYTEMAP_OFFSET		0  // Disk block where the bytemap starts


/* operations on inode structures

  allocate: Sets the entry to "used" (1), writes it to disk;
    parameters:
     @in: entry address
    errors:
     -EFBIG entry address past bmap size
     -EBUSY the entry was already allocated
     those resulting from disk operations

  deallocate: sets the entry to "free" (0), writes it to disk
    parameters:
     @in: entry number
    errors:
     -EFBIG entry past bmap size
     -EINVAL the entry was already free
     those resulting from disk operations

  getfree: get the first available (free) entry
    parameters:
     @out: free entry number
    errors:
     -ENOSPC there are no free entries
     those resulting from disk operations

*/

/* Helper function prototypes

  bytemap_print_table: prints the full table, 16 entries per line
    parameters:
     @in: number of entries in table

*/

int bytemap_print_table(unsigned int max_entries);


struct bytemap_operations {
  int (*allocate)(unsigned int entry);
  int (*deallocate)(unsigned int entry);
  int (*getfree)();
};

