#include "DueFlashStorage.h"

DueFlashStorage::DueFlashStorage() {
  uint32_t retCode;

  /* Initialize flash: 6 wait states for flash writing. */
  retCode = flash_init(FLASH_ACCESS_MODE_128, 6);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash init failed\n");
  }
}

byte DueFlashStorage::read(uint32_t offset, uint8_t flash_n) {
    uint32_t flash_start;
    flash_start = flash_n == 0 ? IFLASH0_ADDR : IFLASH1_ADDR;
    return ((byte*) flash_start)[offset];
}
// byte* DueFlashStorage::readAddress(uint32_t address) {
//   return FLASH_START+address;
// }
//
// boolean DueFlashStorage::write(uint32_t address, byte value) {
//   uint32_t retCode;
//   uint32_t byteLength = 1;
//   byte *data;
//
//   retCode = flash_unlock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + byteLength - 1, 0, 0);
//   if (retCode != FLASH_RC_OK) {
//     _FLASH_DEBUG("Failed to unlock flash for write\n");
//     return false;
//   }
//
//   // write data
//   retCode = flash_write((uint32_t)FLASH_START+address, &value, byteLength, 1);
//   //retCode = flash_write((uint32_t)FLASH_START, data, byteLength, 1);
//
//   if (retCode != FLASH_RC_OK) {
//     _FLASH_DEBUG("Flash write failed\n");
//     return false;
//   }
//
//   // Lock page
//   retCode = flash_lock((uint32_t)FLASH_START+address, (uint32_t)FLASH_START+address + byteLength - 1, 0, 0);
//   if (retCode != FLASH_RC_OK) {
//     _FLASH_DEBUG("Failed to lock flash page\n");
//     return false;
//   }
//   return true;
// }

boolean DueFlashStorage::write(uint32_t offset, byte *data, uint32_t dataLength, uint8_t flash_n) {
  uint32_t retCode;
  uint32_t flash_start, flash_size;

  flash_start = flash_n == 0 ? IFLASH0_ADDR : IFLASH1_ADDR;
  flash_size = flash_n == 0 ? IFLASH0_SIZE : IFLASH1_SIZE;

  if (offset >= flash_size) {
    _FLASH_DEBUG("Flash write address too high\n");
    return false;
  }

  if ((flash_start+offset & 3) != 0) {
    _FLASH_DEBUG("Flash start address must be on four byte boundary\n");
    return false;
  }

  // Unlock page
  retCode = flash_unlock(flash_start+offset, flash_start+offset + dataLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to unlock flash for write\n");
    return false;
  }

  // write data
  retCode = flash_write(flash_start+offset, data, dataLength, 1);

  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Flash write failed\n");
    return false;
  }

  // Lock page
    retCode = flash_lock(flash_start+offset, flash_start+offset + dataLength - 1, 0, 0);
  if (retCode != FLASH_RC_OK) {
    _FLASH_DEBUG("Failed to lock flash page\n");
    return false;
  }
  return true;
}
//
// boolean DueFlashStorage::write_unlocked(uint32_t address, byte value) {
//   uint32_t retCode;
//   uint32_t byteLength = 1;
//   byte *data;
//
//   // write data
//   retCode = flash_write((uint32_t)FLASH_START+address, &value, byteLength, 1);
//   //retCode = flash_write((uint32_t)FLASH_START, data, byteLength, 1);
//
//   if (retCode != FLASH_RC_OK) {
//     _FLASH_DEBUG("Flash write failed\n");
//     return false;
//   }
//
//   return true;
// }
//
// boolean DueFlashStorage::write_unlocked(uint32_t address, byte *data, uint32_t dataLength) {
//   uint32_t retCode;
//
//   if ((uint32_t)FLASH_START+address < IFLASH1_ADDR) {
//     _FLASH_DEBUG("Flash write address too low\n");
//     return false;
//   }
//
//   if ((uint32_t)FLASH_START+address >= (IFLASH1_ADDR + IFLASH1_SIZE)) {
//     _FLASH_DEBUG("Flash write address too high\n");
//     return false;
//   }
//
//   if (((uint32_t)FLASH_START+address & 3) != 0) {
//     _FLASH_DEBUG("Flash start address must be on four byte boundary\n");
//     return false;
//   }
//
//   // write data
//   retCode = flash_write((uint32_t)FLASH_START+address, data, dataLength, 1);
//
//   if (retCode != FLASH_RC_OK) {
//     _FLASH_DEBUG("Flash write failed\n");
//     return false;
//   }
//
//   return true;
// }
