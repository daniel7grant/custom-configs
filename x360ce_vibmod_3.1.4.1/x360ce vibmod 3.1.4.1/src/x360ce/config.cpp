#include "stdafx.h"
#include "globals.h"
#include "utils.h"
#include "config.h"
#include "directinput.h"

#include <vector>
#include <ios>
#include <string>
#include <sstream>

WORD wFakeAPI=0;
BOOL bInitBeep=0;

LPCTSTR buttonNames[14] = {
	_T("A"),
	_T("B"),
	_T("X"),
	_T("Y"),
	_T("Left Shoulder"),
	_T("Right Shoulder"),
	_T("Back"),
	_T("Start"),
	_T("Left Thumb"),
	_T("Right Thumb"),
	_T("D-pad Up"),
	_T("D-pad Down"),
	_T("D-pad Left"),
	_T("D-pad Right")
};

LPCTSTR axisNames[4] = {
	_T("Left Analog X"),
	_T("Left Analog Y"),
	_T("Right Analog X"),
	_T("Right Analog Y")
};
LPCTSTR axisLNames[4] = {
	_T("Left Analog X Linear"),
	_T("Left Analog Y Linear"),
	_T("Right Analog X Linear"),
	_T("Right Analog Y Linear")

};
LPCTSTR axisBNames[8] = {
	_T("Left Analog X+ Button"),
	_T("Left Analog X- Button"),
	_T("Left Analog Y+ Button"),
	_T("Left Analog Y- Button"),
	_T("Right Analog X+ Button"),
	_T("Right Analog X- Button"),
	_T("Right Analog Y+ Button"),
	_T("Right Analog Y- Button")
};
LPCTSTR padNames[4] = {
	_T("PAD1"),
	_T("PAD2"),
	_T("PAD3"),
	_T("PAD4"),
};

GamepadMap GamepadMapping[4];

VOID InitConfig() {

	LPTSTR pStr;
	TCHAR strPath[MAX_PATH];
	extern TCHAR tstrConfigFile[MAX_PATH];	
	GetModuleFileName (NULL, strPath, MAX_PATH);
	pStr = _tcsrchr(strPath, _T('\\'));
	if (pStr != NULL)
		*(++pStr)=_T('\0'); 

	TCHAR buffer[MAX_PATH];

	_stprintf_s(tstrConfigFile,_T("%s%s"),strPath, _T("x360ce.ini"));

	// Read global options
	ReadFromFile(_T("Options"), _T("UseInitBeep"), buffer, _T("1"));	bInitBeep = _tstoi(buffer);
	ReadFromFile(_T("Options"), _T("Log"), buffer, _T("0"));			writelog = _tstoi(buffer);
	ReadFromFile(_T("Options"), _T("FakeAPI"), buffer, _T("0"));		wFakeAPI = _tstoi(buffer);

	// Read pad mappings
	for (INT b = 0; b < 4; b++) 
		ReadPadConfig(b);
}


DWORD hexToDword(LPTSTR buf)
{
	long v = wcstoul(buf, NULL, 16);
	return v;
}

#if 0
void string_to_bytearray(std::string str, BYTE* &array, INT& size)
{
	int length = str.length();
	// make sure the input string has an even digit numbers
	if(length%2 == 1)
	{
		str = "0" + str;
		length++;
	}

	// allocate memory for the output array
	array = new BYTE[length/2+1];
	size = length/2;

	std::stringstream sstr(str);
	for(INT i=0; i < size; i++) { 
		CHAR ch1, ch2; 
		UINT a;
		sstr >> ch1 >> ch2; 
		sscanf_s(&ch2, "%01x", &a);
		array[i] = (BYTE)a;
		sscanf_s(&ch1, "%01x", &a); 
		array[i] |= (BYTE)(a << 4); 
	} 
}

VOID ReadPatchData() {

	TCHAR section[20];
	_stprintf_s(section,_T("Patch"));

	TCHAR buffer[MAX_PATH];

	size_t   i;
	char* tmp=new char[MAX_PATH];
	std::string str;

	ReadFromFile(section, _T("Enabled"), buffer, _T("0"));patch = _tstoi(buffer);
	ReadFromFile(section, _T("Stealth"), buffer, _T("1"));					spatch = _tstoi(buffer);
	ReadFromFile(section, _T("RVA"), buffer);								rvaoff = hexToDword(buffer);

	ReadFromFile(section, _T("Data"), buffer);
	wcstombs_s(&i,tmp,(size_t)MAX_PATH,buffer,(size_t)MAX_PATH);
	str = tmp;
	string_to_bytearray(str,pdata,psize);

}
#endif


VOID ReadPadConfig(INT idx) {

	TCHAR section[20];
	_stprintf_s(section,_T("PAD%d"),idx+1);
	GamepadMap &PadMap = GamepadMapping[idx];

	TCHAR buffer[MAX_PATH];

	ReadFromFile(section, _T("Index"), buffer, _T("-1"));					Gamepad[idx].id = _ttoi(buffer);
	if (Gamepad[idx].id < 0) { return; } else { PadMap.enabled = true; }

	//TODO add this to Gamepad struct
	ReadFromFile(section, _T("VID"), buffer, _T("0"));						Gamepad[idx].vid = hexToDword(buffer);
	ReadFromFile(section, _T("PID"), buffer, _T("0"));						Gamepad[idx].pid = hexToDword(buffer);
	ReadFromFile(section, _T("SwapMotor"), buffer, _T("0"));				Gamepad[idx].swapmotor = _ttoi(buffer);
	ReadFromFile(section, _T("Native"), buffer, _T("0"));					Gamepad[idx].native = _ttoi(buffer);
	ReadFromFile(section, _T("TriggerDeadzone"), buffer, _T("0"));			Gamepad[idx].tdeadzone = _ttoi(buffer);
	ReadFromFile(section, _T("UseForceFeedback"), buffer, _T("0"));			Gamepad[idx].useforce = _ttoi(buffer);
	ReadFromFile(section, _T("ForcePercent"), buffer, _T("100"));			Gamepad[idx].forcepercent = _ttoi(buffer);
	ReadFromFile(section, _T("POVXReverse"), buffer, _T("0"));				Gamepad[idx].povxrev = _ttoi(buffer);
	ReadFromFile(section, _T("POVYReverse"), buffer, _T("0"));				Gamepad[idx].povyrev = _ttoi(buffer);
	ReadFromFile(section, _T("ControllerType"), buffer, _T("1"));			Gamepad[idx].gamepadtype = _ttoi(buffer);	//WILDS
	ReadFromFile(section, _T("AxisToDPad"), buffer, _T("0"));				Gamepad[idx].axistodpad = _ttoi(buffer);			//WILDS
	ReadFromFile(section, _T("AxisToDPadDeadZone"), buffer, _T("256"));		Gamepad[idx].axistodpaddeadzone = _ttoi(buffer);	//WILDS
	ReadFromFile(section, _T("AxisToDPadOffset"), buffer, _T("0"));			Gamepad[idx].axistodpadoffset = _ttoi(buffer);	//WILDS

	for (INT i = 0; i < 14; ++i) PadMap.Button[i] = -1;
	for (INT i = 0; i < 2; ++i) PadMap.Trigger[i].type = NONE;
	PadMap.DpadPOV = -1;

	// All values start from 1 in the INI file

	for (INT i=0; i<14; ++i) {
		if (ReadFromFile(section, buttonNames[i], buffer) > 0) {
			INT val = _tstoi(buffer);
			PadMap.Button[i] = val - 1;
		}
	}

	for (INT i=0; i<4; ++i) {
		if (ReadFromFile(section, axisNames[i], buffer) > 0) {
			LPTSTR a = buffer;
			if (_totlower(*a) == 's') { // Slider
				PadMap.Axis[i].analogType = SLIDER;
				++a;
				PadMap.Axis[i].id = _ttoi(a);
			}else{
				// Axis
				PadMap.Axis[i].analogType = AXIS;
				PadMap.Axis[i].id = _ttoi(a);
			}
		}
	}

	for (INT i=0; i<4; ++i) {
		ReadFromFile(section, axisLNames[i], buffer, _T("0"));
		Gamepad[idx].axislinear[i] = _ttoi(buffer);
	}
	for (INT i=0; i<4; ++i) {
		if (ReadFromFile(section, axisBNames[i*2], buffer) > 0) {
			PadMap.Axis[i].hasDigital = true;
			PadMap.Axis[i].positiveButtonID = _ttoi(buffer) - 1;
		}
		if (ReadFromFile(section, axisBNames[i*2+1], buffer) > 0) {
			PadMap.Axis[i].hasDigital = true;
			PadMap.Axis[i].negativeButtonID = _ttoi(buffer) - 1;
		}
	}
	if (ReadFromFile(section, _T("Left Trigger"), buffer) > 0) {
		LPTSTR a = buffer;
		if ((PadMap.Trigger[0].type = getTriggerType(a)) == DIGITAL) {
			PadMap.Trigger[0].id = _ttoi(a) - 1;
		} else {
			++a;
			PadMap.Trigger[0].id = _ttoi(a);
		}
	}

	if (ReadFromFile(section, _T("Right Trigger"), buffer) > 0) {
		LPTSTR a = buffer;
		if ((PadMap.Trigger[1].type = getTriggerType(a)) == DIGITAL) {
			PadMap.Trigger[1].id = _ttoi(a) - 1;
		} else {
			++a;
			PadMap.Trigger[1].id = _ttoi(a);
		}
	}

	if (ReadFromFile(section, _T("D-pad POV"), buffer) > 0) {
		INT val = _tstoi(buffer);
		PadMap.DpadPOV = val - 1;
	}
}

// NOTE: Letters corresponding to mapping types changed. Include in update notes.
MappingType getTriggerType(LPCTSTR s) {
	if (_totlower(*s) == 'a') return AXIS;	// Axis
	if (_totlower(*s) == 's') return SLIDER;	// Slider
	if (_totlower(*s) == 'x') return HAXIS;	// Half range axis
	if (_totlower(*s) == 'h') return HSLIDER;	// Half range slider
	return DIGITAL;							// Digital
}
