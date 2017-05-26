#include "stdafx.h"
#include "globals.h"
#include "utils.h"
#include "config.h"
#include "directinput.h"

HINSTANCE hx360ceInstance = NULL;
HINSTANCE hNativeInstance = NULL;

DWORD dwAppPID = NULL;

extern void FakeWMI();
extern void FakeDInput();
extern void FakeThread(HANDLE);

extern WORD wFakeAPI;

BOOL RegisterWindowClass(HINSTANCE hinstance) 
{ 
	WNDCLASSEX wcx; 

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW | 
		CS_VREDRAW;                    // redraw if size changes 
	wcx.lpfnWndProc = DefWindowProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = hinstance;         // handle to instance 
	wcx.lpszClassName = _T("x360ce_wc");  // name of window class 

	// Register the window class. 

	return RegisterClassEx(&wcx); 
} 

VOID InitInstance(HMODULE hModule) 
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	hx360ceInstance = (HINSTANCE) hModule;

	DisableThreadLibraryCalls(hx360ceInstance);

	RegisterWindowClass(hx360ceInstance);

	hWnd = CreateWindow( 
		_T("x360ce_wc"),     // name of window class //simple MDIClient class
		_T("x360ce"),        // title-bar string 
		WS_OVERLAPPEDWINDOW, // top-level window 
		CW_USEDEFAULT,       // default horizontal position 
		CW_USEDEFAULT,       // default vertical position 
		CW_USEDEFAULT,       // default width 
		CW_USEDEFAULT,       // default height 
		(HWND) NULL,         // no owner window 
		(HMENU) NULL,        // use class menu 
		hx360ceInstance,      // handle to application instance 
		(LPVOID) NULL);      // no window-creation data 

	if(!hWnd)
	{
		WriteLog(_T("CRITICAL:: Window not created"));
	}

	dwAppPID=GetCurrentProcessId();
	

	InitConfig();
	WriteLog(_T("x360ce started by process %s PID %d"),PIDName(dwAppPID),dwAppPID);

	//currentthread = GetCurrentThread();
	//FakeThread(currentthread);

	if(wFakeAPI)
	{
		FakeWMI();
		FakeDInput();
	}


#if 0
	ReadPatchData();
	if(patch == 1)
	{
		WriteLog(_T("Process patching enabled"));
		if(spatch)
			MakeUndo(rvaoff,pundo,psize);
		Patch(rvaoff,pdata,psize);
	}
#endif


}

VOID ExitInstance() 
{    
	delete[] logfilename;
	for(INT i=0;i<EnumPadCount();i++)
	{
		Gamepad[i].g_pGamepad = NULL;
	}
	g_pDI = NULL;

	if (hNativeInstance)
	{
		::FreeLibrary(hNativeInstance);
		hNativeInstance = NULL;  
		CloseHandle(hNativeInstance);
	}

	CloseHandle(hWnd);
	CloseHandle(hx360ceInstance);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	if (dwReason == DLL_PROCESS_ATTACH ) 
	{
		InitInstance(hinst);
	}

	else if (dwReason == DLL_PROCESS_DETACH) 
	{
		WriteLog(_T("DLL_PROCESS_DETACH"));
		if(reserved != NULL) //process termination
		{
			ExitInstance();
		}

	}
	return TRUE;
}

HMODULE WINAPI Detoured()
{
	return (HMODULE) hx360ceInstance;
}