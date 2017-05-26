#include "stdafx.h"
#include "utils.h"
#include <detours.h>

#define CINTERFACE
#include <dinput.h>
#include "directinput.h"

extern WORD wFakeAPI;

LPDIENUMDEVICESCALLBACK lpOldCallback= NULL;

BOOL CALLBACK FakeEnumCallback( const DIDEVICEINSTANCE* pInst,VOID* pContext )
{

	if (pContext != NULL && _tcsstr( (TCHAR*)pContext, _T("[x360ce]") ))
	{
		return lpOldCallback(pInst,pContext);
	}

	DIDEVICEINSTANCE pFakeInst;
	if(pInst && pInst->dwSize!=0)
	{
		WriteLog(_T("FakeEnumCallback"));
		GUID fakeguid = pInst->guidProduct;

		if(wFakeAPI>=1)
		{
			for(int i = 0; i < ConfigPadCount(); i++)
			{
				DWORD dwPIDVID = MAKELONG(Gamepad[i].vid,Gamepad[i].pid);
				if (dwPIDVID != 0 && dwPIDVID == fakeguid.Data1)
				{
					memcpy(&pFakeInst,pInst,pInst->dwSize);

					pFakeInst.guidProduct.Data1=0x028E045E;
					pFakeInst.guidProduct.Data2=0x0000;
					pFakeInst.guidProduct.Data3=0x0000;
					BYTE pdata4[8] = {0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44};
					memcpy(&pFakeInst.guidProduct.Data4, pdata4, 8);

					LPOLESTR strOldguidProduct;
					LPOLESTR strNewguidProduct;
					StringFromIID(pInst->guidProduct,&strOldguidProduct);
					StringFromIID(pFakeInst.guidProduct,&strNewguidProduct);
					WriteLog(_T("GUID change from %s to %s"),strOldguidProduct,strNewguidProduct);

					pFakeInst.dwDevType = 66069;
					pFakeInst.wUsage = 5;
					pFakeInst.wUsagePage = 1;

					_stprintf_s(pFakeInst.tszProductName, _T("%s"), _T("XBOX 360 For Windows (Controller)"));
					WriteLog(_T("Product Name change from %s to %s"),pInst->tszProductName,pFakeInst.tszProductName);
					_stprintf_s(pFakeInst.tszInstanceName, _T("%s"), _T("XBOX 360 For Windows (Controller)")); 	
					WriteLog(_T("Instance Name change from %s to %s"),pInst->tszInstanceName,pFakeInst.tszInstanceName);


					return lpOldCallback(&pFakeInst,pContext);
				}
			}

		}
	}

	return lpOldCallback(pInst,pContext);
}


HRESULT (STDMETHODCALLTYPE *OldEnumDevices) (LPDIRECTINPUT8 This, DWORD dwDevType,LPDIENUMDEVICESCALLBACK lpCallback,LPVOID pvRef,DWORD dwFlags) = NULL;

HRESULT STDMETHODCALLTYPE NewEnumDevices (LPDIRECTINPUT8 This, DWORD dwDevType,LPDIENUMDEVICESCALLBACK lpCallback,LPVOID pvRef,DWORD dwFlags)
{
	if (lpCallback != NULL)
	{
		lpOldCallback= lpCallback;
		return OldEnumDevices(This,dwDevType,FakeEnumCallback,pvRef,dwFlags);
	}
	return OldEnumDevices(This,dwDevType,lpCallback,pvRef,dwFlags);
}

HRESULT (STDMETHODCALLTYPE *OldGetDeviceInfo) (LPDIRECTINPUTDEVICE8 This, LPDIDEVICEINSTANCE pdidi) = NULL;

HRESULT STDMETHODCALLTYPE NewGetDeviceInfo (LPDIRECTINPUTDEVICE8 This, LPDIDEVICEINSTANCE pdidi)
{
	HRESULT hr;
	hr = OldGetDeviceInfo ( This, pdidi );
	if(pdidi != NULL)
	{
		WriteLog(_T("NewGetDeviceInfo"));
		if(wFakeAPI>=1)
		{
			for(int i = 0; i < ConfigPadCount(); i++)
			{
				DWORD dwPIDVID = MAKELONG(Gamepad[i].vid,Gamepad[i].pid);
				if(dwPIDVID != 0 && dwPIDVID == pdidi->guidProduct.Data1)
				{
					LPOLESTR strOldguidProduct;
					StringFromIID(pdidi->guidProduct,&strOldguidProduct);

					pdidi->guidProduct.Data1=0x028E045E;
					pdidi->guidProduct.Data2=0x0000;
					pdidi->guidProduct.Data3=0x0000;
					unsigned char pdata4[8] = {0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44};
					memcpy(&(pdidi->guidProduct.Data4), pdata4, 8);

					LPOLESTR strNewguidProduct;
					StringFromIID(pdidi->guidProduct,&strNewguidProduct);
					WriteLog(_T("GUID change from %s to %s"),strOldguidProduct,strNewguidProduct);

					pdidi->dwDevType = 66069;
					pdidi->wUsage = 5;
					pdidi->wUsagePage = 1;
					WriteLog(_T("Product Name change from %s"), pdidi->tszProductName );
					_stprintf_s(pdidi->tszProductName, _T("%s"), _T("XBOX 360 For Windows (Controller)"));
					WriteLog(_T("to %s"), pdidi->tszProductName );
					WriteLog(_T("Instance Name change from %s"), pdidi->tszInstanceName );
					_stprintf_s(pdidi->tszInstanceName, _T("%s"), _T("XBOX 360 For Windows (Controller)"));
					WriteLog(_T("to %s"), pdidi->tszInstanceName );
				}
			}
		}
	}
	return hr;
}

HRESULT (STDMETHODCALLTYPE *OldGetProperty) (LPDIRECTINPUTDEVICE8 This, REFGUID rguidProp, LPDIPROPHEADER pdiph) = NULL;

HRESULT STDMETHODCALLTYPE NewGetProperty (LPDIRECTINPUTDEVICE8 This, REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	HRESULT hr;
	hr = OldGetProperty (This, rguidProp, pdiph);
	WriteLog(_T("NewGetProperty"));

	if ( (&rguidProp==&DIPROP_VIDPID) && (wFakeAPI>=1) )
	{
		WriteLog(_T("Old VIDPID = %08X"),((LPDIPROPDWORD)pdiph)->dwData);
		((LPDIPROPDWORD)pdiph)->dwData = 0x028E045E;
		WriteLog(_T("New VIDPID = %08X"),((LPDIPROPDWORD)pdiph)->dwData);
	}
	if ( (&rguidProp==&DIPROP_PRODUCTNAME) && (wFakeAPI>=1) )
	{
		WriteLog(_T("Old PRODUCTNAME = %s"),((LPDIPROPSTRING)pdiph)->wsz);
		_stprintf_s( ((LPDIPROPSTRING)pdiph)->wsz, _T("%s"), _T("XBOX 360 For Windows (Controller)") );
		WriteLog(_T("New PRODUCTNAME = %s"),((LPDIPROPSTRING)pdiph)->wsz);

	}
	return hr;
}

HRESULT (STDMETHODCALLTYPE *OldCreateDevice) (LPDIRECTINPUT8 This, REFGUID rguid, LPDIRECTINPUTDEVICE8 *lplpDirectInputDevice, LPUNKNOWN pUnkOuter) = NULL;

HRESULT STDMETHODCALLTYPE NewCreateDevice (LPDIRECTINPUT8 This, REFGUID rguid, LPDIRECTINPUTDEVICE8 * lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	HRESULT hr;
	LPDIRECTINPUTDEVICE8 pDID;

	hr = OldCreateDevice (This, rguid, lplpDirectInputDevice, pUnkOuter);
	if(lplpDirectInputDevice != NULL)
	{
		WriteLog(_T("NewCreateDevice"));
		pDID = (LPDIRECTINPUTDEVICE8) *lplpDirectInputDevice;
		if(pDID != NULL)
		{
			if(OldGetDeviceInfo == NULL)
			{
				OldGetDeviceInfo = pDID->lpVtbl->GetDeviceInfo;
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)OldGetDeviceInfo, NewGetDeviceInfo);
				DetourTransactionCommit();
			}
			if(OldGetProperty == NULL)
			{
				OldGetProperty = pDID->lpVtbl->GetProperty;
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)OldGetProperty, NewGetProperty);
				DetourTransactionCommit();
			}
		}
	}
	return hr;
}

HRESULT (WINAPI *OldDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) = DirectInput8Create;

HRESULT WINAPI NewDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{

	HRESULT hr;
	LPDIRECTINPUT8 pDI;

	hr = OldDirectInput8Create(hinst,dwVersion,riidltf,ppvOut,punkOuter);

	if(ppvOut != NULL) 
	{
		WriteLog(_T("NewDirectInput8Create"));

		pDI = (LPDIRECTINPUT8) *ppvOut;
		if(pDI != NULL) 
		{
			if(OldEnumDevices == NULL) 
			{
				OldEnumDevices = pDI->lpVtbl->EnumDevices;

				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)OldEnumDevices, NewEnumDevices);
				DetourTransactionCommit();
			}
			if(OldCreateDevice == NULL)
			{
				OldCreateDevice = pDI->lpVtbl->CreateDevice;

				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)OldCreateDevice, NewCreateDevice);
				DetourTransactionCommit();
			}
		}
	}

	return hr;
}

void FakeDInput()
{
	WriteLog(_T("FakeDInput"));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)OldDirectInput8Create, NewDirectInput8Create);
	DetourTransactionCommit();

}
