#include "CBaseVeh.h"
#include "helpers.h"
#include "injector/injector.hpp"
#include "Hooking.Patterns-master/Hooking.Patterns.h"

size_t g_origSaveGarageCall;
size_t g_origRestoreGarageCall;

void* pCurrGarage = NULL;

struct CSaveGarage {
	float m_pos[3];
	DWORD m_dwHandlingFlags;
	WORD m_wModelIndex;
	
	BYTE m_nbPlateTexIndex;
	BYTE m_nbCharCount;
	
	//BYTE __12;

	char m_aPlateText[PLATE_NUM_CHARS];
	float m_aLettersScale[2];
	float m_aLettersOffset[2];

	//BYTE __12[30];
	WORD m_flags;
	BYTE m_colors[4];
	DWORD m_fExtras;
	DWORD m_dwLiveryIndex;

	BYTE m_aPlateTextTintRG[2];
	//BYTE __40[2];
	BYTE _f42;
	char m_iFrontX;
	char m_iFrontY;
	char m_iFrontZ;
	BYTE m_aPlateTextTintBA[2];
	//BYTE __pad_46[2];


	char unk(CExtVeh* veh) {
		auto ret = ((char(__thiscall*)(CSaveGarage*, CVehicle*))g_origSaveGarageCall)(this, veh);
		memcpy(m_aPlateText, veh->m_aLicensePlateText, PLATE_NUM_CHARS);
		m_nbCharCount = veh->m_nbCharCount;
		m_nbPlateTexIndex = veh->m_nbPlateTexIndex;
		m_aLettersScale[0] = veh->m_aLettersScale[0];
		m_aLettersScale[1] = veh->m_aLettersScale[1];
		m_aLettersOffset[0] = veh->m_aLettersOffset[0];
		m_aLettersOffset[1] = veh->m_aLettersOffset[1];
		*(WORD*)m_aPlateTextTintRG = *(WORD*)veh->m_aPlateTextTint;
		*(WORD*)m_aPlateTextTintBA = *(WORD*)(veh->m_aPlateTextTint + 2);

		//char msg[0x1ff];
		//sprintf(msg, "m_nbCharCount=%u\nm_nbPlateTexIndex=%u\nm_aLettersScale0=%f\nm_aLettersScale1=%f\nm_aLettersOffset0=%f\nm_aLettersOffset1=%f\nm_aPlateTextTintR=%u\nm_aPlateTextTintG=%u\nm_aPlateTextTintB=%u\nm_aPlateTextTintA=%u",
		//	m_nbCharCount, m_nbPlateTexIndex, m_aLettersScale[0], m_aLettersScale[1], m_aLettersOffset[0], m_aLettersOffset[1],
		//	m_aPlateTextTintRG[0], m_aPlateTextTintRG[1], m_aPlateTextTintBA[0], m_aPlateTextTintBA[1]);
		//MessageBoxA(NULL, msg, NULL, 0x10);
		return ret;
	}

	// get garage addr
	size_t unk2(CExtVeh*) { pCurrGarage = this; return 0; }

	// restore car from garage
	CSaveGarage* unk3() {
		//MessageBoxA(NULL, "GARAGE 2", NULL, 0x10);
		CSaveGarage* pGarage = (CSaveGarage*)pCurrGarage;

		CExtVeh* pVeh = (CExtVeh*)this;

		//memcpy(pVeh->m_aLicensePlateText, pGarage->m_aPlateText, PLATE_NUM_CHARS);
		//pVeh->m_nbCharCount = 0;
		//for (size_t i = 0; i < PLATE_NUM_CHARS; i++) {
		//	if (pVeh->m_aLicensePlateText[i] == -1)
		//		break;
		//	pVeh->m_nbCharCount++;
		//}
		memcpy(pVeh->m_aLicensePlateText, pGarage->m_aPlateText, PLATE_NUM_CHARS);
		pVeh->m_nbCharCount = pGarage->m_nbCharCount;
		pVeh->m_nbPlateTexIndex = pGarage->m_nbPlateTexIndex;
		pVeh->m_aLettersScale[0] = pGarage->m_aLettersScale[0];
		pVeh->m_aLettersScale[1] = pGarage->m_aLettersScale[1];
		pVeh->m_aLettersOffset[0] = pGarage->m_aLettersOffset[0];
		pVeh->m_aLettersOffset[1] = pGarage->m_aLettersOffset[1];
		*(WORD*)pVeh->m_aPlateTextTint = *(WORD*)pGarage->m_aPlateTextTintRG;
		*(WORD*)(pVeh->m_aPlateTextTint + 2) = *(WORD*)pGarage->m_aPlateTextTintBA;


		//char msg[0x1ff];
		//sprintf(msg, "m_nbCharCount=%u\nm_nbPlateTexIndex=%u\nm_aLettersScale0=%f\nm_aLettersScale1=%f\nm_aLettersOffset0=%f\nm_aLettersOffset1=%f\nm_aPlateTextTintR=%u\nm_aPlateTextTintG=%u\nm_aPlateTextTintB=%u\nm_aPlateTextTintA=%u",
		//	pGarage->m_nbCharCount, pGarage->m_nbPlateTexIndex, pGarage->m_aLettersScale[0], pGarage->m_aLettersScale[1], pGarage->m_aLettersOffset[0], pGarage->m_aLettersOffset[1],
		//	pGarage->m_aPlateTextTintRG[0], pGarage->m_aPlateTextTintRG[1], pGarage->m_aPlateTextTintBA[0], pGarage->m_aPlateTextTintBA[1]);
		//MessageBoxA(NULL, msg, NULL, 0x10);

		//sprintf(msg, "m_nbCharCount=%u\nm_nbPlateTexIndex=%u\nm_aLettersScale0=%f\nm_aLettersScale1=%f\nm_aLettersOffset0=%f\nm_aLettersOffset1=%f\nm_aPlateTextTintR=%u\nm_aPlateTextTintG=%u\nm_aPlateTextTintB=%u\nm_aPlateTextTintA=%u",
		//	pVeh->m_nbCharCount, pVeh->m_nbPlateTexIndex, pVeh->m_aLettersScale[0], pVeh->m_aLettersScale[1], pVeh->m_aLettersOffset[0], pVeh->m_aLettersOffset[1],
		//	pVeh->m_aPlateTextTint[0], pVeh->m_aPlateTextTint[1], pVeh->m_aPlateTextTint[2], pVeh->m_aPlateTextTint[3]);
		//MessageBoxA(NULL, msg, NULL, 0x10);


		// restore livery index
		BYTE* pDrawRef = *(BYTE**)((size_t)pVeh + 0x34);
		BYTE* pCustomShaderFx = *(BYTE**)(pDrawRef + 4);
		if (pCustomShaderFx) {
			*(DWORD*)(pCustomShaderFx + 0xd8) = pGarage->m_dwLiveryIndex;
		}

		auto ret = ((CSaveGarage * (__thiscall*)(CSaveGarage*))g_origRestoreGarageCall)(this); // update colors
		return this;
	}

};

// hook for saving car from the garage
void backupCarHook() {
	hook::pattern p("E8 ? ? ? ? 80 7D 0C 00 74 0E");
	if (!p.empty()) {
		auto addr = p.get_first();
		BYTE* patch = (BYTE*)addr;
		patch++;
		g_origSaveGarageCall = (*(DWORD*)patch + ((size_t)addr + 5));
		injector::MakeCALL(addr, getThisCallFuncAddr(&CSaveGarage::unk));
	}
	else {
		ERROR("testGarage()");
	}

}
//

// hook for restore car from the garage
void restoreCarHook() {
	hook::pattern p("8B 4F 3C 51 8B CE E8 ? ? ? ?");
	if (!p.empty()) {
		size_t addr = (size_t)p.get_first();

		addr += 3; addr++;

		// mov ecx, edi; edi is pGarage
		injector::WriteMemory(addr + 1, 0xcf); 
		addr += 2; // skip mov

		// call to fn for save the pGarage
		injector::MakeCALL(addr, getThisCallFuncAddr(&CSaveGarage::unk2));
		addr += 5; // skip call

		addr += 2; // skip mov

		BYTE* patch = (BYTE*)addr; // patch is ptr to call
		patch++; // go to addr

		// save orig call to CCustomShaderEffectVehFx::update
		g_origRestoreGarageCall = (*(DWORD*)patch + (addr + 5));

		// replace CCustomShaderEffectVehFx::update by my func
		injector::MakeCALL(addr, getThisCallFuncAddr(&CSaveGarage::unk3));

	}
	else {
		ERROR("testGarage2()");
	}

}

namespace garage_car_patch {
	void init() {
		backupCarHook();
		restoreCarHook();
	}
}

//