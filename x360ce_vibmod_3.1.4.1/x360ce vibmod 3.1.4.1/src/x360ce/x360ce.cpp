#include "stdafx.h"
#include "globals.h"
#include "utils.h"
#include "config.h"
#include "directinput.h"


HWND hWnd = NULL;
BOOL bEnabled = FALSE;
BOOL UseEnabled= FALSE;

VOID LoadOriginalDll(VOID)
{

	TCHAR buffer[MAX_PATH];

	// Getting path to system dir and to xinput1_3.dll
	GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	_tcscat_s(buffer,sizeof(buffer),_T("\\xinput1_3.dll"));

	// try to load the system's dinput.dll, if pointer empty
	if (!hNativeInstance) hNativeInstance = ::LoadLibrary(buffer);

	// Debug
	if (!hNativeInstance)
	{
		ExitProcess(0); // exit the hard way
	}
}

HRESULT XInit(DWORD dwUserIndex){

	HRESULT hr=ERROR_DEVICE_NOT_CONNECTED;
	if(Gamepad[dwUserIndex].id == -1) return hr;

	if(!bEnabled && UseEnabled) return S_FALSE;

	if(
		ConfigPadCount() > EnumPadCount() 
		&& Gamepad[dwUserIndex].g_pGamepad == NULL
		)
	{ 

		WriteLog(_T("Initializing Gamepad %d"),dwUserIndex+1);

		if(bInitBeep)
		{
			MessageBeep(MB_OK);
			bInitBeep=0;
		}

		WriteLog(_T("Configured devices: %d, Enumerated: %d"),ConfigPadCount(),EnumPadCount());
		hr = Enumerate(dwUserIndex); //enumerate when is more configured devices than created
		if(SUCCEEDED(hr))
		{
			WriteLog(_T("Configured devices: %d, Enumerated: %d"),ConfigPadCount(),EnumPadCount());
			WriteLog(_T("Enumeration finished"));
		}
		if(FAILED(hr)) return ERROR_DEVICE_NOT_CONNECTED;

		hr = InitDirectInput(hWnd,dwUserIndex);
		if(FAILED(hr))
		{
			WriteLog(_T("InitDirectInput fail (1) with %s"),DXErrStr(hr));
		}

	}
	else return ERROR_DEVICE_NOT_CONNECTED;

	return S_OK;
}

extern "C" DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	//WriteLog(_T("XInputGetState"));
	if(Gamepad[dwUserIndex].native == 1) 
	{
		if(!hNativeInstance) LoadOriginalDll();
		typedef DWORD (WINAPI* XInputGetState_Type)(DWORD dwUserIndex, XINPUT_STATE* pState);
		XInputGetState_Type nativeXInputGetState = (XInputGetState_Type) GetProcAddress( hNativeInstance, "XInputGetState");
		return nativeXInputGetState(dwUserIndex,pState);
	}

	HRESULT hr=ERROR_DEVICE_NOT_CONNECTED;
	hr = XInit(dwUserIndex);
	if(hr==S_FALSE)return S_OK;

	if(Gamepad[dwUserIndex].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED; //if device is not created by enumeration, return

	GamepadMap PadMap = GamepadMapping[dwUserIndex];

	XINPUT_GAMEPAD &xGamepad = pState->Gamepad;

	xGamepad.wButtons = 0;
	xGamepad.bLeftTrigger = 0;
	xGamepad.bRightTrigger = 0;
	xGamepad.sThumbLX = 0;
	xGamepad.sThumbLY = 0;
	xGamepad.sThumbRX = 0;
	xGamepad.sThumbRY = 0;

	// poll data from device
	hr = UpdateState(dwUserIndex);

	// timestamp packet
	pState->dwPacketNumber=GetTickCount();

	// --- Map buttons ---
	for (INT i = 0; i < 14; ++i) {
		if ((PadMap.Button[i] >= 0)
			&&	ButtonPressed(PadMap.Button[i],dwUserIndex)
			) {
				xGamepad.wButtons |= buttonIDs[i];
		}
	}

	// --- Map POV to the D-pad ---
	if (PadMap.DpadPOV >= 0) {

		INT pov = POVState(PadMap.DpadPOV,dwUserIndex);

		if ((pov == 7) || (pov == 0) || (pov == 1) || (pov == 8)) // Up-left, up, up-right, up (at 360 degrees)
		{
			if(Gamepad[dwUserIndex].povyrev == 1)
			{
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			else xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
		}

		if ((pov >= 1) && (pov <= 3)) // Up-right, right, down-right
		{	
			if(Gamepad[dwUserIndex].povxrev == 1)
			{
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			else xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;	}

		if ((pov >= 3) && (pov <= 5)) // Down-right, down, down-left
		{	
			if(Gamepad[dwUserIndex].povyrev == 1)
			{
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
			}
			else xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;	}

		if ((pov >= 5) && (pov <= 7)) // Down-left, left, up-left
		{	
			if(Gamepad[dwUserIndex].povxrev == 1)
			{
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			else xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;	}

	}

	// Created so we can refer to each axis with an ID
	LONG axis[7] = {
		Gamepad[dwUserIndex].state.lX,
		Gamepad[dwUserIndex].state.lY,
		Gamepad[dwUserIndex].state.lZ,
		Gamepad[dwUserIndex].state.lRx,
		Gamepad[dwUserIndex].state.lRy,
		Gamepad[dwUserIndex].state.lRz,
		0
	};
	LONG slider[2] = {
		Gamepad[dwUserIndex].state.rglSlider[0],
		Gamepad[dwUserIndex].state.rglSlider[1]
	};

	// --- Map triggers ---
	BYTE *targetTrigger[2] = {
		&xGamepad.bLeftTrigger,
		&xGamepad.bRightTrigger
	};

	for (INT i = 0; i < 2; ++i) {

		MappingType triggerType = PadMap.Trigger[i].type;

		if (triggerType == DIGITAL) {
			if (ButtonPressed(PadMap.Trigger[i].id,dwUserIndex))
			{	*(targetTrigger[i]) = 255;		}
		} else {
			LONG *values;
			switch (triggerType) {
				case AXIS:
				case HAXIS:


					values = axis;
					break;


				case SLIDER:
				case HSLIDER:

					values = slider;
					break;
				default:
					values = axis;
					break;
			}

			LONG v = (PadMap.Trigger[i].id > 0 ?
				values[PadMap.Trigger[i].id -1] :
			-values[-PadMap.Trigger[i].id -1] - 1);


			// --- v is the full range (-32768 .. +32767) that should be projected to 0...255

			// --- Full ranges
			// AXIS:	(	0 to 255 from -32768 to 32767) using axis
			// SLIDER:	(	0 to 255 from -32768 to 32767) using slider
			// --
			// --- Half ranges
			// HAXIS:	(	0 to 255 from 0 to 32767) using axis
			// HSLIDER:	(	0 to 255 from 0 to 32767) using slider

			LONG v2=0;
			LONG offset=0;
			LONG scaling=1;


			switch (triggerType) {
				// Full range
				case AXIS:
				case SLIDER:
					scaling = 256; offset = 32768;
					break;
					// Half range
				case HAXIS:
				case HSLIDER:
					scaling = 128; offset = 0;
					break;
				default:
					scaling = 1; offset = 0;
					break;
			}

			v2 = (v + offset) / scaling;

			// Add deadzones
			*(targetTrigger[i]) = (BYTE) deadzone(v2, 0, 255, Gamepad[dwUserIndex].tdeadzone, 255-Gamepad[dwUserIndex].tdeadzone);

		}
	}

	// --- Map thumbsticks ---

	// Created so we can refer to each axis with an ID
	SHORT *targetAxis[4] = {
		&xGamepad.sThumbLX,
		&xGamepad.sThumbLY,
		&xGamepad.sThumbRX,
		&xGamepad.sThumbRY
	};

	// NOTE: Could add symbolic constants as indexers, such as 
	// THUMB_LX_AXIS, THUMB_LX_POSITIVE, THUMB_LX_NEGATIVE
	if(Gamepad[dwUserIndex].axistodpad==0)
	{


		for (INT i = 0; i < 4; ++i) {
			LONG *values;
			// Analog input
			if (PadMap.Axis[i].analogType == AXIS) values = axis;
			if (PadMap.Axis[i].analogType == SLIDER) values = slider;
			if (PadMap.Axis[i].analogType != NONE) {


				if(PadMap.Axis[i].id > 0 )
				{
					*(targetAxis[i]) = (SHORT) values[PadMap.Axis[i].id - 1];
					*(targetAxis[i])= (SHORT) clamp(*(targetAxis[i]),-32768,32767);
				}
				else if(PadMap.Axis[i].id < 0 )
				{
					LONG val = -values[-PadMap.Axis[i].id - 1];

					if(val > 0) val = 32767 * val / 32768;
					else val = 32768 * val / 32767; 
					*(targetAxis[i]) = (SHORT) val;
				}


			}

			// Digital input, positive direction
			if (PadMap.Axis[i].hasDigital && PadMap.Axis[i].positiveButtonID >= 0) {

				if (ButtonPressed(PadMap.Axis[i].positiveButtonID,dwUserIndex))
					*(targetAxis[i]) = 32767;	
			}	
			// Digital input, negative direction
			if (PadMap.Axis[i].hasDigital && PadMap.Axis[i].negativeButtonID >= 0) {

				if (ButtonPressed(PadMap.Axis[i].negativeButtonID,dwUserIndex))
					*(targetAxis[i]) = -32768;
			}	
		}
	}

		//WILDS - Axis to D-Pad
		if(Gamepad[dwUserIndex].axistodpad==1)
		{
			//WriteLog("x: %d, y: %d, z: %d",Gamepad[dwUserIndex].state.lX,Gamepad[dwUserIndex].state.lY,Gamepad[dwUserIndex].state.lZ);

			if(Gamepad[dwUserIndex].state.lX - Gamepad[dwUserIndex].axistodpadoffset > Gamepad[dwUserIndex].axistodpaddeadzone)
				xGamepad.wButtons |= XINPUT_GAMEPAD_LEFT_THUMB;
			if(Gamepad[dwUserIndex].state.lX - Gamepad[dwUserIndex].axistodpadoffset < -Gamepad[dwUserIndex].axistodpaddeadzone)
				xGamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_THUMB;

			if(Gamepad[dwUserIndex].state.lY - Gamepad[dwUserIndex].axistodpadoffset < -Gamepad[dwUserIndex].axistodpaddeadzone)
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
			if(Gamepad[dwUserIndex].state.lY - Gamepad[dwUserIndex].axistodpadoffset > Gamepad[dwUserIndex].axistodpaddeadzone)
				xGamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
		}
		//WILDS END

		// --- Do Linears ---
		// TODO

		return hr;
}

extern "C" DWORD WINAPI XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{

	if(Gamepad[dwUserIndex].native == 1) 
	{
		if(!hNativeInstance) LoadOriginalDll();
		typedef DWORD (WINAPI* XInputSetState_Type)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
		XInputSetState_Type nativeXInputSetState = (XInputSetState_Type) GetProcAddress( hNativeInstance, "XInputSetState");
		return nativeXInputSetState(dwUserIndex,pVibration);
	}

	if (!pVibration || dwUserIndex >= XUSER_MAX_COUNT) return ERROR_BAD_ARGUMENTS; 

	HRESULT hr=ERROR_SUCCESS;
	hr = XInit(dwUserIndex);
	if(hr==S_FALSE)return S_OK;

	if(Gamepad[dwUserIndex].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED;
	if(!Gamepad[dwUserIndex].useforce) return ERROR_SUCCESS;

	WORD wLeftMotorSpeed = 0;
	WORD wRightMotorSpeed = 0;

	//Modified by Racer_S 9/20/2009
	if(Gamepad[dwUserIndex].swapmotor)
	{
		wRightMotorSpeed = (WORD)((float)pVibration->wLeftMotorSpeed * ((float)Gamepad[dwUserIndex].forcepercent * (float)0.01));
		wLeftMotorSpeed =  (WORD)((float)pVibration->wRightMotorSpeed * ((float)Gamepad[dwUserIndex].forcepercent * (float)0.01));
	}
	else
	{

		wLeftMotorSpeed =  (WORD)((float)pVibration->wLeftMotorSpeed * ((float)Gamepad[dwUserIndex].forcepercent * (float)0.01));
		wRightMotorSpeed = (WORD)((float)pVibration->wRightMotorSpeed * ((float)Gamepad[dwUserIndex].forcepercent * (float)0.01));
	}
	//

	if(wLeftMotorSpeed) WriteLog(_T("wLeftMotorSpeed::  %d"),wLeftMotorSpeed);
	if(wRightMotorSpeed) WriteLog(_T("wRightMotorSpeed:: %d"),wRightMotorSpeed);

	//Modified by Racer_S 9/20/2009
	//Safe way to make ffb

	if(wLeftMotorSpeed)
	{
		// wLeftMotorSpeed
		if(NULL == Gamepad[dwUserIndex].g_pEffect[0])hr = PrepareForce(dwUserIndex,0);
		if(FAILED(hr))WriteLog(_T("PrepareForce for pad %d failed with code HR = %s"), dwUserIndex, DXErrStr(hr));

		if(SUCCEEDED(hr))hr = SetDeviceForces(dwUserIndex,wLeftMotorSpeed,0);
		if(FAILED(hr))WriteLog(_T("SetDeviceForces for pad %d failed with code HR = %s"), dwUserIndex, DXErrStr(hr));

	}

	if(wRightMotorSpeed)
	{

		// wRightMotorSpeed
		if(NULL == Gamepad[dwUserIndex].g_pEffect[1]) hr = PrepareForce(dwUserIndex,1);
		if(FAILED(hr))WriteLog(_T("PrepareForce for pad %d failed with code HR = %s"), dwUserIndex, DXErrStr(hr));

		if(SUCCEEDED(hr))hr = SetDeviceForces(dwUserIndex,wRightMotorSpeed,1);
		if(FAILED(hr))WriteLog(_T("SetDeviceForces for pad %d failed with code HR = %s"), dwUserIndex, DXErrStr(hr));
		//
	}

	return ERROR_SUCCESS;
}

extern "C" DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
{
	if(Gamepad[dwUserIndex].native == 1) 
	{
		if(!hNativeInstance) LoadOriginalDll();
		typedef DWORD (WINAPI* XInputGetCapabilities_Type)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
		XInputGetCapabilities_Type nativeXInputGetCapabilities = (XInputGetCapabilities_Type) GetProcAddress( hNativeInstance, "XInputGetCapabilities");
		return nativeXInputGetCapabilities(dwUserIndex,dwFlags,pCapabilities);
	}

	if (!pCapabilities || dwUserIndex >= XUSER_MAX_COUNT) return ERROR_BAD_ARGUMENTS; 

	if(Gamepad[dwUserIndex].id == -1) return ERROR_DEVICE_NOT_CONNECTED;

	// Report support for full range and all buttons
	XINPUT_GAMEPAD xGamepad;
	xGamepad.bLeftTrigger = (BYTE)0xFF;
	xGamepad.bRightTrigger = (BYTE)0xFF;
	xGamepad.sThumbLX = (SHORT)0xFFFF;
	xGamepad.sThumbLY = (SHORT)0xFFFF;
	xGamepad.sThumbRX = (SHORT)0xFFFF;
	xGamepad.sThumbRY = (SHORT)0xFFFF;
	xGamepad.wButtons = (WORD)0xFFFF;

	XINPUT_VIBRATION Vibration = {(WORD)0xFFFF,(WORD)0xFFFF};

	pCapabilities->Flags = (WORD)0;
	pCapabilities->SubType=(BYTE)Gamepad[dwUserIndex].gamepadtype;
	pCapabilities->Gamepad = xGamepad;
	pCapabilities->Vibration = Vibration;
	pCapabilities->Type = (BYTE)XINPUT_DEVTYPE_GAMEPAD;

	WriteLog(_T("XInputGetCapabilities send type %i"),(BYTE)Gamepad[dwUserIndex].gamepadtype);

	return ERROR_SUCCESS;
}

extern "C" VOID WINAPI XInputEnable(BOOL enable)
{
	bEnabled = enable;
	UseEnabled = TRUE;
}

extern "C" DWORD WINAPI XInputGetDSoundAudioDeviceGuids
(
 DWORD dwUserIndex,          // [in] Index of the gamer associated with the device
 GUID* pDSoundRenderGuid,    // [out] DSound device ID for render
 GUID* pDSoundCaptureGuid    // [out] DSound device ID for capture
 )
{
	if(Gamepad[dwUserIndex].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED;
	return ERROR_SUCCESS;
}

extern "C" DWORD WINAPI XInputGetBatteryInformation
(
 DWORD                       dwUserIndex,        // [in]  Index of the gamer associated with the device
 BYTE                        devType,            // [in]  Which device on this user index
 XINPUT_BATTERY_INFORMATION* pBatteryInformation // [out] Contains the level and types of batteries
 )

{
	if(Gamepad[dwUserIndex].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED;

	// Report a wired controller
	pBatteryInformation->BatteryType = BATTERY_TYPE_WIRED;
	return ERROR_SUCCESS;

}

extern "C" DWORD WINAPI XInputGetKeystroke
(
 DWORD dwUserIndex,              // [in]  Index of the gamer associated with the device
 DWORD dwReserved,               // [in]  Reserved for future use
 PXINPUT_KEYSTROKE pKeystroke    // [out] Pointer to an XINPUT_KEYSTROKE structure that receives an input event.
 )
{

	if(Gamepad[dwUserIndex].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED;

	pKeystroke->Flags = NULL;
	pKeystroke->HidCode = NULL;
	pKeystroke->Unicode = NULL;
	pKeystroke->UserIndex = NULL;
	dwReserved=NULL;

	return ERROR_SUCCESS;
}