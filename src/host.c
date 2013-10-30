#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libusb.h>
#include <stdbool.h>

#define VID 0x03eb
#define DID 0x2064

static libusb_device_handle* dev = NULL;

void handleRet(int ret)
{
	switch (ret)
	{
		case LIBUSB_ERROR_TIMEOUT:
			printf("ERROR: timeout\n");
			break;
		case LIBUSB_ERROR_PIPE:
			printf("ERROR: broken pipe, I'm sexy and I know it ;)\n");
			break;
		case LIBUSB_ERROR_NO_DEVICE:
			printf("ERROR: no device\n");
			break;
		default:
			break;
	}
}

void setLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 1;
	uint16_t wValue = 0;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

void clearLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 0;
	uint16_t wValue = 0;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

void toggleLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 2;
	uint16_t wValue = 0;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

int main(int argc, char* argv[])
{
	libusb_init(NULL);
	int initialTrys = 60;
	while (dev == NULL)
	{
		dev = libusb_open_device_with_vid_pid(NULL, (uint16_t)VID, (uint16_t)DID);
		if (dev == NULL)
		{
			printf("ERROR: device could not be opened\n");
			initialTrys--;
			if (initialTrys == 0)
			{
				return -1;
			}
			sleep(1);
		}
	}

	for (int i=0; i<3; i++)
	{
		setLed(0);
		usleep(100000);
		clearLed(0);
		usleep(900000);
	}

	libusb_close(dev);
	libusb_exit(NULL);
	return 0;
}
