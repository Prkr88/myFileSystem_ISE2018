#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataTypes.h"

DataType folderCreate(char* name)
{
	DataType newDataT = (DataType)malloc(sizeof(struct Data_t));
	newDataT->name = (char*)malloc(sizeof(char)*strlen(name)+1);
	newDataT->type = 0;
	strcpy(newDataT->name, name);
	newDataT->data = 0;
	return newDataT;
}
DataType fileCreate(char* name, int data)
{
	DataType newDataT = (DataType)malloc(sizeof(struct Data_t));
	newDataT->name = (char*)malloc(sizeof(char)*strlen(name) + 1);
	newDataT->type = 1;
	strcpy(newDataT->name, name );
	newDataT->data = data;
	return newDataT;
}
int compareData(DataType d1, DataType d2)
{
	//check if casting helps
	int ans = 0;
	if (d1->type==d2->type)
	{
		if (strcmp(d1->name, d2->name) == 0)   
		{
			ans = 1;
		}
	}
	return ans;
}

int compareLexi(DataType d1, DataType d2)
{
	return strcmp(d1->name, d2->name);
}

DataType copyData(DataType d1)
{
	DataType newData;
	if ((DataType)d1->type == 0)
	{
		newData = folderCreate(d1->name);
	}
	else
	{
		newData = fileCreate(d1->name, d1->data);
	}
	return newData;
}

int printDataType(DataType d1)
{
	if (strcmp(d1->name,"root")!=0)
	{
		printf("%s ", d1->name);
	}
	return d1->type;
}


void freeData(DataType d1)
{
	//int i = 0;
	//free((DataType)d1->data);
	free((DataType)d1->name);
	//free((DataType)d1->type);
	free((DataType)d1);
	d1 = NULL;
}