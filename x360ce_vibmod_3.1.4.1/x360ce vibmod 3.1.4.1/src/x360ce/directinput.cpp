#include "stdafx.h"
#include "globals.h"
#include "directinput.h"
#include "utils.h"
#include "config.h"

LPDIRECTINPUT8	g_pDI = NULL ;   //only one DirectInput interface is needed
DIGAMEPAD Gamepad[4];	//but we need a 4 gamepads

INT init[4] = {NULL};
INT enumcount=0;
INT axiscount=0;

WORD ConfigPadCount()
{
	INT ret=0;
	for(INT pad=0;pad<4;pad++)
	{
		if(Gamepad[pad].id > -1) ret++;
	}
	return ret;	//returns configured pad count
}

WORD EnumPadCount()
{
	INT ret=0;
	for(INT pad=0;pad<4;pad++)
	{
		if(Gamepad[pad].g_pGamepad != NULL) ret++;
	}
	return ret;	//returns enumerated (created) pad count
}

VOID FreeDirectInput(INT idx)
{
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.

	if( Gamepad[idx].g_pGamepad )
		Gamepad[idx].g_pGamepad->Unacquire();

	// Release any DirectInput objects.
	for(INT i=0;i<2;i++)
		SAFE_RELEASE( Gamepad[idx].g_pEffect[i] );

	SAFE_RELEASE( Gamepad[idx].g_pGamepad );
	SAFE_RELEASE( g_pDI );

	//Added by Racer_S 9/20/2009
	Gamepad[idx].g_pGamepad = NULL;
	g_pDI = NULL;
	delete[] &Gamepad[idx].name;
}

BOOL CALLBACK EnumGamepadsCallback( const DIDEVICEINSTANCE* pInst,
								   VOID* pContext )
{
	LPDIRECTINPUTDEVICE8 pDevice;
	DIGAMEPAD * gp = (DIGAMEPAD*) pContext;

	if(enumcount == gp->id)
	{
		g_pDI->CreateDevice( pInst->guidInstance, &pDevice, NULL );
		if(pDevice)
		{
			enumcount = 0;
			gp->g_pGamepad = pDevice;
			//gp->guid = pInst->guidProduct; 
			_tcscpy_s(gp->name,pInst->tszProductName);
			WriteLog(_T("Device \"%s\" with Index=%d initialized"),gp->name,gp->id);

			//LPOLESTR bbb;
			//StringFromIID(gp->guid,&bbb);
			//WriteLog(_T("GUID %s"),bbb);
		}
		return DIENUM_STOP;
	}
	else
	{
		enumcount++;
		return DIENUM_CONTINUE;
	}
}

BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
{

	DIGAMEPAD * gp = (DIGAMEPAD*) pContext;

	// For axes that are returned, set the DIPROP_RANGE property for the
	// enumerated axis in order to scale min/max values.
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow        = DIPH_BYID; 
		diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin              = -32768;
		diprg.lMax              = +32767; 

		// Set the range for the axis
		if( FAILED( gp->g_pGamepad->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
			return DIENUM_STOP;

	}
	axiscount++;
	return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumFFAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
{
	DWORD* pdwNumForceFeedbackAxis = (DWORD*) pContext;

	if( ((pdidoi->dwFlags && DIDOI_FFACTUATOR) != 0) )
		(*pdwNumForceFeedbackAxis)++;

	return DIENUM_CONTINUE;
}

HRESULT UpdateState(INT idx )
{
	HRESULT hr;

	if( (NULL == Gamepad[idx].g_pGamepad) || (Gamepad[idx].id == -1))
		return S_FALSE;

	// Poll the device to read the current state
	hr = Gamepad[idx].g_pGamepad->Poll();
	if( FAILED( hr ) )
	{
		WriteLog(_T("Failed to poll pad %d, HR = %s"), idx, DXErrStr(hr));

		hr = Gamepad[idx].g_pGamepad->Acquire();
		WriteLog(_T("Trying to reacquire pad %d, HR = %s"), idx, DXErrStr(hr));
		while( (hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED) ) {
			hr = Gamepad[idx].g_pGamepad->Acquire();
		}
		// Activate/deactivate the device depending on success/failure
		//GamepadMapping[idx].enabled = !FAILED(hr);

		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of 
		// switching, so just try again later 
		return S_OK;
	}

	// Get the input's device state
	if( FAILED( hr = Gamepad[idx].g_pGamepad->GetDeviceState( sizeof( DIJOYSTATE2 ), &Gamepad[idx].state ) ) )
		return hr; // The device should have been acquired during the Poll()

	return S_OK;
}

HRESULT Enumerate(DWORD idx)
{
	HRESULT hr;

	if( FAILED( hr = DirectInput8Create( hx360ceInstance, DIRECTINPUT_VERSION,		
		IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
	{
		return hr;
	}

	// Enumerate configured devices
	enumcount=0;// reset counter
	WriteLog(_T("Enumerating device %d"),idx+1);
	if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
		EnumGamepadsCallback, &Gamepad[idx],
		DIEDFL_ATTACHEDONLY ) ) )
	{
		WriteLog(_T("!!! Enumeration of PAD%d FAILED !!!"),idx+1);
		return hr;
	}
	if(Gamepad[idx].g_pGamepad == NULL) WriteLog(_T("!!! Enumeration of PAD%d FAILED !!!"),idx+1);
	return ERROR_SUCCESS;
}

HRESULT InitDirectInput( HWND hDlg, INT idx )
{

	if(Gamepad[idx].g_pGamepad == NULL) return ERROR_DEVICE_NOT_CONNECTED;
	//if(init[idx]==1) return ERROR_SUCCESS;

	//if(enabled == TRUE && mode == TRUE) 
	//{
	//	coopmode=DISCL_FOREGROUND;
	//}
	//else coopmode=DISCL_BACKGROUND;

	DIPROPDWORD dipdw;
	HRESULT hr=S_OK;
	HRESULT coophr=S_OK;

	if( FAILED( hr = Gamepad[idx].g_pGamepad->SetDataFormat( &c_dfDIJoystick2 ) ) )
	{
		WriteLog(_T("SetDataFormat for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
		return hr;
	}

	// Set the cooperative level to let DInput know how this device should
	// interact with the system and with other DInput applications.
	// Exclusive access is required in order to perform force feedback.

	if( FAILED( coophr = Gamepad[idx].g_pGamepad->SetCooperativeLevel( hDlg,
		DISCL_EXCLUSIVE |
		DISCL_BACKGROUND ) ) )
	{
		WriteLog(_T("SetCooperativeLevel (1) for pad %d failed with code HR = %s"), idx, DXErrStr(coophr));
		//return coophr;
	}
	if(coophr!=S_OK) 
	{
		WriteLog(_T("Device not exclusive acquired, disabling ForceFeedback"));
		Gamepad[idx].useforce = 0;
		if( FAILED( coophr = Gamepad[idx].g_pGamepad->SetCooperativeLevel( hDlg,
			DISCL_NONEXCLUSIVE |
			DISCL_BACKGROUND ) ) )
		{
			WriteLog(_T("SetCooperativeLevel (2) for pad %d failed with code HR = %s"), idx, DXErrStr(coophr));
			//return coophr;
		}
	}

	// Since we will be playing force feedback effects, we should disable the
	// auto-centering spring.
	dipdw.diph.dwSize = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = FALSE;

	if( FAILED( hr = Gamepad[idx].g_pGamepad->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
	{
		WriteLog(_T("SetProperty for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
		//return hr;
	}

	if( FAILED( hr = Gamepad[idx].g_pGamepad->EnumObjects( EnumObjectsCallback,
		( VOID* )&Gamepad[idx], DIDFT_AXIS ) ) )
	{
		WriteLog(_T("EnumObjects for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
		//return hr;
	}
	else
	{
		WriteLog(_T("Device has %d axis"),axiscount);
	}
	axiscount=0;

	if( FAILED( hr = Gamepad[idx].g_pGamepad->EnumObjects( EnumFFAxesCallback,
		( VOID* )&Gamepad[idx].g_dwNumForceFeedbackAxis, DIDFT_AXIS ) ) )
	{
		WriteLog(_T("EnumFFAxesCallback for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
		//return hr;
	}

	if( Gamepad[idx].g_dwNumForceFeedbackAxis > 2 )
		Gamepad[idx].g_dwNumForceFeedbackAxis = 2;

	if( FAILED( hr = Gamepad[idx].g_pGamepad->Acquire() ) )
	{
		WriteLog(_T("Acquire for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
		//return hr;
	}

	return coophr;
}

HRESULT SetDeviceForces(INT idx, WORD force, INT type)
{
	// Modifying an effect is basically the same as creating a new one, except
	// you need only specify the parameters you are modifying
	HRESULT hr= S_OK;
	LONG     rglDirection[2] = { 0, 0 };
	DWORD    rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };  // X- and y-axis

	DICONSTANTFORCE cf;

	LONG magnitude = (LONG)(force/256*256-1);

	if(type == 0)
	{
		rglDirection[0] =  1;
		cf.lMagnitude = magnitude;
	}
	if(type == 1)
	{
		rglDirection[1] =  1;
		cf.lMagnitude = magnitude;
	}

	DIEFFECT eff;
	ZeroMemory( &eff, sizeof( eff ) );
	eff.dwSize = sizeof( DIEFFECT );
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.cAxes = Gamepad[idx].g_dwNumForceFeedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	// Now set the new parameters and start the effect immediately.
	hr= Gamepad[idx].g_pEffect[type]->SetParameters( &eff, DIEP_DIRECTION |DIEP_TYPESPECIFICPARAMS |DIEP_START );
	return hr;
}

HRESULT PrepareForce(INT idx, INT effidx)
{
	HRESULT hr= S_FALSE;

	if(effidx == 0)
	{
		// wLeftMotorSpeed

		DWORD    rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };  // X- and y-axis
		LONG rglDirection[2] = { 1, 0 };

		DICONSTANTFORCE cf;
		DIEFFECT eff;

		ZeroMemory( &eff, sizeof( eff ) );
		eff.dwSize = sizeof( DIEFFECT );
		eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration = INFINITE;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.dwTriggerButton = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes = Gamepad[idx].g_dwNumForceFeedbackAxis;
		eff.rgdwAxes = rgdwAxes;
		eff.rglDirection = rglDirection;
		eff.lpEnvelope = 0;
		eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay = 0;

		// Create the prepared effect
		if( FAILED( hr = Gamepad[idx].g_pGamepad->CreateEffect( GUID_ConstantForce  ,
			&eff, &Gamepad[idx].g_pEffect[effidx] , NULL ) ) )
		{
			WriteLog(_T("CreateEffect (first) for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
			return hr;
		}
		if( NULL == Gamepad[idx].g_pEffect[effidx] )
		{
			WriteLog(_T("g_pEffect is NULL!!!!"));
			return E_FAIL;
		}
		return S_OK;
	}
	else if(effidx ==1)
	{
		DWORD    rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };  // X- and y-axis
		LONG rglDirection[2] = { 1, 0 };

		DICONSTANTFORCE cf;
		DIEFFECT eff;

		// wRightMotorSpeed
		ZeroMemory( &eff, sizeof( eff ) );
		eff.dwSize = sizeof( DIEFFECT );
		eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration = INFINITE;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.dwTriggerButton = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes = Gamepad[idx].g_dwNumForceFeedbackAxis;
		eff.rgdwAxes = rgdwAxes;
		eff.rglDirection = rglDirection;
		eff.lpEnvelope = 0;
		eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay = 0;

		// Create the prepared effect
		if( FAILED( hr = Gamepad[idx].g_pGamepad->CreateEffect( GUID_ConstantForce  ,
			&eff, &Gamepad[idx].g_pEffect[effidx] , NULL ) ) )
		{
			WriteLog(_T("CreateEffect (secound) for pad %d failed with code HR = %s"), idx, DXErrStr(hr));
			return hr;
		}
		if( NULL == Gamepad[idx].g_pEffect[effidx] )
			{
				WriteLog(_T("g_pEffect is NULL!!!!"));
				return E_FAIL;
			}
		return S_OK;
	}
	return S_OK;
}

// return buttons state (1 pressed, 0 not pressed)
BOOL ButtonPressed(DWORD buttonidx, INT idx) {

	return (Gamepad[idx].state.rgbButtons[buttonidx] & 0x80) != 0;
}

INT POVState(DWORD buttonidx, INT idx) {
	//if(jyindex[idx] == -1) return false;
	DWORD pov = Gamepad[idx].state.rgdwPOV[buttonidx];
	if (LOWORD(pov) == 0xFFFF) // Centered
		return -1;
	if (pov == 0) // Up
		return 0;
	if ((pov > 4400) && (pov < 4600)) // Up-right
		return 1;
	if ((pov > 8900) && (pov < 9100)) // Right
		return 2;
	if ((pov > 13400) && (pov < 13600)) // Down-right
		return 3;
	if ((pov > 17900) && (pov < 18100)) // Down
		return 4;
	if ((pov > 22400) && (pov < 22600)) // Down-left
		return 5;
	if ((pov > 26900) && (pov < 27100)) // Left
		return 6;
	if ((pov > 31400) && (pov < 31600)) // Up-left
		return 7;
	if ((pov > 35900) && (pov < 36100)) // Up
		return 8;
	// Default
	return -1;
}