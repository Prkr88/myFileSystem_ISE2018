#include "Func.h"
#ifndef _MAP_H
#define _MAP_H

typedef struct Map_t* Map;
typedef enum {MapSuccess,MapBadString} MapResult;

Map mapCreate(void);
MapResult insert(Map map, Func value);
MapResult removeFromMap(Map map,char* key);
Func get(Map map,char* key);
void printMap(Map map);
char* getFuncName(char* command);
void executeFunc(Tree tree, Map map, char* key, char* path, char* source);
void DestroyMap(Map map);
char* getPath(char* command);
#endif


 