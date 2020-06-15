/*
 * Device.h
 *
 *  Created on: Oct 2, 2012
 *      Author: Milman  Evgeniy
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <libusb-1.0/libusb.h>

#include "pvmex.h"

class Device {
private:
	libusb_device_handle * handle;
	libusb_device * device;
	int progress;
	ErrorCode error;
	char * ep0buffer;
	ErrorCode read(int request, uint16_t value, uint16_t index, size_t lenght, void *data);
	ErrorCode write(int request, uint16_t value, uint16_t index, size_t lenght, void *data);
public:
	Device(libusb_device * device);
	~Device();

	void open();
	void close();
	void uploadFX3Firmware(size_t lenght, char * data);
	void uploadFPGAFirmware(size_t lenght, char * data);
	void downloadFPGAFirmware(size_t* size, char ** data);
	unsigned char getAddress();
	void setAddress(unsigned char address);
	char* getFPGAVersion();
	void setFPGAVersion(const char* str);
	int getProgress();
	ErrorCode getError();
    libusb_device *getDevice() const;
    void reset();
};

#endif /* DEVICE_H_ */
