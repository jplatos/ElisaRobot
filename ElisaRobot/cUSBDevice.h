#pragma once
#ifndef __USBDEVICE_H__
#define __USBDEVICE_H__
#include "libusb.h"

class cUSBDevice
{
protected:
	struct libusb_device_handle *m_Handle = NULL;

	inline int FindDevice();
public:
	const static int ERR_OK = 0;
	const static int ERR_INIT = -1;
	const static int ERR_DEVICE_NOT_FOUND = -2;
	const static int ERR_INTERFACE_CLAIM = -3;
	const static int ERR_WRITE_ERROR = -4;
	const static int ERR_SHORT_WRITE = -5;
	const static int ERR_READ_ERROR = -6;
	const static int ERR_SHORT_READ = -7;

	inline int Send(unsigned char* data, int nbytes);

	inline int Receive(unsigned char* data, int nbytes);

	inline int Open();

	inline void Close();
};

int cUSBDevice::Open()
{
	int error = 0;
	int result = ERR_OK;
	error = libusb_init(NULL);
	if (error < 0) {
		result = ERR_INIT;
		goto RET;
	}
	error = FindDevice();
	if (error < 0) {
		result = ERR_DEVICE_NOT_FOUND;
		goto RET;
	}

	error = libusb_claim_interface(m_Handle, 0);
	if (error < 0) {
		result = ERR_INTERFACE_CLAIM;
		goto RET;
	}

RET:
	return result;
}

void cUSBDevice::Close()
{
	libusb_release_interface(m_Handle, 0);
	libusb_close(m_Handle);
	libusb_exit(NULL);
}

int cUSBDevice::FindDevice(void) {
	m_Handle = (libusb_device_handle*)libusb_open_device_with_vid_pid(NULL, 0x1915, 0x0101);
	return m_Handle ? 0 : -1;
}

int cUSBDevice::Send(unsigned char* data, int nbytes) {

	int transferred = 0;
	int r = 0;

	r = libusb_bulk_transfer(m_Handle, 0x01, (unsigned char*)data, nbytes, &transferred, 5000); // address 0x01
	if (r < 0) {
		return ERR_WRITE_ERROR;
	}
	if (transferred < nbytes) {
		return ERR_SHORT_WRITE;
	}

	return ERR_OK;

}

int cUSBDevice::Receive(unsigned char* data, int nbytes) {

	int received = 0;
	int r = 0;

	r = libusb_bulk_transfer(m_Handle, 0x81, (unsigned char*)data, nbytes, &received, 5000);
	if (r < 0) {
		return ERR_READ_ERROR;
	}
	if (received < nbytes) {
		return ERR_SHORT_READ;
	}

	return ERR_OK;
}





#endif