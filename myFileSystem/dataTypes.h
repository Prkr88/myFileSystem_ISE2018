#ifndef _DATATYPES_H
#define _DATATYPES_H
#include "Tree.h"

typedef struct Data_t* DataType;

typedef struct Data_t {    
	int type;
	char* name;
	int data;
};


DataType folderCreate(char* name);
DataType fileCreate(char* name, int data);
int compareData(DataType d1, DataType d2);
DataType copyData(DataType d1);
int printDataType(DataType d1);
void freeData(DataType d1);
int compareLexi(DataType d1, DataType d2);

#endif