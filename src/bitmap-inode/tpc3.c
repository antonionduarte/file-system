#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef TPC3_BYTEMAP_H
#include "tpc3_bytemap.h"
extern struct bytemap_operations bmap_ops;
extern int max_bm_entries;
#endif

#ifndef TPC3_INODE_H
#include "tpc3_inode.h"
extern struct inode_operations inode_ops;
#endif

#define LINESIZE 16
#define ARGVMAX 4

int makeargv(char *s, char *argv[ARGVMAX + 1]) {
  // in: s points a text string with words
  // out: argv[] points to all words in the string s (*s is modified!)
  // pre: argv is predefined as char *argv[ARGVMAX+1]
  // return: number of words pointed to by the elements in argv (or -1 in case
  // of error)

  int ntokens;

  if (s == NULL || argv == NULL || ARGVMAX == 0)
    return -1;

  ntokens = 0;
  argv[ntokens] = strtok(s, " \t\n");
  while ((argv[ntokens] != NULL) && (ntokens < ARGVMAX)) {
    ntokens++;
    argv[ntokens] = strtok(NULL, " \t\n");
  }
  argv[ntokens] = NULL; // it must terminate with NULL
  return ntokens;
}

// deals with allocate first free entry, specified as -1
// must return the entry number, to be used in inode allocation
int bmap_allocate(int entry) {
  int ercode;

  if (entry == -1) {
    // **TODO** use the appropriate function to get a free bytemap entry
    if (ercode < 0)
      return ercode;
    entry = ercode;
  }

  // **TODO** use the appropriate function to allocate the entry
  // previously found
  if (ercode < 0)
    return ercode;

  return entry;
}

void run_A(char *args[]) {
  int ercode;

  ercode = bmap_allocate(atoi(args[1]));
  if (ercode >= 0) { // ercode >= 0 is not an error, is the entry for the inode
    // **TODO** use the appropriate function to allocate the inode
    printf("%s %s\t\tOK\n", args[0], args[1]);
  } else
    printf("%s %s\t\tERROR\n", args[0], args[1]);
}

void run_C(char *args[]) {
  int ercode;

  ercode = disk_ops.close();
  if (ercode < 0)
    printf("%s\t\tERROR\n", args[0]);
  else
    printf("%s\t\tOK\n", args[0]);
}

void run_D(char *args[]) {
  int ercode;

  ercode = // deallocate the entry (atoi(args[1]));
      if (ercode < 0) printf("%s %s\t\tERROR\n", args[0], args[1]);
  else { // ercode >= 0 is not an error, is the entry for the inode
    // **TODO** use the appropriate function to deallocate the inode
    printf("%s %s\t\tOK\n", args[0], args[1]);
  }
}

void run_O(int argc, char *args[]) {
  int ercode;
  int nblocks = 0;

  if (argc == 3)
    nblocks = atoi(args[2]);
  ercode = disk_ops.open(args[1], nblocks);
  if (ercode < 0) {
    if (argc == 3)
      printf("%s %s %s\tERROR\n", args[0], args[1], args[2]);
    else
      printf("%s %s\t\tERROR\n", args[0], args[1]);
  } else {
    if (argc == 3)
      printf("%s %s %s\tOK\n", args[0], args[1], args[2]);
    else
      printf("%s %s\t\tOK\n", args[0], args[1]);
  }
}

void runcommand(int argc, char *args[]) {

  switch (args[0][0]) {
  case 'A':
    run_A(args);
    break;
  case 'B':
    bytemap_print_table(max_bm_entries);
    break;
  case 'C':
    run_C(args);
    break;
  case 'D':
    run_D(args);
    break;
  case 'I':
    inode_table_print(max_bm_entries);
    break;
  case 'O':
    run_O(argc, args);
    break;
  case 'S':
    printf("%s %s\n", args[0], args[1]);
    max_bm_entries = atoi(args[1]);
    break;
  case 'T':
    printf("%s %s\n", args[0], args[1]);
    max_bm_entries = atoi(args[1]);
    break;
  default:
    printf("WRONG SPEC FILE?\n");
    break;
  }
}

void interp() {
  char line[LINESIZE];
  char *av[ARGVMAX];
  int argc;

  while (fgets(line, LINESIZE, stdin) != NULL) {
    if ((argc = makeargv(line, av)) > 0)
      runcommand(argc, av);
  }
}

int main(int argc, char *argv[]) {

  interp();

  return 0;
}
