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
const char sharedmem_fn[] = "mysharedmem.txt";
//TCHAR sharedmem_fn[]=TEXT("Global\\MyFileMappingObject");  // Case sensitive file name
const char szMsg[]="Message from first process.";
typedef enum { READER, WRITER } t_mode;

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
	t_mode mode;


	HANDLE hMapFile;
	LPCTSTR pBuf;


	/* determine if this is a reader or a writer invocation. */
	/* make sre we have at least one argument for the mode. */
	if (argc < 2) {
		myprint("USAGE: %s <reader|writer>\n",argv[0]);
		myprint("USAGE: %s writer & sleep 1; %s reader \n",argv[0], argv[0]);
		my_exit(1);
	}

	/* Set mode based upon command line parameter. */
	if (strcmp(argv[1],"reader") == 0) {
		mode = READER;
	} else {
		mode = WRITER;
	}



	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD) 
                 BUF_SIZE,                // maximum object size (low-order DWORD)  
                 sharedmem_fn);            // name of mapping object
 

	/*
	 * a reader could do this as well
   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   sharedmem_fn);               // name of mapping object 
	*
	*/


	if (NULL == hMapFile) {
		status = my_get_last_error();
		myprint("CreateFileMapping failed.  my_get_last_error is %ld\n",status);
		my_exit(4);
	}

	status = my_get_last_error();
	if (ERROR_ALREADY_EXISTS == status) {
		myprint("SharedMapping already exists. CreateFileMapping error ignored. \n");
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

	if (mode == WRITER) {
		CopyMemory((PVOID)pBuf, szMsg, 5);
	} else {
		myprint("buf is %s\n",pBuf);
	}
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




