// 
// USB FTDI communication controller for NeuroStack
// Copyright(c) General Vision Inc.
//
// Communication with the NeuroStack board via its USB port
// dependancy= ftdi driver

#include "WinTypes.h"
#include "stdlib.h"	 //for calloc
#include "string.h"  //for memcpy
#include "ftd2xx.h"

FT_HANDLE ftHandle;
FT_STATUS ftStatus; 
DWORD lengthout;

int platform = 1;
int navail = 4096;
int maxveclength = 256;

//-----------------------------------------------
// Open USB connection via FTDI
//-----------------------------------------------
int Connect(int DeviceID)
{
	//need to add the support of DeviceID
	FT_Close(ftHandle);
	ftStatus = FT_Open(0, &ftHandle);
	ftStatus = FT_SetLatencyTimer(ftHandle, 0x01);
	ftStatus = FT_SetBitMode(ftHandle, 0xFF, 0x40);
	ftStatus = FT_SetTimeouts(ftHandle, 3000, 500);
	return ((int)(ftStatus));
}

//-----------------------------------------------
// Close FTDI
//-----------------------------------------------
int Disconnect()
{
	ftStatus=FT_Close(ftHandle);
	return ((int)(ftStatus));
}

char* cmd_buff= (char*)calloc(520, 1); // 8 bytes of command + max of 512 bytes of data
char* data_buff= (char*)calloc(512, 1);


// --------------------------------------------------------
// Read the register of a given module (module + reg = addr)
//---------------------------------------------------------
int Read(char module, char reg)
{
	int data=0xFFFF;
	//The USB controller in the FPGA expect data length in word
	cmd_buff[0]= 0; //reserved for a revision number
	cmd_buff[1]= module;
	cmd_buff[2]= 0;
	cmd_buff[3]= 0;
	cmd_buff[4]= reg;
	cmd_buff[5]= 0;
	cmd_buff[6]= 0;
	cmd_buff[7]= 1;
	ftStatus = FT_Write(ftHandle, cmd_buff, 8, &lengthout);
	ftStatus = FT_Read(ftHandle, data_buff, 2, &lengthout);
	if (ftStatus == 0) data = (int)(data_buff[0] << 8) + data_buff[1];
	return(data);
}

// ---------------------------------------------------------
// Write the register of a given module (module + reg = addr)
// ---------------------------------------------------------
void Write(char module, char reg, int data)
{
	//The USB controller in the FPGA expect data length in word
	cmd_buff[0]= 0;
	cmd_buff[1]= module + 0x80;;
	cmd_buff[2]= 0;
	cmd_buff[3]= 0;
	cmd_buff[4]= reg;
	cmd_buff[5]= 0;
	cmd_buff[6]= 0;
	cmd_buff[7]= 1;
	cmd_buff[8] = (char)((data & 0xFF00) >> 8);
	cmd_buff[9] = (char)(data & 0x00FF);
	ftStatus=FT_Write(ftHandle, cmd_buff, 10, &lengthout);
	//return((int)ftStatus);
}

//-----------------------------------------------------
// Generic USB Write command
//-----------------------------------------------------
int Write_Addr(int addr, int length_inByte, char data[])
{
	//The USB controller in the FPGA expect data length in word
	int length_inWords = length_inByte >> 1;

	//length_inWords is rounded to next even length
	int rmder = length_inByte % 2;
	if (rmder != 0) length_inWords += 1;

	char* send_buff = (char*)calloc((8 + (length_inWords * 2)), sizeof(char));
	send_buff[0] = 0; //reserved for a board number
	int module = (char)((addr & 0xFF000000) >> 24);
	send_buff[1] = module | 0x80;
	send_buff[2] = (char)((addr & 0x00FF0000) >> 16);
	send_buff[3] = (char)((addr & 0x0000FF00) >> 8);
	send_buff[4] = (char)(addr & 0x000000FF);
	send_buff[5] = (char)((length_inWords & 0x00FF0000) >> 16);
	send_buff[6] = (char)((length_inWords & 0x0000FF00) >> 8);
	send_buff[7] = (char)(length_inWords & 0x000000FF);

	//append the input data to the command
	memcpy(send_buff + 8, data, length_inByte);
	ftStatus = FT_Write(ftHandle, send_buff, 8 + (length_inWords * 2), &lengthout);

	return((int)ftStatus);
}

//---------------------------------------------
// Generic USB Read command
//---------------------------------------------
int Read_Addr(int addr, int length_inByte, char data[])
{
	//The USB controller in the FPGA expect data length in word
	int length_inWords = length_inByte >> 1;

	//length_inWords is rounded to previous even length
	int rmder = length_inByte % 2;
	if (rmder != 0) length_inWords += 1;

	char send_buff[8];
	send_buff[0] = 0; //reserved for a revision number
	send_buff[1] = (char)((addr & 0xFF000000) >> 24);
	send_buff[2] = (char)((addr & 0x00FF0000) >> 16);
	send_buff[3] = (char)((addr & 0x0000FF00) >> 8);
	send_buff[4] = (char)(addr & 0x000000FF);
	send_buff[5] = (char)((length_inWords & 0x00FF0000) >> 16);
	send_buff[6] = (char)((length_inWords & 0x0000FF00) >> 8);
	send_buff[7] = (char)(length_inWords & 0x000000FF);
	ftStatus = FT_Write(ftHandle, send_buff, 8, &lengthout);

	char* data_buff = (char*)calloc((length_inWords * 2), sizeof(char));
	ftStatus = FT_Read(ftHandle, data_buff, (length_inWords * 2), &lengthout);
	memcpy(data, data_buff, length_inByte);

	return((int)ftStatus);
}

