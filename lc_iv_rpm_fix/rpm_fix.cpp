// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "..\lc_iv\CBaseVeh.h"
#include "..\lc_iv\helpers.h"
#include "..\lc_iv\hooks.h"
#include <cmath>
#include "..\lc_iv\timer.h"
#include "..\lc_iv\Hooking.Patterns-master\Hooking.Patterns.h"
#include "..\lc_iv\injector\injector.hpp"

void process_engine_rpm(CExtVeh* veh) {

	if (!(*((BYTE*)veh + 0xf64) & 0x8)) { // engine off
		float* pRevs = (float*)((BYTE*)veh + 0x10e0 + 4);
		float* pRevsOld = (float*)((BYTE*)veh + 0x10e0 + 8);

		float revsOld = *pRevs; // save old rpm
		if (!(*((BYTE*)veh + 0xf64) & 0x10)) { // when engine off
			if (*pRevs > 0.0f)
				*pRevs = smoothValue(0.f, *pRevsOld, 2);
		}
		else { // when engine starting
			//veh->m_transmission.m_fRevs = veh->m_transmission.m_fRevs;
			//if (veh->m_transmission.m_fRevs < 0.15f) {
			*pRevs += (0.15f + fabsf(sinf((veh->m_fCrankshaftRot * 0.5f)) * 2.5f) * 0.2f) * *timer::g_pTimestep;
			*pRevs -= *pRevs * 0.1f;
		}
		*pRevsOld = revsOld;
	}
}

void patchIdleRpm() {
	float* pNewIdleRpm = new float(0.1f);
	hook::pattern p("F3 0F 10 15 ? ? ? ? F3 0F 59 C5 0F 2F D0 76 03 0F 28 C2 F3 0F 10 56 ? 0F 2F C2 76 03 0F 28 C2 F3 0F 11 46 ?");
	if (!p.empty()) {
		size_t addr = (size_t)p.get_first(4);
		injector::WriteMemory(addr, pNewIdleRpm);
	}
	else MessageBoxA(NULL, "[lc_iv_rpm_fix] patchIdleRpm(...): unsupported version", NULL, 0x10);

	hook::pattern p2("F3 0F 10 05 ? ? ? ? 0F 2F C1 76 03 0F 28 C8 F3 0F 10 56 ? 0F 2F CA 76 18 F3 0F 10 40 ?");
	if (!p2.empty()) {
		size_t addr = (size_t)p2.get_first(4);
		injector::WriteMemory(addr, pNewIdleRpm);
	}
	else MessageBoxA(NULL, "[lc_iv_rpm_fix] patchIdleRpm(...), 2: unsupported version", NULL, 0x10);
	
}

namespace hooks {
	namespace CAutomobile__processControl {
		void hook();
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//hooks::init();
		hooks::CAutomobile__processControl::hook(); // только этот хук. Основной скрипт меняет не только адрес вызова а и другие опкоды и этот просто не найдет и покажет ошибку
		patchIdleRpm();
		hooks::CAutomobile__processControl::add((void(*)(CVehicle*))process_engine_rpm);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

