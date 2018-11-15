#ifndef _TREE_H
#define _TREE_H

/*generic ADT of Tree of Elements*/
typedef void* Element;
typedef Element(*CopyFunction)(Element);
typedef int(*CompareFunction)(Element,Element);
typedef void(*FreeFunction)(Element);
typedef int(*PrintFunction)(Element);
typedef int(*CompareLexi)(Element,Element);
typedef struct Tree_t* Tree;
typedef struct TreeNode_t* TreeNode;

struct Tree_t {
	TreeNode root;
	CopyFunction copyElement;
	FreeFunction freeElement;
	CompareFunction compareElements;
	CompareLexi CompareLexiElements;
	PrintFunction printElement;
};

typedef struct TreeNode_t {
	Element element;
	int numOfChildren;
	int level;
	TreeNode parent;
	TreeNode childs;
	TreeNode siblings;
};

typedef enum {TREE_NULL_ROOT,TREE_SUCCESS,TREE_FAIL,TREE_EMPTY,TREE_BAD_PATH} TreeResult;

Tree treeCreate(CopyFunction copyFunction, FreeFunction freeFunction, CompareFunction comperfunction, PrintFunction printfunction, CompareLexi compareLexi);

TreeNode TreeNodeCreate(Tree tree, Element element);

void subTreeDestroy(Tree tree, TreeNode node, int firstFlag);

TreeResult add(Tree tree, TreeNode root, Element element);

TreeResult addIn(Tree tree, Element parent, Element toAdd);

TreeResult removeFromTree(Tree tree, Element toRemove);

TreeResult reassign(Tree tree, TreeNode toDelete, TreeNode toAdd, Element dummy);

TreeResult printT(Tree tree, TreeNode root, int mode, int printFlag);

void printAviv(Tree tree, TreeNode current, int numOfTabs);

TreeNode find(Tree tree, TreeNode root, Element parent);

void TreeDestroy(Tree tree);

#endif


