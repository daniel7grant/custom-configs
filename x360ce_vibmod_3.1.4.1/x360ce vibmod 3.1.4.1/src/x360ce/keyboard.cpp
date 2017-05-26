#include "stdafx.h"
#include "utils.h"
#include "extern.h"
#include "config.h"

void PressKeyb(BYTE vk_code)
{
	INPUT keyb;
	ZeroMemory(&keyb, sizeof(INPUT));
	keyb.type = INPUT_KEYBOARD;
	keyb.ki.wVk = vk_code;

	SendInput(1, &keyb, sizeof(INPUT));
	keyb.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyb, sizeof(INPUT));
}
