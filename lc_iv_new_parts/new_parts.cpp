// dllmain.cpp : Defines the entry point for the DLL application.
#include "Windows.h"

#include "..\lc_iv\modelinfo.h"
#include "..\lc_iv\hooks.h"
#include "..\lc_iv\CBaseVeh.h"
#include "..\lc_iv\timer.h"
#include "..\lc_iv\Hooking.Patterns-master/Hooking.Patterns.h"
#include <stdio.h>

	struct crBoneData {
		char* m_pszName;
		DWORD m_dwFlags;
		crBoneData* m_pNext;
		crBoneData* m_pChild;
		crBoneData* m_pParent;
		WORD m_wIndex;
		WORD m_wId;
		WORD m_wMirrorIndex;
		BYTE m_nbTransFlagsCount;
		BYTE m_nbRotFlagsCount;
		BYTE m_nbScaleFlagsCount;
		BYTE _1d[3];
		BYTE _20[12 * 4 * 4];

	};
	size_t addr_getLocalTransformMatrix = (size_t)hook::pattern("56 8B F1 8B 06 8B 90 A0 00 00 00 57 FF D2 85 C0 74 1C").get_first();
	size_t addr_Matrix34__addToRotFromEulerX = (size_t)hook::pattern("55 8B EC 83 E4 F0 83 EC 0C F3 0F 5A 45 ? 56 8B F1 E8 ? ? ? ? F2 0F 5A C0 F3 0F 11 44 24 ? F3 0F 5A 45 ? E8 ? ? ? ? F3 0F 10 56 ? F3 0F 10 5E").get_first();

	struct matrix {
		struct ls {
			float x, y, z, w;
		};

		ls a;
		ls b;
		ls c;
		ls d;

		void addToRotFromEulerX(float pEulers) {
			((void(__thiscall*)(matrix*, float))addr_Matrix34__addToRotFromEulerX)(this, pEulers);
		}
		void setRotFromEulerX(float pEulers) {
			//pEulers = -pEulers;
			memset(this, 0x0, 0x30);
			a.x = b.y = c.z = 1.f;
			b.y = cosf(pEulers);
			b.z = -sinf(pEulers);
			c.y = sinf(pEulers);
			c.z = cosf(pEulers);

			//a.y = a.z = b.x = b.z = c.x = c.y = 0.f;
			//addToRotFromEulerX(pEulers);
		}
	};


	struct CLocalVeh : CExtVeh {
		matrix* getLocalTransformMatrix(int boneIndex) {
			return ((matrix * (__thiscall*)(CVehicle*, int))addr_getLocalTransformMatrix)(this, boneIndex);
		}

	};

	void processRhinoFakeWheels(CExtVeh* veh) {
		for (BYTE i = 0; i < 16; i++) {
			if (veh->m_newPartsIds.m_nbFakeTankLWheels[i] != 0xff) {
				auto mtx = ((CLocalVeh*)veh)->getLocalTransformMatrix(veh->m_newPartsIds.m_nbFakeTankLWheels[i]);
				/*if(i >=5 && i<=6) mtx->setRotFromEulerX(veh->m_aFakeTrackWheelsRot[0] * 2.f);
				else */mtx->addToRotFromEulerX(veh->m_aTrackSpeed[0] * *timer::g_pTimestep);
			}
		}
		for (BYTE i = 0; i < 16; i++) {
			if (veh->m_newPartsIds.m_nbFakeTankRWheels[i] != 0xff) {
				auto mtx = ((CLocalVeh*)veh)->getLocalTransformMatrix(veh->m_newPartsIds.m_nbFakeTankRWheels[i]);
				//mtx->setRotFromEulerX(veh->m_aFakeTrackWheelsRot[1]);
				/*if (i >= 5 && i <= 6) mtx->setRotFromEulerX(veh->m_aFakeTrackWheelsRot[1] * 2.f);
				else */mtx->addToRotFromEulerX(veh->m_aTrackSpeed[1] * *timer::g_pTimestep);
			}
		}

	}

	struct crSkelData {
		crBoneData* m_pBones;
		BYTE _f4[0x10];
		WORD m_wNumBones;

		WORD getBoneIndexByName(const char* pszName) {
			if (!m_wNumBones) return -1;
			for (WORD i = 0; i < m_wNumBones; i++)
				if (!strcmp(m_pBones[i].m_pszName, pszName))
					return i;
			return -1;
		}
	};

	struct modelInfo {
		BYTE __0[0x8];
		struct {
			BYTE __0[0xb4];
			struct {
				BYTE __0[0xc];
				crSkelData* m_pSkel;
			}*m_pDrawable;
		}*m_pFragType;
	};

	void process(CExtVeh* veh) {

		if (!veh->m_ExtFlags.bHaveNewParts) {
			WORD modelIndex = *(WORD*)((BYTE*)veh + 46);
			if (modelIndex != -1) {

				//	//crSkelData* pSkel = *(crSkelData**)(*(DWORD*)(*(DWORD*)(*(DWORD*)(g_ppModelInfoPtrs + 4 * modelIndex) + 8) + 180) + 12);
				//	//crSkelData* pSkel = *(crSkelData**)(*(DWORD*)(*(DWORD*)(*(DWORD*)(g_ppModelInfoPtrs + 4 * modelIndex) + 0xc) + 0x0) + 0xc);
				crSkelData* pSkel = ((modelInfo**)g_ppModelInfoPtrs)[modelIndex]->m_pFragType->m_pDrawable->m_pSkel;

				//	veh->m_newPartsIds.m_nbFakeTankLWheels[0] = pSkel->getBoneIndexByName("wheel_lm1");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[1] = pSkel->getBoneIndexByName("wheel_lm2");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[2] = pSkel->getBoneIndexByName("wheel_lm3");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[3] = pSkel->getBoneIndexByName("wheel_lm4");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[4] = pSkel->getBoneIndexByName("wheel_lm5");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[5] = pSkel->getBoneIndexByName("wheel_lm6");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[6] = pSkel->getBoneIndexByName("wheel_lm7");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[7] = pSkel->getBoneIndexByName("wheel_lm8");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[8] = pSkel->getBoneIndexByName("wheel_lm9");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[9] = pSkel->getBoneIndexByName("wheel_lm10");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[10] = pSkel->getBoneIndexByName("wheel_lm11");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[11] = pSkel->getBoneIndexByName("wheel_lm12");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[12] = pSkel->getBoneIndexByName("wheel_lm13");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[13] = pSkel->getBoneIndexByName("wheel_lm14");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[14] = pSkel->getBoneIndexByName("wheel_lm15");
				//	veh->m_newPartsIds.m_nbFakeTankLWheels[15] = pSkel->getBoneIndexByName("wheel_lm16");
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[9] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[10] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[11] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[12] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[13] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[14] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankLWheels[15] = 0xff;

				//	veh->m_newPartsIds.m_nbFakeTankRWheels[0] = pSkel->getBoneIndexByName("wheel_rm1");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[1] = pSkel->getBoneIndexByName("wheel_rm2");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[2] = pSkel->getBoneIndexByName("wheel_rm3");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[3] = pSkel->getBoneIndexByName("wheel_rm4");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[4] = pSkel->getBoneIndexByName("wheel_rm5");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[5] = pSkel->getBoneIndexByName("wheel_rm6");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[6] = pSkel->getBoneIndexByName("wheel_rm7");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[7] = pSkel->getBoneIndexByName("wheel_rm8");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[8] = pSkel->getBoneIndexByName("wheel_rm9");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[9] = pSkel->getBoneIndexByName("wheel_rm10");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[10] = pSkel->getBoneIndexByName("wheel_rm11");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[11] = pSkel->getBoneIndexByName("wheel_rm12");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[12] = pSkel->getBoneIndexByName("wheel_rm13");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[13] = pSkel->getBoneIndexByName("wheel_rm14");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[14] = pSkel->getBoneIndexByName("wheel_rm15");
				//	veh->m_newPartsIds.m_nbFakeTankRWheels[15] = pSkel->getBoneIndexByName("wheel_rm16");
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[9] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[10] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[11] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[12] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[13] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[14] = 0xff;
				//	//veh->m_newPartsIds.m_nbFakeTankRWheels[15] = 0xff;

				////	memset(veh->m_newPartsIds.m_nbFakeTankLWheels + 1, -1, 0x1f);

				char msg[0x100];
				for (DWORD i = 0; i < 16; i++) {
					sprintf(msg, "wheel_lm%i", i);
					veh->m_newPartsIds.m_nbFakeTankLWheels[i] = pSkel->getBoneIndexByName(msg);
				}
				for (DWORD i = 0; i < 16; i++) {
					sprintf(msg, "wheel_rm%i", i);
					veh->m_newPartsIds.m_nbFakeTankRWheels[i] = pSkel->getBoneIndexByName(msg);
				}


			}
			veh->m_ExtFlags.bHaveNewParts = 1;
		}

		processRhinoFakeWheels(veh);
	}

	void _new(CExtVeh* veh) {
		veh->m_ExtFlags.bHaveNewParts = 0;
	}

namespace hooks {
	namespace CAutomobile__prerender { void hook(); }
	namespace CVehicle__CVehicle { void hook(); }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		hooks::CAutomobile__prerender::hook();
		hooks::CVehicle__CVehicle::hook();
		hooks::CAutomobile__prerender::add((void(*)(CVehicle*))process);
		hooks::CVehicle__CVehicle::add((void(*)(CVehicle*))_new);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

