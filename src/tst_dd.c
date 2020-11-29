#include <stdio.h>
#include <string.h>

#include "disk_driver.h"

extern struct disk_operations disk_ops;

unsigned char buf[DISK_BLOCK_SIZE];
unsigned char buf1[DISK_BLOCK_SIZE];


int main() {
	int ercode;
	// create a disk named disk0 w/ size 100 blocks
	ercode = disk_ops.open("disk0", 100); printf("open %d\n", ercode);

	// get the disk status... it must return 100 
	ercode = disk_ops.stat(); printf("size %d\n", ercode);

	memset(buf, 1, DISK_BLOCK_SIZE);
	// escrever um bloco na posição 1 com zeros nos 512 bytes
	ercode = disk_ops.write(1, buf); printf("write %d\n", ercode);

	// ler o bloco na posição 1 para outro buffer
	ercode = disk_ops.read(0, buf1); printf("read %d\n", ercode);
	if (!memcmp(buf, buf1, DISK_BLOCK_SIZE)) printf("contents match!\n");

	ercode = disk_ops.close(); printf("close %d\n", ercode);

	// disco fechado, deve dar erro...
	ercode = disk_ops.write(1, buf); printf("write %d\n", ercode);
} 