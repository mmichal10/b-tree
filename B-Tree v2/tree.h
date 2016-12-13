#pragma once

#include "fileHandler.h"

#define DEEGREE 3

typedef struct TREE TREE;
typedef struct RECORD RECORD;
typedef struct NODE NODE;


typedef struct RECORD
{
	int key;
	int posInFile;
}RECORD;

typedef struct NODE
{
	int posInFile;
	RECORD keys[2 * DEEGREE];
	int children[2 * DEEGREE + 1];
	int parent;
}NODE;

typedef struct TREE
{
	char modified;
	TREE *parent;
	TREE *child;
	NODE *node;
}TREE;


void find(int fileOffst, int key);
void insert(char *text);