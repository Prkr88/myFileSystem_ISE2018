#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Func.h"
#include "Map.h"
#include "dataTypes.h"
#include "Tree.h"

extern int breakCode;

Func FuncCreate(char* name, void* pFunc)
{
	Func function = (Func)malloc(sizeof(struct Function_t));
	function->name = (char*)malloc(sizeof(char) * strlen(name)+1);
	strcpy(function->name, name);
	function->func = (void*)pFunc;
	return function;
}

char* getDestAndSource(char* path) 
{									 
	int i = 0;
	int j = 0;
	char* rev = (char*)malloc(sizeof(char)*strlen(path));
	char* pName = (char*)malloc(sizeof(char)*strlen(path));
	if (pName == NULL)
	{
		return NULL;
	}
	memset(pName, 0, strlen(path));
	memset(rev, 0, strlen(path));
	for (j = strlen(path)-1; j >= 0 && !(path[j] == '/' && path[j - 1] == ' '); j--)
	{
		rev[i] = path[j];
		i++;
	}
	rev[i] = '/';
	j = strlen(rev) - 1;
	for (i = 0; i < (int)strlen(rev); i++)
	{
		pName[i] = rev[j];
		j--;
	}

	pName[strlen(rev)] = '\0';
	
	return pName;

}
char* getLastNameInPath(char* path) 
{
	char* lastNameInPath = (char*)malloc(sizeof(char)*strlen(path)+1);
	memset(lastNameInPath, 0, strlen(path));
	char* tempPath = (char*)malloc(sizeof(char)*strlen(path)+1);
	int i = strlen(path) - 1;
	int j = 0;
	int check = 0;
	strcpy(tempPath, path);
	for (j = 2; j < (int)strlen(path); j++)
	{
		if (path[j] == '/')
		{
			check = 1;
		}
	}
	j = 0;
	if (check){
		while (tempPath[i] != '/')
		{
			j++;
			i--;
		}
		i = strlen(path) - 1;
		while (tempPath[i] != '/')
		{
			lastNameInPath[j - 1] = tempPath[i];
			i--;
			j--;
		}
	}
	else
	{
		lastNameInPath = path;
	}
	if (lastNameInPath[0] == '/')
	{
		for (i = 0; i < strlen(lastNameInPath); i++)
		{
			lastNameInPath[i] = lastNameInPath[i + 1];
		}
		lastNameInPath[i] = '\0';
	}
	return lastNameInPath;
}

char* getfatherNameOfNotExsist(char* path)
{
	int i = 0;
	char* tmpPath = (char*)malloc(strlen(path));
	memset(tmpPath, 0, strlen(path));
	strcpy(tmpPath, path);
	for (i = strlen(path); path[i] != '/'; i--)
	{
		tmpPath[i] = '\0';
	}
	tmpPath[i] = '\0';
	tmpPath = getLastNameInPath(tmpPath);
	if (strcmp(tmpPath, "/root") == 0)
	{
		for (i = 0; i < (int)strlen(tmpPath); i++)
		{
			tmpPath[i] = tmpPath[i + 1];
		}
	}
	return tmpPath;
}

char* pathCutter(char* path, int mode) //returns the path after findind the next element name.
{									  //mode=1 find current parent . mode=2 find current.
	int i = 0;
	int j = 0;
	int slashNum = 0;
	char* rev = (char*)malloc(sizeof(char)*strlen(path));
	char* pName = (char*)malloc(sizeof(char)*strlen(path));
	if (pName == NULL)
	{
		return NULL;
	}
	memset(pName, 0, strlen(path));
	memset(rev, 0, strlen(path));
	if (mode == 1){
		for (j = strlen(path); j >= 0 && slashNum != 2; j--)
		{
			if (path[j] == '/')
			{
				slashNum++;
				j--;
			}
			if (slashNum == 1)
			{
				rev[i] = path[j];
				i++;
			}
		}
		j = strlen(rev) - 1;
		for (i = 0; i < (int)strlen(rev); i++)
		{
			pName[i] = rev[j];
			j--;
		}

		pName[strlen(rev)] = '\0';
	}
	else if (mode == 2)
	{
		i = 0;
		for (j = strlen(path)-1; path[j] != '/'; j--)
		{
			rev[i] = path[j];
			i++;
		}
		j = strlen(rev) - 1;
		for (i = 0; i < (int)strlen(rev); i++)
		{
			pName[i] = rev[j];
			j--;
		}

		pName[strlen(rev)] = '\0';
	}
	
	return pName;
}

char* findNonExistentPath(Tree tree,char* path)
{
	int notFound = 0;
	int i = 0;
	DataType tmpToFind = NULL;
	char *nodeName = (char*)malloc(sizeof(char)*strlen(path));
	char* pathCopy = (char*)malloc(sizeof(char)*strlen(path));
	if (pathCopy == NULL)
	{
	return NULL;
	}
	for (i = 0; i < (int)strlen(path)-5; i++)
	{
		pathCopy[i] = path[i+5];
	}
	pathCopy[i] = '\0';
	while (pathCopy != NULL && !notFound)
	{
		for (i = 0; i < (int)strlen(pathCopy) && pathCopy[i]!='/'; i++)
		{
			nodeName[i] = pathCopy[i];
		}
		nodeName[i] = '\0';
		tmpToFind = folderCreate(nodeName);
		if (find(tree, tree->root, tmpToFind) == NULL)
		{
			notFound = 1;
		}
		for (i = 0; i < (int)strlen(path); i++)
		{
			pathCopy[i] = pathCopy[i + strlen(nodeName)+1];
		}
	}
	free(pathCopy);
	return nodeName; 
}
FuncResult mkdir(Tree tree, char* path, char* junk)
{
	DataType d = folderCreate(getLastNameInPath(path));
	DataType p = NULL;
	junk = NULL;
	TreeNode tmpNode = NULL;
	char* parentName = (char*)malloc(sizeof(char)*strlen(path)+1);
	strcpy(parentName, path);
	strcpy(parentName,pathCutter(parentName, 1));
	if (strcmp(parentName, "root") == 0)
	{
		add(tree, tree->root, (Element)d);
	}
	else
	{
		p = folderCreate(parentName);
		tmpNode = find(tree, tree->root, (Element)p);
		if (tmpNode!=NULL)
		{
			addIn(tree, (Element)p, (Element)d);
		}
		else
		{
			printf("Path %s does not exists \n", parentName);
		}
	}
	return FUNCTION_SUCCESS;
}


FuncResult touch(Tree tree, char* path, char* junk)
{
	DataType d = fileCreate(getLastNameInPath(path),100);
	DataType p = NULL;
	junk = NULL;
	TreeNode tmpNode = NULL;
	char* parentName = (char*)malloc(sizeof(char)*strlen(path)+1);

	strcpy(parentName, path);
	strcpy(parentName, pathCutter(parentName, 1));
	if (strcmp(parentName, "root") == 0)
	{
		add(tree, tree->root, (Element)d);
	}
	else
	{
		p = folderCreate(parentName);
		tmpNode = find(tree, tree->root, (Element)p);
		if (tmpNode != NULL)
		{
			addIn(tree, (Element)p, (Element)d);
		}
		else
		{
			printf("Path %s does not exists \n", parentName);
		}
	}
	return FUNCTION_SUCCESS;
}

FuncResult ls(Tree tree, char* path, char* flag)
{
	DataType p = NULL;      //tmp DataType to represent the parent to print from.
	TreeNode tmpN = NULL;
	char wrongPath[500] = { 0 };
	int mode = 0;
	char* parentName = (char*)malloc(sizeof(char)*strlen(path));
	strcpy(parentName, path);
	if (strcmp(path, "root/") == 0)
	{
		mode = 1;
		strcpy(parentName, pathCutter(parentName, mode));
	}
	else if (strcmp(flag, "r") == 0)
	{
		mode = 2;
		strcpy(parentName, pathCutter(parentName, mode));
	}
	else
	{
		strcpy(parentName, pathCutter(parentName, 2));
		mode = 3;
	}
	p = folderCreate(parentName); //TODO free p
	tmpN=find(tree, tree->root, p);
	if (tmpN != NULL)
	{
		if (printT(tree, tmpN, mode, 0) != 3)
		{
			printf("\n");
		}
		return FUNCTION_SUCCESS;
	}
	{
		strcpy(wrongPath,findNonExistentPath(tree, path));
		printf("Path %s does not exists \n", wrongPath);
		return FUNCTION_BAD_PATH;
	}
	
}

FuncResult mv(Tree tree, char* sourcePath, char* destinationPath)
{
	DataType s = NULL;					//tmp DataType to represent the source to move.
	DataType dFolder = NULL;			//tmp DataType to represent the destination Folder to move to.
	DataType dFather = NULL;
	DataType sFather = NULL;
	DataType dFile = NULL;				//tmp DataType to represent the destination File to move to.
	DataType dummy = NULL;				//tmp DataType of not exsisting object in path.
	TreeNode sourceNode = NULL;
	TreeNode destinationNode = NULL;
	TreeNode destinationParentNode = NULL;
	TreeNode sourceParentNode = NULL;
	TreeNode tmpFather = NULL;
	int validate = -1;
	char* sourceName = (char*)malloc(sizeof(char)*strlen(sourcePath));
	char* destinationName = (char*)malloc(sizeof(char)*strlen(sourcePath));
	strcpy(sourceName, getLastNameInPath(sourcePath));
	strcpy(destinationName, getLastNameInPath(destinationPath));
	s = fileCreate(sourceName, 120);
	dFolder = folderCreate(destinationName);
	dFile = fileCreate(destinationName,120);
	dFather = folderCreate(getfatherNameOfNotExsist(destinationPath));
	sFather = folderCreate(getfatherNameOfNotExsist(sourcePath));
	sourceNode = find(tree, tree->root, s);
	if (sourceNode == NULL)
	{
		printf("File %s does not exists \n", sourcePath);
	}
	else
	{
		if (!tree->compareElements(s, sourceNode->element))
		{
			printf("File %s does not exists \n", sourceName);
			return FUNCTION_BAD_PATH;
		}
		destinationParentNode = find(tree, tree->root, dFather);
		sourceParentNode = find(tree, tree->root, sFather);
		if (destinationParentNode == NULL || sourceParentNode == NULL)
		{
			validate = 4;
		}
		else
		{
			destinationNode = find(tree, tree->root, dFolder);   //search for folder
			if (destinationNode == NULL)
			{
				destinationNode = find(tree, tree->root, dFile);//search for file
				if (destinationNode == NULL)
				{
					validate = 4;
				}
			}
			if (destinationNode != NULL)
			{
				validate = reassign(tree, sourceNode, destinationNode, NULL);
			}
			else
			{
				dFolder = folderCreate(getfatherNameOfNotExsist(destinationPath));
				tmpFather = find(tree, tree->root, dFolder);
				if (destinationNode == NULL && tmpFather != NULL)
				{
					dummy = fileCreate(destinationName, 120);
					validate = reassign(tree, sourceNode, tmpFather, dummy);
				}
			}
		}
	}
	if (validate == 4)
	{
		if (sourceParentNode == NULL)
		{
			printf("Path %s does not exists \n", sourcePath);
		}
		else
		{
			printf("Path %s does not exists \n", destinationPath);
			return FUNCTION_BAD_PATH;
		}
	}
	return FUNCTION_SUCCESS;
}

FuncResult cp(Tree tree, char* sourcePath, char* destinationPath) //Add check to source
{
	DataType dFolder = NULL;
	DataType sFolder = NULL;
	DataType sFile = NULL;
	TreeNode toCopyTo = NULL;
	TreeNode toCopyFrom = NULL;
	char* sourceName = (char*)malloc(sizeof(char)*strlen(sourcePath));
	char* destinationName = (char*)malloc(sizeof(char)*strlen(sourcePath));
	char* sourceParentName = (char*)malloc(sizeof(char)*strlen(sourcePath));
	strcpy(sourceName, getLastNameInPath(sourcePath));
	strcpy(destinationName, getfatherNameOfNotExsist(destinationPath));
	strcpy(sourceParentName, getfatherNameOfNotExsist(sourcePath));
	sFile = fileCreate(sourceName, 120);
	dFolder= folderCreate(destinationName);
	sFolder = folderCreate(sourceParentName);
	toCopyTo = find(tree, tree->root, dFolder);
	toCopyFrom = find(tree, tree->root, sFolder);
	if (toCopyTo == NULL)	//check for bad destination Folder path
	{
		printf("Path %s does not exist\n", destinationName);
	}
	else if (toCopyFrom == NULL)
	{
		printf("Path %s does not exist\n", sourceParentName);
	}
	else
	{
		toCopyTo = find(tree, tree->root, sFile);
		if (toCopyTo == NULL)
		{
			printf("File %s does not exist\n", sourcePath);
		}
		else
		{
			addIn(tree, dFolder, sFile);
		}
	}

	return FUNCTION_SUCCESS;
}

FuncResult rm(Tree tree, char* sourcePath, char* flag)
{
	DataType tmpToRemoveFolder = NULL;//tmp Folder to remove
	DataType tmpToRemoveFile = NULL; //tmp File to remove
	TreeNode toRemoveNode = NULL;			//the Node to remove
	char* sourceName = (char*)malloc(sizeof(char)*strlen(sourcePath)); //TODO: FREE
	strcpy(sourceName, getLastNameInPath(sourcePath));
	tmpToRemoveFolder = folderCreate(sourceName);
	tmpToRemoveFile = fileCreate(sourceName, 120);
	toRemoveNode = find(tree, tree->root, tmpToRemoveFolder);
	if (toRemoveNode == NULL)	//check for bad destination Folder path
	{
		toRemoveNode = find(tree, tree->root, tmpToRemoveFile);
		if (toRemoveNode == NULL)
		{
			printf("File %s does not exist\n", sourceName);
		}
		else
		{
			subTreeDestroy(tree, toRemoveNode,0);
		} 
	}
	else
	{
		if (strcmp(flag,"r")==0)
		{
			subTreeDestroy(tree, toRemoveNode,0);
		}
		else
		{
			printf("The %s describes a Folder\n", sourcePath);
		}
	}
	return FUNCTION_SUCCESS;
}

int findFlag(char* command)
{
	return (int)strstr(command, "-r");
}
void exitPrg(Tree tree)
{
	TreeDestroy(tree);
	tree = NULL;
	breakCode = -999;
}


void destroyFunc(Func func)
{
	free(func->name);
	free(func);
	func = NULL;
}


