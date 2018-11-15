#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "Tree.h"
#define MIN_SIZE 10

struct Map_t
{
	char** keys;
	Func* values;
	int size;
};

Map mapCreate(void)
{
	int i = 0;
	Map map = (Map)malloc(sizeof(struct Map_t));
	map->size = 10;
	map->keys = (char**)malloc(sizeof(char*)*MIN_SIZE);
	map->values = (Func*)malloc(sizeof(Func)*MIN_SIZE);
	for (i = 0; i < MIN_SIZE; i++)
	{
		map->keys[i] = (char*)malloc(20 * sizeof(char));
		map->values[i] = (Func)malloc(sizeof(Func));
		strcpy(map->keys[i],"$");
		map->values[i] = NULL;
	}
	
	return map;
}


MapResult insert(Map map, Func value)
{
	int arrayLength = map->size;
	int i = 0;
	int index = 0;
	int lastIndexFound=0;
	if (strcmp(map->keys[arrayLength - 1],"$")!=0)
	{
		char** tmpKeys;
		Func* tmpValues;
		tmpKeys = (char**)malloc(sizeof(char*)+MIN_SIZE);
		tmpValues = (Func*)malloc(sizeof(Func)+MIN_SIZE);
		map->size = arrayLength + MIN_SIZE;
		arrayLength = map->size;
		for (i = 0; i < arrayLength; i++)
		{
			tmpKeys[i] = (char*)malloc(20 * sizeof(char));
			tmpValues[i] = (Func)malloc(20 * sizeof(Func));
		}
		for (i = 0; i < arrayLength; i++)
		{
			tmpKeys[i] = map->keys[i];
			tmpValues[i] = map->values[i];
		}
		map->keys = tmpKeys;//TODO Free OLD Pointers.
		map->values = tmpValues;
		map->keys[arrayLength] = value->name;
		map->values[arrayLength] = value;
	}
	else
	{
		while (index<arrayLength && !lastIndexFound)
		{
			if (strcmp(map->keys[index],"$")==0)
			{
				lastIndexFound = 1;
				index--;
			}
			index++;
		}
		strcpy(map->keys[index],value->name);
		map->values[index] = value;
	}

	return MapSuccess;
}
MapResult removeFromMap(Map map, char* key)
{

	int arrayLength = map->size;
	int indexToRemove = 0;
	int i = 0;
	int found=0;
	while(!found)
	{
		if (strcmp(map->keys[indexToRemove], key) == 0)
		{
			found = 1;
		}
		else
		{
			indexToRemove++;
		}
	}
	for (i = indexToRemove; i < arrayLength; i++)
	{
		map->keys[i] = map->keys[i + 1];
		map->values[i] = map->values[i + 1];
	}
	
	return MapSuccess;
}

Func get(Map map, char* key)
{
	int i = 0;
	int found = 0;
	int arrayLength = map->size;
	Func ans=NULL;
	for (i = 0; i < arrayLength-1 && !found; i++)
	{
		if (strcmp(map->keys[i], key) == 0)
		{
			found = 1;
			ans = map->values[i];
		}
	}
	return ans;
}

char* getFuncName(char* command)
{
	char* fName = (char*)malloc(sizeof(char)*strlen(command)+1);
	int i = 0;
	for (i = 0; (command[i] != ' ' && command[i]!='\n')  && i < (int)strlen(command); i++)
	{
		fName[i] = command[i];
	}
	fName[i] = '\0';
	return fName;
}

char* getPath(char* command)
{
	int i = 0;
	int index = 0;
	int j = 0;
	char* path = (char*)malloc(sizeof(char)*strlen(command)+10);
	for (i =0; command[i] != '/' && i<(int)strlen(command); i++)
	{}
	i++;
	for (index = i; index < (int)strlen(command);index++)
	{
		path[j] = command[index];
		j++;
	}
	path[j] = '\0';
	if (strlen(path) < 1)
	{
		strcpy(path, "root/");
	}
	return path;
}

void executeFunc(Tree tree, Map map, char* key, char* path, char* source)
{
	Func recentOp = get(map, key);
	recentOp->func(tree, path, source);
}

void DestroyMap(Map map)
{
	//Destroying the keys:
	int i = 0;
	for (i = 0; i < map->size; i++)
	{
		if (strcmp(map->keys[i],"$")==0)
		{
			free(map->keys[i]);
		}
		else
		{
			free(map->keys[i]);
			destroyFunc(map->values[i]);
		}
	}
	free(map->keys);
	free(map->values);
	free(map);
	map = NULL;

}