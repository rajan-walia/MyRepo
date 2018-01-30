// gvcomm.h
// Copyright General Vision Inc.
//----------------------------------------------------------------
//
// Declaration of the communication functions necessary to access
// NeuroMem_Smart hardware platforms
//
// The protocol implemented in the R/W functions below has been written
// to support a variety of communication drivers including USB_Cypress,
// USB FTDI, SPI, I2C, etc.
// for more details regarding this protocol, refer to
// http://www.general-vision.com/documentation/TM_NeuroMem_Smart_protocol.pdf
//
/**********************************************************************
int Connect(int DeviceID);
int Disconnect();
int Read(unsigned char module, unsigned char reg);
void Write(unsigned char module, unsigned char reg, int value);
int Write_Addr(int addr, int length_inByte, unsigned char data[]);
int Read_Addr(int addr, int length_inByte, unsigned char data[]);
**********************************************************************/

//
// Mandatory Map Address
//Definition of the NeuroMem module and registers
//
#define MOD_NM		0x01 // if using nmsimu.lib

#define NCR			0x00
#define COMP		0x01
#define LCOMP		0x02
#define DIST		0x03 
#define INDEXCOMP	0x03 
#define CAT			0x04
#define AIF			0x05
#define MINIF		0x06
#define MAXIF		0x07
#define TESTCOMP	0x08
#define TESTCAT		0x09
#define NID			0x0A
#define GCR			0x0B
#define RESETCHAIN	0x0C
#define NSR			0x0D
#define NCOUNT		0x0F	
#define FORGET		0x0F
