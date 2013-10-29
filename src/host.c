#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libusb.h>
#include <stdbool.h>

#define VID 0x03EB
#define DID 0x2064

int main(int argc, char* argv[])
{
	libusb_init(NULL);
	libusb_device_handle* dev = NULL;
	int initialTrys = 60;
	while (dev == NULL)
	{
		libusb_device_handle* dev = libusb_open_device_with_vid_pid(NULL, VID, DID);
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
		uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
		uint8_t bRequest = 1;
		uint16_t wValue = 0;
		uint16_t wIndex = 0;
		unsigned char data[256];
		uint16_t wLength = 0;
		unsigned int timeout = 500;
		int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
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
		usleep(100);
		bRequest = 0;
		ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
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
		usleep(900);
	}
	libusb_close(dev);
	libusb_exit(NULL);
#if 0
	libusb_init(NULL);
	libusb_device_handle* dev = NULL;
	int initialTrys = 60;
	while (dev == NULL)
	{
		libusb_device_handle* dev = libusb_open_device_with_vid_pid(NULL, VID, DID);
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
	while (true)
	{
		uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
		uint8_t bRequest = 0;//could be any if LIBUSB_REQUEST_TYPE_STANDARD in bmRequestType is not set
		uint16_t wValue = 0;
		uint16_t wIndex = 0;
		unsigned char* data[256];
		uint16_t wLength = 0;

		unsigned int timeout = 0;
		unsigned int currentSize = 0;
		bool exitFlag = false;
		while (true)
		{
			char* buf[256];
			gets(buf);
			if (strcmp(buf, "exit") == 0)
			{
				exitFlag = true;
				break;
			}
			if (strcmp(buf, "send") == 0)
			{
				break;
			}
			else
			{
				unsigned int length = strlen(buf);
				data[currentSize] = length;
				currentSize++;
				for (unsigned int i=0; i<length; i++)
				{
					data[currentSize] = buf[i];
					currentSize++;
				}
			}
		}
		if (exitFlag)
		{
			break;
		}
		wLength = currentSize;
		int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
		switch (ret)
		{
			case LIBUSB_ERROR_TIMEOUT:
				printf("ERROR: timeout\n");
				break;
			case LIBUSB_ERROR_PIPE:
				printf("ERROR: broken pipe, I am sexy and I know it ;)\n");
				break;
			case LIBUSB_ERROR_NO_DEVICE:
				printf("ERROR: no device\n");
				break;
			default:
		}
		if (bmRequestType & LIBUSB_ENDPOINT_IN)
		{
			unsigned int currentPos = 0;
			if (ret > currentPos)
			{
				unsigned int bufSize = data[currentPos];
				currentPos++;
				char* buf[bufSize+1];
				for (unsigned int i=0; i<bufSize; i++)
				{
					buf[i] = data[currentPos]
					currentPos++;
				}
				buf[bufSize] = '\0';
				printf("%s\n", buf);
			}
		}
	}
	libusb_close(dev);
	libusb_exit(NULL);
#endif
	return 0;
}
