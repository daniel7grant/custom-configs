#pragma once

#define arrayof(x) (sizeof(x)/sizeof(x[0])) 

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif

#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WINVISTA
#endif

#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800

// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <psapi.h>

#include <time.h>
#include <fstream>

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif
