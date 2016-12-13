#pragma once

#include "fileHandler.h"

FILE *recordFile = NULL;
FILE *bTreeFile = NULL;

void openFiles()
{
	if (!recordFile)
		fopen_s(&recordFile, "records.txt", "w+");
	if (!bTreeFile)
		fopen_s(&bTreeFile, "bTree.txt", "w+");
}

void fileClose()
{
	fclose(recordFile);
	fclose(bTreeFile);
}


char *readRecord(int offset)
{
	static char buffer[PAGE_SIZE];
	static int posInFile = -1;

	if (posInFile == -1 || offset < posInFile || posInFile > offset + (int)(PAGE_SIZE / MAX_RECORD_LENGTH) + 1)
	{
		fseek(recordFile, offset, SEEK_SET);
		fgets(buffer, PAGE_SIZE, recordFile);
		posInFile = offset;
	}

	char *value = calloc(MAX_RECORD_LENGTH, sizeof(char));
	int key;
	sscanf_s(buffer + (offset - posInFile), "%u %s", &key, value);
	return value;
}

int writeRecord(char *record, int key)
{
	int filePosition = ftell(recordFile);
	fprintf_s(recordFile, "%u %s\n", key, record);
	return filePosition;
}

void writePage(TREE *tree)
{
	//je¿eli zapisujê stronê po raz pierwszy, dopisujê j¹ na koniec
	//w przeciwnym razie tylko aktualizujê

	if (tree->node->posInFile < 0)
		fseek(bTreeFile, 0, SEEK_END);
	else 
		fseek(bTreeFile, tree->node->posInFile, SEEK_SET);

	tree->node->posInFile = ftell(bTreeFile);

	fwrite(tree->node, sizeof(NODE), 1, bTreeFile);
}

TREE *readPage(int offset)
{
	TREE *tree = malloc(sizeof(TREE));
	tree->child = NULL;
	tree->parent = NULL;
	tree->modified = NULL;

	tree->node = malloc(sizeof(NODE));

	fseek(bTreeFile, offset, SEEK_SET);

	fread_s(tree->node, sizeof(NODE), sizeof(NODE), 1, bTreeFile);

	return tree;
}