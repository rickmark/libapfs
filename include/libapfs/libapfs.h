#ifndef LIBAPFS_LIBAPFS_H
#define LIBAPFS_LIBAPFS_H

#include "apfs_specification.h"

nx_superblock_t* read_superblock(int address);
nx_efi_jumpstart_t* read_jumpstart(int address);
void* read_block(int address);
uint8_t* fletcher64_checksum(void* object);
void assert_arrays_equal(int length, uint8_t* x, uint8_t* y);
void load_and_execute(void* address);

#endif
