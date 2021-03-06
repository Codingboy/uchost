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
			printf("ERROR: broken pipe, I am sexy and I know it ;)\n");
			break;
		case LIBUSB_ERROR_NO_DEVICE:
			printf("ERROR: no device\n");
			break;
		default:
			break;
	}
	ret = libusb_clear_halt(dev, 0);
	if (ret)
	{
		printf("error while cleaning up\n");
		switch (ret)
		{
			case LIBUSB_ERROR_NOT_FOUND:
				printf("ERROR: endpoint does not exist\n");
				break;
			case LIBUSB_ERROR_NO_DEVICE:
				printf("ERROR: device not found\n");
				break;
		}
		ret = libusb_clear_halt(dev, 0);
		if (ret)
		{
			printf("ERROR: stall could not be cleared\n");
			libusb_close(dev);
			libusb_exit(NULL);
			exit(-1);
		}
	}
}

void onLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 1;
	uint16_t wValue = (uint16_t)led;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

void offLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 0;
	uint16_t wValue = (uint16_t)led;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

void toggleLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT;
	uint8_t bRequest = 2;
	uint16_t wValue = (uint16_t)led;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 0;
	unsigned int timeout = 500;
	int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
}

bool checkLed(unsigned int led)
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN;
	uint8_t bRequest = 3;
	uint16_t wValue = (uint16_t)led;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 1;
	unsigned int timeout = 500;
	int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
	return data[0];
}

unsigned int measureEz3()
{
	uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN;
	uint8_t bRequest = 3;
	uint16_t wValue = (uint16_t)led;
	uint16_t wIndex = 0;
	unsigned char data[256];
	uint16_t wLength = 1;
	unsigned int timeout = 500;
	int ret = libusb_control_transfer(dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	handleRet(ret);
	return (unsigned int)(data[0])<<8 | (unsigned int)(dist[1]);
}

int main(int argc, char* argv[])
{
	libusb_init(NULL);
	while (dev == NULL)
	{
		dev = libusb_open_device_with_vid_pid(NULL, (uint16_t)VID, (uint16_t)DID);
		if (dev == NULL)
		{
			sleep(1);
		}
	}
	printf("device initialised\n");

	for (int i=0; i<3; i++)
	{
		onLed(0);
		usleep(100000);
		onLed(1);
		usleep(100000);
		onLed(2);
		usleep(100000);
		onLed(3);
		usleep(100000);
		offLed(0);
		usleep(100000);
		offLed(1);
		usleep(100000);
		offLed(2);
		usleep(100000);
		offLed(3);
		usleep(100000);
	}
	while (true)
	{
		unsigned int dist = measureEz3();
		if (dist == 0xffff)
		{
			offLed(0);
			offLed(1);
			offLed(2);
			offLed(3);
		}
		if (dist < 0xffff && >= 100)
		{
			onLed(0);
			offLed(1);
			offLed(2);
			offLed(3);
		}
		if (dist < 100 && >= 50)
		{
			onLed(0);
			onLed(1);
			offLed(2);
			offLed(3);
		}
		if (dist < 50 && >= 25)
		{
			onLed(0);
			onLed(1);
			onLed(2);
			offLed(3);
		}
		if (dist < 25)
		{
			onLed(0);
			onLed(1);
			onLed(2);
			onLed(3);
		}
	}

	libusb_close(dev);
	libusb_exit(NULL);
	return 0;
}
