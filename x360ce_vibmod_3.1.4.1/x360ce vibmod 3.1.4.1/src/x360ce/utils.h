

INT WriteToFile(LPTSTR, LPTSTR, LPTSTR);
INT ReadFromFile(LPCTSTR strFileSection, LPCTSTR strKey, LPTSTR strOutput);
INT ReadFromFile(LPCTSTR strFileSection, LPCTSTR strKey, LPTSTR strOutput, LPTSTR strDefault);
HWND GetWindowHandle(DWORD);
LPTSTR const DXErrStr(HRESULT dierr);
INT WriteLog(LPCTSTR,...);
LONG clamp(LONG val, LONG min, LONG max);
LONG deadzone(LONG val, LONG min, LONG max, LONG lowerDZ, LONG upperDZ);


// externs

extern TCHAR tstrConfigFile[MAX_PATH];
extern BOOL writelog;
extern BOOL logready; 
extern LPTSTR logfilename;