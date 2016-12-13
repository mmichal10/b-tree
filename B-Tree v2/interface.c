#include "interface.h"

void mainMenu()
{
	openFiles();
	while (1)
	{
		system("CLS");
		printf("1. Dodaj rekord\n");
		printf("2. Znajdz rekord\n");
		printf("3. Usun rekord\n");
		printf("4. Wyœwietl drzewo\n");
		printf("0. - wyjscie\n");
		fseek(stdin, 0, SEEK_END);
		char x;
		scanf_s("  %c", &x);
		switch (x)
		{
		case '1':
			insertMenu();
			break;
		case '2':
			findMenu();
			break;
		case '3':
			deleteMenu();
			break;
		case '4':
			//TODO
			break;
		case '0':
			fileClose();
			return;
		default:

			;
		}
	}
}

void findMenu()
{

}

void deleteMenu()
{

}

void insertMenu()
{
	system("CLS");
	printf("1. Generuj rekord\n");
	printf("2. Podaj rekord\n");

	char buf[30] = { 0 };
	char x;
	fseek(stdin, 0, SEEK_END);
	scanf_s("  %c", &x);
	switch (x)
	{
	case '1':
		insert(randomRecord());
		break;
	case '2':
		fgets(buf, 30, stdin);
		insert(buf);
		break;
	default:
		break;
	}
}