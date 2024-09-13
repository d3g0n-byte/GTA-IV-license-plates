#include "hooks.h"
#include "CBaseVeh.h"
#include <Windows.h>
#include"timer.h"
#include "hooks.h"

namespace engine_crankshaft {
	void process(CExtVeh* veh) {
		veh->m_fCrankshaftRot += *(float*)((BYTE*)veh + 0x260 + 0x418 + 0x10) * 75.39828f * *timer::g_pTimestep;

		while (veh->m_fCrankshaftRot > 3.14159f) veh->m_fCrankshaftRot -= 6.28319f; // 6.28319
	}

	void create(CExtVeh *veh) {
		veh->m_fCrankshaftRot = 0;
	}


	void init() {
		hooks::CAutomobile__processControl::add((void(*)(CVehicle*))process);
		hooks::CVehicle__CVehicle::add((void(*)(CVehicle*))create);
	}

}