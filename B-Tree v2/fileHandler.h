#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "tree.h"

#define MAX_RECORD_LENGTH 30
#define PAGE_SIZE 512
#define DEEGREE 3

typedef struct TREE TREE;
typedef struct RECORD RECORD;
typedef struct NODE NODE;


void openFiles();
void fileClose();

/*
* Funkcje do obs³ugi pliku z rzeczywistymi rekordami.
*/
char *readRecord(int offset);
//zwraca pozycjê w pliku, pod któr¹ zostanie zapisany rekord
int writeRecord(char *record, int key);

/*
* Obs³uga B-drzewa.
*/
void writePage(TREE *node);
TREE* readPage(int offset);

