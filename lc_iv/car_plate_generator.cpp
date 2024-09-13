#include "hooks.h"
#include "CBaseVeh.h"

// for a new civilization that wants to continue this idea
// UNUSED
void translateToLPlate(char* pszText, size_t count) {
	for (size_t i = 0; i < count; i++) {
		if (pszText[i] == '*')
			pszText[i] = 36;
		else if (pszText[i] == ' ')
			pszText[i] = 37;
		else if (pszText[i] >= '0' && pszText[i] <= '9') {
			pszText[i] -= 0x30;
		}
		else {
			if (pszText[i] >= 'A' && pszText[i] <= 'Z')
				pszText[i] += 0x20;
			if (pszText[i] >= 'a' && pszText[i] <= 'z')
				pszText[i] -= 87;

		}
	}
}

void __cdecl generatePlate(CExtVeh* veh) {
	//ERROR("USED OLD generator!");
	//DWORD type1 = *(DWORD*)((BYTE*)veh + 0x1350);
	//DWORD type2 = *(DWORD*)((BYTE*)veh + 0x1354);

	//if ((!type1 && !type2) || (type1 == 1)) {
	//	DWORD randomorg = 0 | (0 >> 16);
	//	for (char i = 0; i < 3; i++)
	//		veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 26 + 10;
	//	veh->m_aLicensePlateText[3] = 36;
	//	for (char i = 0; i < 4; i++)
	//		veh->m_aLicensePlateText[i + 4] = (randomorg >> (i * 4) & 0xf) % 10;
	//	veh->m_nbCharCount = 8;
	//}
	//else
	//	for (char i = 0; i < 8; i++)
	//		veh->m_aLicensePlateText[i] = 0;
	//strcpy(veh->m_aLicensePlateText, "shvab");
	veh->m_aLicensePlateText[0] = 28;
	veh->m_aLicensePlateText[1] = 17;
	veh->m_aLicensePlateText[2] = 31;
	veh->m_aLicensePlateText[3] = 10;
	veh->m_aLicensePlateText[4] = 11;

	veh->m_nbCharCount = 5;

	veh->m_nbPlateTexIndex = 0;

	veh->m_aPlateTextTint[0] = 255;
	veh->m_aPlateTextTint[3] = 255;
	veh->m_aLettersScale[0] = 2.f;
	veh->m_aLettersScale[1] = 1.f;
	veh->m_aLettersOffset[0] = 0.f;
	veh->m_aLettersOffset[1] = 0.f;

}

void(__cdecl* getPlate)(CExtVeh* veh) = NULL;

void initGenerator() {
	HMODULE module_ = LoadLibraryA("lc_iv_plate_generator.lc_iv");
	if (module_) {
		getPlate = (void(__cdecl*)(CExtVeh * veh))GetProcAddress(module_, "lc_iv::generatePlate");
		if (!getPlate) {
			MessageBoxW(NULL, L"Функция 'generatePlate' в пространстве имен 'lc_iv' не найдено. Удалите lc_iv_plate_generator.lc_iv или исправьте его", NULL, 0x10);
			ExitProcess(0);
		}
	}
	else getPlate = generatePlate;
}

void generateLP(CExtVeh* veh) { getPlate(veh); }

namespace car_plate_generator {

	void init() {
		initGenerator();
		hooks::CVehicleFactoryNY__create::add((void(*)(CVehicle*))generateLP);
	}

}