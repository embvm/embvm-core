# Updating Vendor Integrations

Vendor integrations are often not included in the framework wholesale. If you need to update vendor files (e.g., CMSIS, NRF52 MDK, FreeRTOS), please follow these instructions.

**Table of Contents:**

1. [CMSIS](#cmsis)
2. [FreeRTOS](#freertos)

## CMSIS

* [] Delete docs folder
* [] Delete CMSIS/Pack folder
* [] Delete documentation folder
* [] Delete Doxygen folder
* [] Delete Utilities folder
* [] Copy files over to src/processors/arm/CMSIS_5/
* [] Commit
* [] Update version + date in the Software Inventory

## FreeRTOS

* [] Navigate to FreeRTOS/Source
* [] Copy the include/ folder contents to src/os/freertos/include
* [] Copy the .c files in the Source/ folder to src/os/freertos/src/
* [] Copy the portable/ARMv8M folder to src/os/freertos/src/portable/ARMv8M
* [] Copy the portable/GCC folder to src/os/freertos/src/portable/GCC
* [] Copy the portable/Common folder to src/os/freertos/src/portable/Common
* [] Copy the portable/MemMang folder to src/os/freertos/src/portable/MemMang
* [] Commit
* [] Update version + date in the Software Inventory
