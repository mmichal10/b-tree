#include "generator.h"

char *randomRecord()
{
	int recordLen = 30;
	/*while (!recordLen)
	recordLen = rand() % MAX_RECORD_LENGTH;*/

	char *record = calloc(recordLen, recordLen);

	for (int i = 0; i < recordLen; i++)
	{
		unsigned char letter = rand() % ('z' - 'a') + 'a';
		record[i] = letter;
	}

	return record;
}