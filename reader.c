/*++

This program is simply a place to test things.
--*/


#include "dbgzero.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "common.h"
#include "usrlib.h"

#define BUF_SIZE 256
//char sharedmem_fn[] = "mysharedmem.txt";
TCHAR sharedmem_fn[]=TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[]=TEXT("Message from first process.");

my_main(argc,argv[]) {

    MYHANDLE            hndFile;        // Handle to device, obtain from CreateFile
	int status;
	sem_t *hSemaphore;	// In regular windows, this type is HANDLE, but I believe that translates to void *. 
	unsigned char pchID[10];
	int iSleepTime;
	BOOL bIsCreator = TRUE;
	int i, rc;
	ULONG major_version_number;
	ULONG minor_version_number;
	int hang = 0;


	HANDLE hMapFile;
	LPCTSTR pBuf;


   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   sharedmem_fn);               // name of mapping object 



	if (NULL == hMapFile) {
		status = my_get_last_error();
		myprint("OpenFileMapping failed.  my_get_last_error is %ld\n",status);
		my_exit(4);
	}




	pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,                   
                        0,                   
							BUF_SIZE);           
	if (pBuf == NULL) { 
		myprint("Could not map view of file (%d).\n", GetLastError()); 
		CloseHandle(hMapFile);
		return 1;
	}

   
	myprint("%s",pBuf);
	_getch();

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	myprint("Normal exit \n");

#ifdef WIN32
    myprint("\n *** Hit enter to continue\n");
	getchar();
#endif
	return 0;
}




