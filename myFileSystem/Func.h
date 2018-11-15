#ifndef _FUNC_H
#define _FUNC_H
#include "Tree.h"
typedef enum { FUNCTION_SUCCESS, FUNCTION_FAIL ,FUNCTION_BAD_PATH} FuncResult;

typedef struct Function_t* Func;
typedef struct Function_t
{
	char*  name;
	FuncResult(*func)(Tree tree, char* destinaton, char* source);

};

Func FuncCreate(char* name, void* pFunc);

FuncResult mkdir(Tree tree, char* path, char* junk);

FuncResult touch(Tree tree, char* path, char* junk);

FuncResult ls(Tree tree, char* path, char* junk);

FuncResult mv(Tree tree, char* sourcePath, char* destinationPath);

FuncResult cp(Tree tree, char* sourcePath, char* destinationPath);

FuncResult rm(Tree tree, char* path, char* junk);

char* getDestAndSource(char* path);

int findFlag(char* command);

void exitPrg(Tree tree);

void destroyFunc(Func func);



#endif

