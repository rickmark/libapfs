
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libapfs/libapfs.h>


int main() {
  nx_superblock_t* superblock = read_superblock(0);
  assert(superblock->nx_o.o_cksum == fletcher64_checksum(&superblock));
  assert(superblock->nx_magic == 'BSXN');
  paddr_t jumpstart_address = superblock->nx_efi_jumpstart;
  nx_efi_jumpstart_t* jumpstart = read_jumpstart(jumpstart_address);
  uint8_t* checksum = fletcher64_checksum(&jumpstart);
  assert_arrays_equal(MAX_CKSUM_SIZE, jumpstart->nej_o.o_cksum, checksum);
  assert(jumpstart->nej_version == 1);
  void* efi_driver = malloc(jumpstart->nej_efi_file_len);
  void* efi_driver_cursor = efi_driver;
  for (int i = 0; i < jumpstart->nej_num_extents; i++) {
    prange_t efi_extent_address = jumpstart->nej_rec_extents[i];
    for (int j = 0; j < efi_extent_address.pr_block_count; j++) {
      void* efi_block = read_block(efi_extent_address.pr_start_paddr + j);
      memcpy(efi_driver_cursor, efi_block, superblock->nx_block_size);
      efi_driver_cursor += superblock->nx_block_size;
    }
  }

  load_and_execute(efi_driver);

  return 0;
}
