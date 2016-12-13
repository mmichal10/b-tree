#pragma once

#include "tree.h"

TREE *tree = NULL;

void split(TREE *t, RECORD record);

int insertValue(TREE *t, RECORD record);

int hasChildren(NODE *node)
{
	int i = 0;
	for (int j = 0; j < 2 * DEEGREE + 1; j++)
		if (node->children[j] > 0)
			i++;
	return i;
}

int freeSpace(NODE *node)
{
	int i = 0;
	while (node->keys[i].key > 0)
		i++;

	return 2 * DEEGREE - i;
}

void cleanUp(TREE *tree)
{
	while (tree->child)
		tree = tree->child;

	while (tree->parent)
	{
		if (tree->modified > 0)
			writePage(tree);

		free(tree->node);
		tree = tree->parent;
		free(tree->child);
	}

	if (tree->modified > 0)
		writePage(tree);
}

void nullNode(NODE *node)
{
	for (int i = 0; i < 2 * DEEGREE; i++)
	{
		node->children[i] = -1;
		node->keys[i].key = -1;
		node->keys[i].posInFile = -1;
	}
	node->children[2 * DEEGREE] = -1;
	node->parent = 0;
	node->posInFile = -1;
}

RECORD getMedian(RECORD *tb, RECORD element)
{
	RECORD tmp[2 * DEEGREE + 1];
	for (int i = 0, j = 0; i < 2 * DEEGREE; j++)
		if (tb[i].key < element.key)
			tmp[j].key = tb[i++].key;
		else
		{
			tmp[j] = element;
			element.key = -1;
		}
	return tmp[DEEGREE];
}

int cmpRecords(const void *a, const void *b)
{
	return ((RECORD*)a)->key >= ((RECORD*)b)->key;
}

int getId(int val, NODE *parent)
{
	int i;
	for (i = 0; i < 2 * DEEGREE + 1; i++)
		if (parent->keys[i].key < val)
			return i;
}




void find(int fileOffset, int key)
{
	if (fileOffset != tree->node->posInFile)
	{
		tree->child = readPage(fileOffset);
		tree = tree->child;
	}

	int busy = 2 * DEEGREE - freeSpace(tree->node);

	if (!busy || !hasChildren(tree->node))
		return; 

	int i;
	for (i = 0; i < busy; i++)
		if (tree->node->keys[i].key == key)
			return;
		else if (tree->node->keys[i].key > key || )
			return find(tree->node->children[i], key);

	if (tree->node->children[i + 1] > 0)
		return find(tree->node->children[i + 1], key);
	else
		return;
}

int insertValue(TREE *t, RECORD record)
{
	if (freeSpace(t->node) == 0)
		return 0;

	//muszê przesun¹æ wszystkie elementy tablic, które s¹ wiêksze od nowego klucza
	int j;
	for (j = 2 * DEEGREE - 2; j > 0 && t->node->keys[j].key > record.key; j--)
		t->node->keys[j + 1] = t->node->keys[j];

	//szukam pierwszego wolnego miejsca
	int i;
	for (i = 0; i < 2 * DEEGREE; i++)
		if (t->node->keys[i].key <= 0)
			break;

	//hurraoptymistycznie za³o¿ê, ¿e putValue bêdzie u¿ywane tyko do dodawania rekordów do liœci
	//czyli nic nie robi ze wskaŸnikami na dzieci
	t->node->keys[i] = record;
	t->modified = 1;
	return 1;
}

void insert(char *text)
{
	static int recordCounter = 0;

	RECORD record;

	record.key = ++recordCounter;
	record.posInFile = writeRecord(text, recordCounter);

	if (tree == NULL)
	{
		tree = malloc(sizeof(TREE));
		tree->parent = NULL;
		tree->child = NULL;
		tree->node = malloc(sizeof(NODE));

		nullNode(tree->node);
		insertValue(tree, record, -1);
		cleanUp(tree);
		return;
	}
	else
	{
		find(tree->node->posInFile, recordCounter);
		if (insertValue(tree, record));
		else split(tree, record);

		cleanUp(tree);
		return;
	}
}

void split(TREE *t, RECORD record)
{
	if (t->parent)
	{
		TREE *newTree = malloc(sizeof(TREE));
		newTree->node = malloc(sizeof(NODE));
		newTree->child = NULL;
		newTree->parent = NULL;
		newTree->modified = 0;

		nullNode(newTree->node);

		RECORD tmp[2 * DEEGREE + 1];
		
		for (int i = 0; i < 2 * DEEGREE; i++)
			tmp[i] = tree->node->keys[i];
		tmp[2 * DEEGREE] = record;

		qsort(tmp, 2 * DEEGREE + 1, sizeof(RECORD), cmpRecords);

		for (int i = 0; i < DEEGREE; i++)
		{
			t->node->keys[i] = tmp[i];
			newTree->node->keys[i] = tmp[DEEGREE + i + 1];
			t->node->keys[DEEGREE + i].key = -1;
			t->node->keys[DEEGREE + i].posInFile = -1;
		}


		if (insertValue(t->parent, tmp[DEEGREE]));
		else split(t, tmp[DEEGREE]);

		//wpisujê w rodzicu wkaŸniki
		int idInParent = getId(tmp[DEEGREE].key, t->parent->node);

		writePage(newTree);

		t->parent->node->children[idInParent] = t->node->posInFile;
		t->parent->node->children[idInParent+1] = newTree->node->posInFile;
	}
	else
	{
		TREE *newTree = malloc(sizeof(TREE));
		newTree->node = malloc(sizeof(NODE));
		newTree->child = NULL;
		newTree->parent = NULL;
		newTree->modified = 1;

		nullNode(newTree->node);

		RECORD tmp[2 * DEEGREE + 1];

		for (int i = 0; i < 2 * DEEGREE; i++)
			tmp[i] = tree->node->keys[i];
		tmp[2 * DEEGREE] = record;

		qsort(tmp, 2 * DEEGREE + 1, sizeof(RECORD), cmpRecords);

		for (int i = 0; i < DEEGREE; i++)
		{
			t->node->keys[i] = tmp[i];
			newTree->node->keys[i] = tmp[DEEGREE + i + 1];
			t->node->keys[DEEGREE + i].key = -1;
			t->node->keys[DEEGREE + i].posInFile = -1;
		}

		TREE *newParent = malloc(sizeof(TREE));
		newParent->node = malloc(sizeof(NODE));
		newParent->child = 0;
		newParent->parent = 0;
		newParent->modified = 1;

		nullNode(newParent->node);

		insertValue(newParent, tmp[DEEGREE]);
		
		writePage(newParent);
		t->node->parent = newParent->node->posInFile;
		newTree->node->parent = newParent->node->posInFile;

		writePage(newTree);

		newParent->node->children[0] = t->node->posInFile;
		newParent->node->children[1] = newTree->node->posInFile;


		t->parent = newParent;
		newTree->parent = newParent;


		newParent->child = t;

		tree = newParent;
	}
}