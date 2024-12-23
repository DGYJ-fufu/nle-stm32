/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        15. April 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Device Description for ST Microelectronics STM32L1xx Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef FLASH_MEMORY

#ifdef STM32L1xx_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Med-density Flash", // Device Name (128kB/64kB/32kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00020000,                 // Device Size in Bytes (128kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x0100, 0x000000,           // Sector Size 256B (512 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_128

#ifdef STM32L1xx_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Med+-density Flash", // Device Name (256kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00040000,                 // Device Size in Bytes (256kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x0100, 0x000000,           // Sector Size 256B (1000 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_256

#ifdef STM32L1xx_384
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx High-density Flash", // Device Name (384kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00060000,                 // Device Size in Bytes (384kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x0100, 0x000000,           // Sector Size 256B (1512 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_384

#ifdef STM32L1xx_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx XL-density Flash", // Device Name (512kB)
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00080000,                 // Device Size in Bytes (512kB)
   256,                        // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x0100, 0x000000,           // Sector Size 256B (2000 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_512

#endif // FLASH_MEMORY


#ifdef FLASH_EEPROM

#ifdef STM32L1xx_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Mid-density Data EEPROM",    // Device Name (4kB)
   ONCHIP,                     // Device Type
   0x08080000,                 // Device Start Address
   0x00001000,                 // Device Size in Bytes (4kB)
   256,                        // Programming Page Size (256B)
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x100, 0x000000,            // Sector Size 256B (16 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_128


#ifdef STM32L1xx_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Mid+-density Data EEPROM",    // Device Name (8kB)
   ONCHIP,                     // Device Type
   0x08080000,                 // Device Start Address
   0x00002000,                 // Device Size in Bytes (8kB)
   256,                        // Programming Page Size (256B)
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x100, 0x000000,            // Sector Size 256B (16 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_256


#ifdef STM32L1xx_384
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx High-density Data EEPROM",    // Device Name (12kB)
   ONCHIP,                     // Device Type
   0x08080000,                 // Device Start Address
   0x00003000,                 // Device Size in Bytes (12kB)
   256,                        // Programming Page Size (256B)
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x100, 0x000000,            // Sector Size 256B (16 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_384

#ifdef STM32L1xx_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx XL-density Data EEPROM",    // Device Name (16kB)
   ONCHIP,                     // Device Type
   0x08080000,                 // Device Start Address
   0x00004000,                 // Device Size in Bytes (16kB)
   256,                        // Programming Page Size (256B)
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   500,                        // Program Page Timeout 500 mSec
   500,                        // Erase Sector Timeout 500 mSec

// Specify Size and Address of Sectors
   0x100, 0x000000,            // Sector Size 256B (32 Sectors)
   SECTOR_END
};
#endif // STM32L1xx_512

#endif // FLASH_EEPROM


#ifdef FLASH_OPTION

#ifdef STM32L1xx_128
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Mid-density Options",  // Device Name
   ONCHIP,                     // Device Type
   0x1FF80000,                 // Device Start Address
   0x00000010,                 // Device Size in Bytes (16)
   16,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0010, 0x000000,           // Sector Size 16B
   SECTOR_END
};
#endif // STM32L1xx_128

#ifdef STM32L1xx_256
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx Mid+-density Options",  // Device Name
   ONCHIP,                     // Device Type
   0x1FF80000,                 // Device Start Address
   0x00000010,                 // Device Size in Bytes (24)
   24,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0018, 0x000000,           // Sector Size 24B
   SECTOR_END
};
#endif // STM32L1xx_256

#ifdef STM32L1xx_384
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx High-density Options",  // Device Name
   ONCHIP,                     // Device Type
   0x1FF80000,                 // Device Start Address
   0x00000020,                 // Device Size in Bytes (32)
   32,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0020, 0x000000,           // Sector Size 32B
   SECTOR_END
};
#endif // STM32L1xx_384

#ifdef STM32L1xx_512
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "STM32L1xx XL-density Options",  // Device Name
   ONCHIP,                     // Device Type
   0x1FF80000,                 // Device Start Address
   0x00000028,                 // Device Size in Bytes (40)
   40,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0x00,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

// Specify Size and Address of Sectors
   0x0028, 0x000000,           // Sector Size 40B
   SECTOR_END
};
#endif // STM32L1xx_512

#endif // FLASH_OPTION 
