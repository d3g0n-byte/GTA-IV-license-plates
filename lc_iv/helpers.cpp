#include "helpers.h"
#include "timer.h"

#include "Hooking.Patterns-master/Hooking.Patterns.h"
size_t g_atStringHash_addr = (size_t)hook::pattern("8B 4C 24 08 56 8B 74 24 08 80 3E 22 0F 94 C2 84 D2 74 03 83 C6 01 8A 06 84 C0 74 3C 53 8D 49 00 84 D2").get_first();

DWORD atStringHash(const char* pszString, DWORD dwExistingHash) {
	return ((DWORD(__cdecl*)(const char*, DWORD))g_atStringHash_addr)(pszString, dwExistingHash);
}

float smoothValue(float value, float prevFrameValue, float smoothFactor) {

	if (value > prevFrameValue) {
		if (value > prevFrameValue + (smoothFactor * *timer::g_pTimestep))
			value = prevFrameValue + (smoothFactor * *timer::g_pTimestep);
	}
	else if (value < prevFrameValue) {
		if (value < prevFrameValue - (smoothFactor * *timer::g_pTimestep))
			value = prevFrameValue - (smoothFactor * *timer::g_pTimestep);
	}
	return value;
}