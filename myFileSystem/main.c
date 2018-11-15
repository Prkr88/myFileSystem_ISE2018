#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Func.h"
#include "Map.h"
#include "Tree.h"
#include "dataTypes.h"

int breakCode = 0;

int main(void)
{
	
	char currentPath[50000] = { 0 };
	int commandLength = 10;
	int lastCommandIndex = 0;
	int stopInput = 0;
	char source[50000] = { 0 };
	char command[50000] = { 0 };
	char* tmpPath = NULL;
	char destination[50000] = { 0 };
	char recentPath[50000] = { 0 };
	char key[10] = { 0 };
	char* fName = NULL;
	int i = 0;
	Map map = mapCreate();
	Tree tree = treeCreate(&copyData, &freeData, &compareData, &printDataType, &compareLexi);
	DataType d1 = folderCreate("root"); //root name
	Func f1 = FuncCreate("mkdir", &mkdir);
	Func f2 = FuncCreate("ls", &ls);
	Func f3 = FuncCreate("touch", &touch);
	Func f4 = FuncCreate("mv", &mv);
	Func f5 = FuncCreate("cp", &cp);
	Func f6 = FuncCreate("rm", &rm);
	Func f7 = FuncCreate("exit", &exitPrg);
	insert(map, f1);
	insert(map, f2);
	insert(map, f3);
	insert(map, f4);
	insert(map, f5);
	insert(map, f6);
	insert(map, f7);
	add(tree, tree->root, (Element)d1);
	strcpy(currentPath, "root/");
	printf("Welcome!\n");
	while (breakCode != -999)
	{
		printf("@> ");

		fgets(command, 50000, stdin);
		command[strcspn(command, "\n")] = 0;
		if (strcmp(command, "ls") != 0 && strcmp(command, "exit")!=0)
		{
			tmpPath = getPath(command);
			strcpy(recentPath, tmpPath);
			free(tmpPath);
			tmpPath = NULL;
		}
		if (strcmp(recentPath, "root/") != 0)
		{
			strcat(currentPath, recentPath);
			strcpy(recentPath, currentPath);
		}
		fName=getFuncName(command);
		strcpy(key, fName);
		free(fName);
		fName = NULL;
		strcat(currentPath, "/");
		if (strcmp(key, "mv")==0 || strcmp(key, "cp")==0)
		{
			strcpy(destination, getDestAndSource(command));
			for (i = 0; i < (int)((strlen(command) - strlen(destination)) - 1); i++)
			{
				source[i] = command[i];
			}
			strcpy(source, getDestAndSource(source));
			executeFunc(tree, map, key, source, destination);
		}
		else if (strcmp(key, "ls") == 0 || strcmp(key,"rm")==0 )
		{
			if (tree->root != NULL)
			{
				strcpy(source, "");
				if (findFlag(command) != 0)
				{
					strcpy(source, "r");
				}
				executeFunc(tree, map, key, recentPath, source);
				strcpy(source, "");
			}
			else
			{
				printf("The Tree is empty \n");
			}
		}
		else if (strcmp(key, "exit") == 0)
		{
			executeFunc(tree, map, key, "", "");
			DestroyMap(map);
		}
		else if (strcmp(key, "mkdir") == 0 || strcmp(key, "touch") == 0)
		{
			executeFunc(tree, map, key, recentPath, source);
		}
		else
		{
			printf("Bad command: %s \n", key);
		}
		memset(recentPath, 0, strlen(recentPath));
		strcpy(currentPath, "root/");
	}
	return 0;
}

