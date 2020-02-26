/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #2, Windows
 *
 * Implementing grep
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>
#include <crc32.h>

#define BUFSIZE 512
#define CHUNKSIZE 32

static void GenerateCrc(CHAR *sourceFile, CHAR *destFile)
{
	HANDLE hRead, hWrite;
	CHAR buf[BUFSIZE];
	BOOL bRet;
	DWORD bytesRead, bytesWritten;
	int crc = 0;

	/* TODO 1 - Open source file for reading */
	hRead = CreateFile(sourceFile,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hRead == INVALID_HANDLE_VALUE, "CreateFile");

	/* TODO 1 - Create destination file for writing */
	hWrite = CreateFile(destFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hWrite == INVALID_HANDLE_VALUE, "CreateFile");

	/* read from file  */
	while (1) {

		ZeroMemory(buf, sizeof(buf));

		/* TODO 1 - Read from source file into buf BUFSIZE bytes */

		bRet = ReadFile(hRead,
						buf,
						BUFSIZE,
						&bytesRead,
						NULL);
		DIE(bRet == FALSE, "ReadFile");

		/* TODO 1 - Test for end of file */
		if (bytesRead == 0)
			break;

		/* calculate crc for buf */
		crc = update_crc(crc, (unsigned char *) buf, bytesRead);
	}

	/* TODO 1 - Write crc to destination file */
	bRet = WriteFile(hWrite,
					&crc,
					sizeof(crc),
					&bytesWritten,
					NULL);
	DIE(bRet == FALSE, "WriteFile");

	bRet = CloseHandle(hRead);
	DIE(bRet == FALSE, "CloseHandle");

	bRet = CloseHandle(hWrite);
	DIE(bRet == FALSE, "CloseHandle");
}

static DWORD GetSize(HANDLE file)
{
	DWORD dwSize;

	/* TODO 2 - Calculate and return file size using SetFilePointer */
	dwSize = SetFilePointer(file, 0, NULL, FILE_END);
	DIE(dwSize < 0, "SetFilePointer");

	SetFilePointer(file, 0, NULL, FILE_BEGIN);

	return dwSize;
}

static BOOL CompareFiles(CHAR *file1, CHAR *file2)
{
	DWORD  bytesRead;
	HANDLE hFile1, hFile2;
	CHAR chunk1[CHUNKSIZE], chunk2[CHUNKSIZE];
	BOOL result = FALSE, bRet;

	/* TODO 3 - Open file handles */
	hFile1 = CreateFile(file1,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hFile1 == INVALID_HANDLE_VALUE, "CreateFile");

	hFile2 = CreateFile(file2,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hFile2 == INVALID_HANDLE_VALUE, "CreateFile");

	/* TODO 3 - Compare file size */
	if (GetSize(hFile1) != GetSize(hFile2)) {
		printf("Files are of different sizes\n");
		result = FALSE;
		goto exit;
	}

	/* TODO 3 - Compare the CRC files, chunk by chunk */

	/* Disclaimer: in order for the solution to be easier to
	 * read/understand, we presumed that the ReadFile/WriteFile
	 * operations read/write the
	 * specified number of characters.
	 *
	 * This is not always true and the values returned
	 * (bytesRead/bytesWritten) should always be checked
	 */

	while (1) {
		ZeroMemory(chunk1, sizeof(chunk1));
		ZeroMemory(chunk2, sizeof(chunk2));

		bRet = ReadFile(hFile1,
				chunk1,
				sizeof(chunk1),
				&bytesRead,
				NULL);
		DIE(bRet == FALSE, "ReadFile");

		bRet = ReadFile(hFile2,
				chunk2,
				sizeof(chunk2),
				&bytesRead,
				NULL);
		DIE(bRet == FALSE, "ReadFile");

		if (memcmp(chunk1, chunk2, sizeof(chunk1)) != 0) {
			result = FALSE;
			goto exit;
		}

		/* done reading */
		if (bytesRead == 0) {
			result = TRUE;
			break;
		}
	}

exit:
	/* TODO 3 - Close files */

	bRet = CloseHandle(hFile1);
	DIE(bRet == FALSE, "CloseHandle");

	bRet = CloseHandle(hFile2);
	DIE(bRet == FALSE, "CloseHandle");

	return result;
}

int main(int argc, char *argv[])
{
	BOOL equal;

	if (argc != 4) {
		fprintf(stderr, "Usage:\n"
				"\t crc.exe -g <input_file> <output_file> - "
				"generate crc\n"
				"\t crc.exe -c <file1> <file2>            - "
				"compare files\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "-g") == 0)
		GenerateCrc(argv[2], argv[3]);

	if (strcmp(argv[1], "-c") == 0) {
		equal = CompareFiles(argv[2], argv[3]);

		if (equal)
			printf("Files are equal\n");
		else
			printf("Files differ\n");
	}

	return 0;
}
