#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

TreeNode find(Tree tree, TreeNode root, Element parent);
TreeResult printT(Tree tree, TreeNode root, int tabNum, int printFlag);
void nodeDestroy(Tree tree, TreeNode node);


Tree treeCreate(CopyFunction copyFunction, FreeFunction freeFunction, CompareFunction comperfunction, PrintFunction printfunction ,CompareLexi compareLexi)
{
	Tree tree = (Tree)malloc(sizeof(struct Tree_t));
	if (tree == NULL)
	{
		return NULL;
	}
	tree->root = (TreeNode)malloc(sizeof(TreeNode));
	if (tree == NULL)
	{
		return NULL;
	}
	tree->root = NULL;
	tree->copyElement = copyFunction;
	tree->freeElement = freeFunction;
	tree->compareElements = comperfunction;
	tree->printElement = printfunction;
	tree->CompareLexiElements = compareLexi;
	return tree;
}

TreeNode TreeNodeCreate(Tree tree ,Element element)
{
	TreeNode newNode = (TreeNode)malloc(sizeof(struct TreeNode_t));
	if (newNode == NULL)
	{
		return NULL;
	}
	newNode->element = (Element)tree->copyElement(element); 
	newNode->childs = NULL;
	newNode->siblings = NULL;
	newNode->parent = NULL;
	newNode->numOfChildren = 0;
	newNode->level = 0 ;
	return newNode;
}

TreeNode findParent(Tree tree, TreeNode root, Element son)
{
	TreeNode current = NULL;
    TreeNode parent = NULL;
	TreeNode ans = NULL;
	parent = root;
	current = root->childs;
		while (current != NULL && ans == NULL)
		{
			if (tree->compareElements(current->element, son))
			{
				ans = parent;
			}
			current = current->siblings;
			if (ans == NULL)
			{
				find(tree, current, parent);
			}
		}
	
	return ans;
}

TreeNode find(Tree tree,TreeNode root, Element parent)
{
	TreeNode current=NULL;
	TreeNode ans = NULL;
	current = root;
	if (tree->compareElements(root->element, parent))
	{
		ans = root;
	}
	else
	{

		if (current->childs != NULL && ans == NULL)
		{
			ans = find(tree, current->childs, parent);
		}

		if (current->siblings != NULL && ans == NULL)
		{
			ans = find(tree, current->siblings, parent);
		}	
	}
	return ans;
}


TreeResult add(Tree tree, TreeNode root,Element element) // Adds Node to tree root.
{
	Element elementToAdd = tree->copyElement(element);
	TreeNode toAdd = TreeNodeCreate(tree,elementToAdd);
	if (root == NULL) //if the tree is empty  , this node is the root.
	{ 
		tree->root = toAdd;
	}
	else //if the tree is not empty  , we add the node as child of root.
	{

		TreeNode current = root;
		if (current->childs == NULL)
		{
			current->childs = toAdd;
			toAdd->parent = root;
			toAdd->level = (toAdd->parent->level)+1;
			root->numOfChildren++;
		}
		else
		{
			if (current->childs != NULL && current->childs->siblings == NULL)
			{
				if (tree->CompareLexiElements(element, current->childs->element) < 0)
				{
					toAdd->siblings = current->childs;
					root->childs = toAdd;
				}
				else
				{
					current->childs->siblings = toAdd;
				}
				toAdd->parent = root;
				toAdd->level = (toAdd->parent->level)+1;
				root->numOfChildren++;
			}
			else
			{
				current = root->childs;
				if (tree->CompareLexiElements(element, current->element) < 0)
				{
					toAdd->siblings = current;
					root->childs = toAdd;
					toAdd->parent = root;
					toAdd->level = (toAdd->parent->level) + 1;
					root->numOfChildren++;
				}
				else
				{
					while (current->siblings->siblings != NULL && (tree->CompareLexiElements(element, current->siblings->element) > 0))
					{
						current = current->siblings;		
					}
					if (current->siblings->siblings == NULL && (tree->CompareLexiElements(element, current->siblings->element) > 0))
					{
						current->siblings->siblings = toAdd;
					}
					else
					{
						toAdd->siblings = current->siblings;
						current->siblings = toAdd;
					}
					toAdd->parent = root;
					toAdd->level = (toAdd->parent->level) + 1;
					root->numOfChildren++;
				}
			}
		}
	}
	return TREE_SUCCESS;
}

TreeResult addIn(Tree tree,Element parent, Element toAdd)
{
	TreeNode pNode = NULL;
	pNode = find(tree, tree->root, parent);
	if (pNode != NULL)
	{
		add(tree, pNode, toAdd);
		return TREE_SUCCESS;
	}
	else return TREE_FAIL;
}

TreeResult removeFromTree(Tree tree, Element toRemove)//TODO Add recursion to free childs
{
	TreeNode pParent = NULL;
	TreeNode pRoot = NULL;
	TreeNode current = NULL;
	TreeNode pNode = find(tree, tree->root, toRemove);
	if (pNode != NULL)
	{
		if (tree->compareElements(tree->root->element, toRemove))
		{
			pRoot = TreeNodeCreate(tree, toRemove);
			pRoot->childs = tree->root;
			pParent = pRoot;
			pNode = pRoot->childs;
		}
		else
		{
			pParent = pNode->parent;
		}
		current = pParent;
		if (pParent->childs == pNode)
		{
			(pParent->childs) = (pParent->childs->siblings);
			nodeDestroy(tree, pNode);
			pParent->numOfChildren--;
			if (tree->compareElements(tree->root->element, toRemove))
			{
				tree->root = NULL;
			}
			return TREE_SUCCESS;
		}
		current = pParent->childs;
		while (current->siblings != pNode)
		{
			current = current->siblings;
		}
		current->siblings = pNode->siblings;
		nodeDestroy(tree, pNode);
		pParent->numOfChildren--;
		return TREE_SUCCESS;
	}
	else
	{
		return TREE_BAD_PATH;
	}
}

TreeResult reassign(Tree tree,TreeNode toDelete, TreeNode toAdd,Element dummy)
{
	
	if (dummy == NULL && toAdd != NULL)    //inserts to Folder
	{
		Element tmpToDelete = tree->copyElement(toDelete->element);
		removeFromTree(tree, toDelete->element);    //deletes the source
		addIn(tree, toAdd->element, tmpToDelete); //add in destination
	}
	else if (dummy != NULL && toAdd != NULL) //checks for bad path . if toAdd is null there no such root;
	{
		removeFromTree(tree, toDelete->element);
		addIn(tree, toAdd->element, dummy);
	}
	else
	{
		return TREE_BAD_PATH;
	}
	return TREE_SUCCESS;
}

TreeResult printT(Tree tree, TreeNode root,int mode ,int printFlag)
{
	TreeNode current = root;
	int i = 0;
	int ans = 0;
	if (printFlag != 0)
	{
		if (!tree->printElement(current->element))//Prints num of childern only if it is folder.
		{
			if (!tree->compareElements(tree->root->element, current->element))
			{
				printf("%d", current->numOfChildren);
			}
		}
	}
	if (current->childs != NULL && mode != 4)
	{
		if (!tree->compareElements(tree->root->element, current->element) && printFlag!=0)
		{
			printf("\n");
		}
		for (i = 0; i < current->childs->level - 2; i++) //Tabs the level
		{
			printf("\t");
		}
		if (mode == 3)
		{
			ans=printT(tree, current->childs, 4, 1);
		}
		else
		{
			ans=printT(tree, current->childs, 1, 1);
		}

	}
	if (current->siblings != NULL && mode != 2 && mode != 3)
	{
		printf("\n");
		for (i = 0; i < current->siblings->level - 2; i++) //Tabs the level
		{
			printf("\t");
		}
		if (mode == 4)
		{
			ans=printT(tree, current->siblings, 4, 1);
		}
		else
		{
			ans=printT(tree, current->siblings, 1, 1);
		}

	}
	if (printFlag==1 || ans==1)
	{
		return TREE_SUCCESS;
	}
	else
	{
		return TREE_EMPTY;
	}
}



void subTreeDestroy(Tree tree ,TreeNode node ,int firstFlag)
{
	TreeNode current = node;
	TreeNode prev = node;
	int prevFlag = 0;
	if (node->childs != NULL)
	{
		subTreeDestroy(tree, node->childs,1);
	}
	if (current->siblings != NULL && firstFlag==1)
	{
		while (node->siblings != NULL)
		{
			while (current->siblings != NULL)
			{
				current = current->siblings;
				if (prevFlag == 0)
				{
					prevFlag = 1;
				}
				else
				{
					prev = prev->siblings;
				}
			}
			prevFlag = 0;
			removeFromTree(tree, current->element);
			prev->siblings = NULL;
			current = node;
			prev = node;
		}
		removeFromTree(tree, node->element);
		node->childs = NULL;
	}
	if(firstFlag==0)
	{
		removeFromTree(tree, node->element);
	}
}

void nodeDestroy(Tree tree,TreeNode node)
{
	tree->freeElement(node->element);
	free(node);
	node = NULL;
}

void TreeDestroy(Tree tree)
{
	TreeNode current = tree->root->childs;
	while (tree->root->childs != NULL)
	{
		while (current->siblings != NULL)
		{
			current = current->siblings;
		}
		subTreeDestroy(tree, current, 0);
		current = NULL;
		current = tree->root->childs;
	}
	nodeDestroy(tree, tree->root);
	free(tree);
}
