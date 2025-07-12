#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

HANDLE create_thread(LPTHREAD_START_ROUTINE func,
	LPVOID             param,
	LPDWORD            pThreadID);

DWORD WINAPI recieving(LPVOID arg);

#ifdef __cplusplus
}
#endif

#endif