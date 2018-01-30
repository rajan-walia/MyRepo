// CogniPat.cpp
// Copyright(c) General Vision Inc.

#include "stdlib.h"	 //for calloc
#include "string.h"  //for memcpy
#include "stdio.h"	// file open
#include "NeuroMem.h" // declaration of the NeuroMem registers and functions
#include "gvcomm.h" // declaration of the communication functions to be found in the lib\comm_xyz folders
#include "gvcomm_ns4k.cpp" // declaration of the communication functions to be found in the lib\comm_xyz folders

extern int platform; // initialized by the GVcomm/Connect function
extern int navail; // initialized by the GVcomm/Connect function
				   // if harware features a NeuroMem expansion connector
				   // navail can be updated by the NeuroMem\CountNeuronsAvailable function
				   // to be executed immediately after the Connect since it clears the network
extern int maxveclength;// initialized by the GVcomm/Connect function

// --------------------------------------------------------------
// Get info about network capacity and neuron memory size
// depending on the selected NeuroMem platform passed to Connect function
// --------------------------------------------------------------
void GetNetworkInfo(int* neuronAvailable, int* neuronMemorySize)
{
	*neuronAvailable = navail;
	*neuronMemorySize = maxveclength;
}
// --------------------------------------------------------------
// Clear the memory of the neurons to the value 0
// Reset default GCR=1, MINIF=2, MANIF=0x4000, CAT=0
// --------------------------------------------------------------
void ClearNeurons()
{
	Write(1, NSR, 16);
	Write(1, TESTCAT, 0x0001);
	Write(1, NSR, 0);
	for (int i = 0; i< maxveclength; i++)
	{
		Write(1, INDEXCOMP, i);
		Write(1, TESTCOMP, 0);
	}
	Write(1, FORGET, 0);
}
// ------------------------------------------------------------ 
// Detect the capacity of the NeuroMem network
//
// - useful for hardware featuring a NeuroMem expansion connector
// - and consequently with a network capacity expandable beyond the default navail
//
// - must be executed immediately after the Connect function and prior to learning
//
// ------------------------------------------------------------ 
int CountNeuronsAvailable()
{
	Write(MOD_NM,FORGET, 0);
	Write(MOD_NM,NSR, 0x0010);
	Write(MOD_NM,TESTCAT, 0x0001);
	Write(MOD_NM,RESETCHAIN, 0);
	int read_cat;	
	navail = 0;
	while (1) {
		read_cat = Read(MOD_NM, CAT);
		if (read_cat == 0xFFFF)
			break;
		navail++;
	}
	Write(MOD_NM,NSR, 0x0000);
	Write(MOD_NM,FORGET, 0);
	return(navail);
}
//-----------------------------------------------
// Uncommit the neurons
// option to change the default MAXIF
//----------------------------------------------
void Forget()
{
	Write(MOD_NM,FORGET,0);
}
void Forget(int Maxif)
{
	Write(MOD_NM,FORGET,0);
	Write(MOD_NM,MAXIF, Maxif);
}
// --------------------------------------------------------
// Broadcast a vector to the neurons and return the recognition status
// 0= unknown, 4=uncertain, 8=Identified
//---------------------------------------------------------
int Broadcast(int vector[], int length)
{
	if (length>maxveclength) length = maxveclength;
	if (length == 1) Write(MOD_NM, 2, vector[0]);
	else
	{
		for (int i = 0; i < length - 1; i++) Write(MOD_NM, COMP, vector[i]);
		Write(MOD_NM, LCOMP, vector[length - 1]);
	}
	return(Read(MOD_NM, NSR));
}
//-----------------------------------------------
// Learn a vector using the current context value
//----------------------------------------------
int Learn(int vector[], int length, int category)
{
	Broadcast(vector, length);
	Write(MOD_NM, CAT,category);
	return(Read(MOD_NM,NCOUNT));
}
//----------------------------------------------
// Recognize a vector and return the response of the top firing neuron
// category (wo/ DEG flag), distance and identifier
//----------------------------------------------
int BestMatch(int vector[], int length, int* distance, int* category, int* nid)
{
	Broadcast(vector, length);
	*distance = Read(MOD_NM, DIST);
	*category= Read(MOD_NM, CAT) & 0x7FFF; 
	*nid =Read(MOD_NM, NID);
	return(Read(MOD_NM, NSR));
}
//----------------------------------------------
// Recognize a vector and return the response  of up to K top firing neurons
// The response includes the distance, category and identifier of the neuron
// The Degenerated flag of the category is masked
// Return the number of firing neurons or K whichever is smaller
//----------------------------------------------
int Recognize(int vector[], int length, int K, int distance[], int category[], int nid[])
{
	Broadcast(vector, length);
	int recoNbr=0;
	for (int i=0; i<K; i++)
	{
		distance[i] = Read(MOD_NM, DIST);
		if (distance[i]==0xFFFF)
		{ 
			category[i]=0xFFFF;
			nid[i]=0xFFFF;
		}
		else
		{
			recoNbr++;
			category[i]= Read(MOD_NM, CAT) & 0x7FFF;
			nid[i] =Read(MOD_NM, NID);
		}
	}
return(recoNbr);
}
// ------------------------------------------------------------ 
// Set a context and associated minimum and maximum influence fields
// ------------------------------------------------------------ 
void setContext(int context, int minif, int maxif)
{
	// context[15-8]= unused
	// context[7]= Norm (0 for L1; 1 for LSup)
	// context[6-0]= Active context value
	Write(MOD_NM, GCR, context);
	Write(MOD_NM, MINIF, minif);
	Write(MOD_NM, MAXIF, maxif);
}
// ------------------------------------------------------------ 
// Get a context and associated minimum and maximum influence fields
// ------------------------------------------------------------ 
void getContext(int* context, int* minif, int* maxif)
{
	// context[15-8]= unused
	// context[7]= Norm (0 for L1; 1 for LSup)
	// context[6-0]= Active context value
	*context = Read(MOD_NM, GCR);
	*minif = Read(MOD_NM, MINIF);
	*maxif = Read(MOD_NM, MAXIF);
}
// --------------------------------------------------------
// Set the neurons in Radial Basis Function mode (default)
//---------------------------------------------------------
void setRBF()
{
	int tempNSR = Read(MOD_NM, NSR);
	Write(MOD_NM, NSR, tempNSR & 0xDF);
}
// --------------------------------------------------------
// Set the neurons in K-Nearest Neighbor mode
//---------------------------------------------------------
void setKNN()
{
	int tempNSR = Read(MOD_NM, NSR);
	Write(MOD_NM, NSR, tempNSR | 0x20);
}

//--------------------------------------------------------------------------------------
// Read the content of a specific neuron
// Warning: neuuons are indexed in the chain starting at 1
// if index is greater than the number of committed neurons, read the RTL neuron
//--------------------------------------------------------------------------------------
void ReadNeuron(int neuronID, int* context, int model[], int* aif, int* minif, int* category)
{
	int ncount = Read(1, NCOUNT);
	if ((neuronID <= 0) | (neuronID > ncount))
	{
		*context = 0; *aif = 0; *minif = 0; *category = 0;
		for (int i = 0; i<maxveclength; i++) model[i] = 0;
		return;
	}
	int Temp = 0;
	int TempNSR = Read(1, NSR);
	Write(1, NSR, 0x0010);
	Write(1, RESETCHAIN, 0);
	if (neuronID > 1) { for (int i = 0; i < neuronID - 1; i++) Temp = Read(1, CAT); }
	*context = Read(1, NCR);
	for (int i = 0; i<maxveclength; i++) model[i] = Read(1, COMP);
	*aif = Read(1, AIF);
	*minif = Read(1, MINIF);
	*category = Read(1, CAT);
	Write(1, NSR, TempNSR);
	return;
}
void ReadNeuron(int neuronID, int neuron[])
{
	int ncount = Read(1, NCOUNT);
	if ((neuronID <= 0) | (neuronID > ncount))
	{
		for (int i = 0; i<maxveclength+4; i++) neuron[i] = 0;
		return;
	}
	int Temp = 0;
	int TempNSR = Read(1, NSR);
	Write(1, NSR, 0x0010);
	Write(1, RESETCHAIN, 0);
	if (neuronID > 1) { for (int i = 0; i < neuronID - 1; i++) Temp = Read(1, CAT); }
	neuron[0] = Read(1, NCR);
	for (int i = 0; i<maxveclength; i++) neuron[i+1] = Read(1, COMP);
	neuron[maxveclength+1] = Read(1, AIF);
	neuron[maxveclength+2] = Read(1, MINIF);
	neuron[maxveclength+3] = Read(1, CAT);
	Write(1, NSR, TempNSR);
	return;
}
//-------------------------------------------------------------
// Read the contents of the neurons
//-------------------------------------------------------------
int ReadNeurons(int *neurons)
{
	int ncount = Read(1, NCOUNT);
	memset(neurons,0, ncount*(maxveclength + 4)*sizeof(int));
	int* neuron=new int[maxveclength+4];
	if (ncount == 0) return(0);
	else
	{
		int TempNSR = Read(1, NSR);
		Write(1, NSR, 16);
		Write(1, RESETCHAIN, 0);
		for (int i = 0; i < ncount; i++)
		{
			neuron[0] = Read(1, NCR);
			for (int i = 0; i<maxveclength; i++) neuron[i + 1] = Read(1, COMP);
			neuron[maxveclength + 1] = Read(1, AIF);
			neuron[maxveclength + 2] = Read(1, MINIF);
			neuron[maxveclength + 3] = Read(1, CAT);
			try
			{
				memcpy(neurons + (i*(maxveclength + 4)), neuron, (maxveclength + 4) * sizeof(int));
			}
			catch (...)
			{
				printf("error in ReadNeurons memcpy of neuron %u", i);
			}
		}
		Write(1, NSR, TempNSR);
		return(ncount);
	}
}
//-------------------------------------------------------------
// load the neurons' content from file
//-------------------------------------------------------------
int WriteNeurons(int *neurons, int ncount)
{
	int TempNSR = Read(1, NSR);
	ClearNeurons();
	Write(1, NSR, 0x0010);
	Write(1, RESETCHAIN, 0);
	int* neuron = new int[maxveclength + 4];
	for (int i = 0; i<ncount; i++)
	{
		memcpy(neuron, neurons + (i*(maxveclength + 4)), (maxveclength + 4) * sizeof(int));
		Write(1, NCR, neuron[0]);
		for (int i = 0; i<maxveclength; i++) Write(1, COMP, neuron[i + 1]);
		Write(1, AIF, neuron[maxveclength + 1]);
		Write(1, MINIF, neuron[maxveclength + 2]);
		Write(1, CAT, neuron[maxveclength + 3]);
	}
	Write(1, NSR, TempNSR);
	return(Read(1, NCOUNT));
}
