#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

HANDLE create_thread(LPTHREAD_START_ROUTINE func,
	LPVOID             param,
	LPDWORD            pThreadID);

DWORD WINAPI recieving(LPVOID arg);

#endif