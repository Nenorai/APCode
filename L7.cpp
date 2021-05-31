Main.cpp
#define _CRT_SECURE_NO_WARNINGS 

#include "Port.h"
#include <locale.h>

int Server(char* path);
void Client();

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	switch (argc)
	{
	case 1:
		Server(argv[0]);
		break;

	default:
		Client();
		break;
	}
	return 0;
}

int Server(char* path) {
	setlocale(LC_ALL, "Russian");
	HANDLE readEnd = CreateEvent(NULL, FALSE, FALSE, "readEnd");
	HANDLE writeEnd = CreateEvent(NULL, FALSE, FALSE, "writeEnd");
	HANDLE hExit = CreateEvent(NULL, FALSE, FALSE, "exit");
	HANDLE hWork = CreateEvent(NULL, FALSE, FALSE, "work");

	HANDLE hFile = CreateFile("COM1", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL) {
		printf("Error\n");
		exit(GetLastError());
	}

	STARTUPINFO si = {};
	si.cb = sizeof si;
	PROCESS_INFORMATION pi = {};
	if (!CreateProcess(path, (LPSTR)" COM2", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		printf("Error while creating process!\n");
		return 1;
	}

	puts("=== COM1 - порт ===");
	PORT1(readEnd, writeEnd, hFile, hExit, hWork);

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	CloseHandle(hFile);
}

void Client()
{
	setlocale(LC_ALL, "Russian");
	HANDLE hFile = CreateFile("COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL) {
		printf("Error\n");
		exit(GetLastError());
	}
	HANDLE readEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, "readEnd");
	HANDLE writeEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, "writeEnd");
	HANDLE hExit = OpenEvent(EVENT_ALL_ACCESS, FALSE, "exit");
	HANDLE hWork = OpenEvent(EVENT_ALL_ACCESS, FALSE, "work");

	puts("=== COM2 - порт ===");
	PORT2(readEnd, writeEnd, hFile, hExit, hWork);

	CloseHandle(hFile);
}

Port.cpp
#define _CRT_SECURE_NO_WARNINGS 
#include "Port.h"


int getch_()
{
	return _kbhit() ? _getch() : -1;
}


void PORT1(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
	char buffer[100];
	int size;
	while (true)
	{
		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)
		{
			puts("Ожидание. . .");
			WaitForSingleObject(writeEnd, INFINITE);
			size = read(hFile, buffer);
			printf("Сообщение из порта COM2: ");
			for (int i = 0; i < size; i++) printf("%c", buffer[i]);
			printf("\n");
			SetEvent(readEnd);
		}

		if (getch_() == 13)
		{

			SetEvent(hWork);
			printf("Сообщение: ");
			if (!write(hFile, buffer))
			{
				SetEvent(hExit);
				SetEvent(writeEnd);
				return;
			}
			else
			{
				SetEvent(writeEnd);
				WaitForSingleObject(readEnd, INFINITE);
				puts("Доставлено!");
			}
		}
	}
}


void PORT2(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
	char buffer[100];
	int size;

	while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
	{
		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT) //if works
		{
			puts("Ожидание. . .");
			WaitForSingleObject(writeEnd, INFINITE);
			if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT) return;

			size = read(hFile, buffer);
			printf("Сообщение из порта COM1: ");
			for (int i = 0; i < size; i++) printf("%c", buffer[i]);
			printf("\n");
			SetEvent(readEnd);

		}

		if (getch_() == 13)
		{
			SetEvent(hWork);
			printf("Сообщение: ");
			write(hFile, buffer);
			SetEvent(writeEnd);
			WaitForSingleObject(readEnd, INFINITE);
			puts("Доставлено!");
		}

	}
}



int read(HANDLE hFile, char* buffer)
{
	int size;
	DWORD numberOfBytesRead;

	ReadFile(hFile, &size, 1 * sizeof(int), &numberOfBytesRead, NULL);	//read size
	ReadFile(hFile, buffer, size * sizeof(char), &numberOfBytesRead, NULL);	//read info

	return size;
}


bool write(HANDLE hFile, char* buffer)
{
	char symb;
	int i = 0;
	DWORD numberOfBytesWrite;

	while (true)
	{
		scanf("%c", &symb);
		if (symb == '\n')
		{
			buffer[i] = '\0';
			WriteFile(hFile, &i, 1 * sizeof(int), &numberOfBytesWrite, NULL);	//write i(size)
			WriteFile(hFile, buffer, i * sizeof(char), &numberOfBytesWrite, NULL);	//write info

			if (!strcmp(buffer, "Выход\0")) {
				return false;
			}
			return true;

		}
		else {
			buffer[i] = symb;
		}
		i++;
	}
}
