/* file NeuroMem.i */
%module NeuroMem
%{
/* Put C header file necessry to compile the interface */
#include "src/NeuroMem.h"
%}

/* list only the function you want to interface */
void GetNetworkInfo(int *neuronAvailable, int *neuronMemorySize);
void Forget();
void Forget(int Maxif);
void ClearNeurons();
int CountNeuronsAvailable();
		
void setContext(int context, int minif, int maxif);
void getContext(int* context, int* minif, int* maxif);
void setRBF();
void setKNN();
		
//Operations on single vector
int Broadcast(int* vector, int length);
int Learn(int* vector, int length, int category);
int BestMatch(int* vector, int length, int* distance, int* category, int* nid);
int Recognize(int* vector, int length, int K, int distance[], int category[], int nid[]);

//Loading and retrieval of neurons' content
int ReadNeurons(int *neurons);
int WriteNeurons(int *neurons, int ncount);
void ReadNeuron(int nid, int model[], int* context, int* aif, int* minif, int* category);
void ReadNeuron(int nid, int neuron[]);
		

