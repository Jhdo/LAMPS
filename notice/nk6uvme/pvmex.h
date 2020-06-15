/*
 * pvmex.h
 *
 *  Created on: Sep 16, 2012
 *      Author: evgeniy
 */

#ifndef PVMEX_H_
#define PVMEX_H_

#include <libusb-1.0/libusb.h>
#include <vector>

class Device;

#define  vendor_id  0x04b4
#define  product_id 0x00f0


// PVMEX protocol
#define I2C_EEPROM_WRITE              (0xBA)
#define I2C_EEPROM_READ               (0xBB)
#define SPI_FLASH_WRITE               (0xC2)
#define SPI_FLASH_READ                (0xC3)
#define SPI_FLASH_ERASE_POLL          (0xC4)

#define PVMEX_RESET					  (0xD6)
#define PVMEX_SET_FPGA_VERSION        (0xD5)
#define PVMEX_GET_FPGA_VERSION        (0xD4)
#define PVMEX_SET_ADDRESS             (0xD3)
#define PVMEX_GET_ADDRESS             (0xD2)

#define EP0SIZE  4096
#define FPGA_VERSION_SIZE 			   100

// --------------

enum ErrorCode{
	SUCCESS = 0,
	DEVICE_NOT_FOUND,
	DEVICE_CANNOT_BE_OPENED,
	SESSION_INITIALIZATION_FAILED,
	UNKNOWN,
	NULL_POINTER,
	LIBUSB_ERROR,
	WRONG_VALUE
};


std::vector<Device*> * getDeviceList();



#endif /* PVMEX_H_ */
