#include "..\lc_iv\CBaseVeh.h"
#include "..\lc_iv\helpers.h"
#include "..\lc_iv\injector/injector.hpp"

#include <random>
#include <string>
#include <fstream>
#include "..\lc_iv\Hooking.Patterns-master\Hooking.Patterns.h"
//DWORD seed = 0;

struct tModelNameId;

tModelNameId** g_ppModelNameList = *(tModelNameId***)hook::pattern("8B 15 ? ? ? ? 89 50 08 A3 ? ? ? ? C2 04 00").get_first(2);

struct tModelNameId {
	const char* pszName;
	int nModelId;
	tModelNameId* pPrev;

	tModelNameId(const char* _pszName) {
		pszName = _pszName;
		//MessageBoxA(NULL, pszName, NULL, 0x10);
		nModelId = -1;
		pPrev = *g_ppModelNameList;
		*g_ppModelNameList = this;
	}
};

tModelNameId police_models[]{ tModelNameId("police2"), tModelNameId("police"), tModelNameId("policeb"), tModelNameId("policea"), tModelNameId("police3"), tModelNameId("police4") };
WORD police_count = sizeof(police_models) / sizeof(tModelNameId);
tModelNameId fbi_models[]{ tModelNameId("fbi") };
WORD fbi_count = sizeof(fbi_models) / sizeof(tModelNameId);
tModelNameId taxi_models[]{ tModelNameId("taxi"),tModelNameId("taxi2"),tModelNameId("cabby") };
WORD taxi_count = sizeof(taxi_models) / sizeof(tModelNameId);
tModelNameId limo_models[]{ tModelNameId("stretch") };
WORD limo_count = sizeof(limo_models) / sizeof(tModelNameId);
tModelNameId official_models[]{ tModelNameId("pbus"), tModelNameId("dukes") };
WORD official_count = sizeof(official_models) / sizeof(tModelNameId);
tModelNameId commercial_models[]{ tModelNameId("reversed") };
WORD commercial_count = sizeof(commercial_models) / sizeof(tModelNameId);
tModelNameId amb_models[]{ tModelNameId("ambulance") };
WORD amb_count = sizeof(amb_models) / sizeof(tModelNameId);

DWORD fake_rand() {
	WORD ret[2]{ rand(),rand()};
	return *(DWORD*)ret;
}


enum ePlateChars : char {
	CHAR_0,
	CHAR_1,
	CHAR_2,
	CHAR_3,
	CHAR_4,
	CHAR_5,
	CHAR_6,
	CHAR_7,
	CHAR_8,
	CHAR_9,
	CHAR_A,
	CHAR_B,
	CHAR_C,
	CHAR_D,
	CHAR_E,
	CHAR_F,
	CHAR_G,
	CHAR_H,
	CHAR_I,
	CHAR_J,
	CHAR_K,
	CHAR_L,
	CHAR_M,
	CHAR_N,
	CHAR_O,
	CHAR_P,
	CHAR_Q,
	CHAR_R,
	CHAR_S,
	CHAR_T,
	CHAR_U,
	CHAR_V,
	CHAR_W,
	CHAR_X,
	CHAR_Y,
	CHAR_Z,
	CHAR_STAR,
	CHAR_SPACE,
	CHAR_LINE,
	CHAR_NEWJERSEY,
	CHAR_NEVADA,
	CHAR_NY,

	// why?
	CHAR_RUS_Б,
	CHAR_RUS_Г,
	CHAR_RUS_Д,
	CHAR_RUS_Ж,
	CHAR_RUS_З,
	CHAR_RUS_И,
	CHAR_RUS_Л,
	CHAR_RUS_П,
	CHAR_RUS_У,
	CHAR_RUS_Ф,
	CHAR_RUS_Ц,
	CHAR_RUS_Ч,
	CHAR_RUS_Ш,
	CHAR_RUS_Щ,
	CHAR_RUS_Ъ,
	CHAR_RUS_Ы,
	CHAR_RUS_Ь,
	CHAR_RUS_Э,
	CHAR_RUS_Ю,
	CHAR_RUS_Я,
	CHAR_UA_Є,
	
	CHAR_RUS_А = CHAR_A,
	CHAR_RUS_В = CHAR_B,
	CHAR_RUS_Е = CHAR_E,
	CHAR_RUS_Ё = CHAR_RUS_Е,
	CHAR_RUS_К = CHAR_K,
	CHAR_RUS_М = CHAR_M,
	CHAR_RUS_Н = CHAR_H,
	CHAR_RUS_О = CHAR_O,
	CHAR_RUS_Р = CHAR_P,
	CHAR_RUS_С = CHAR_C,
	CHAR_RUS_Т = CHAR_T,
	CHAR_RUS_Й = CHAR_RUS_И,
	CHAR_RUS_Х = CHAR_X,
	CHAR_UA_І = CHAR_I,
	CHAR_UA_Ї = CHAR_UA_І,

};

// old(2022)
void translatePlate(char* pszText, size_t count) {
	for (size_t i = 0; i < count; i++) {
		if (pszText[i] == '*')
			pszText[i] = CHAR_NY;
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

BYTE g_nbNumPlates;

enum eFlyingPlateCity : char {
	FLY_CITY_SANFIERRO,
	FLY_CITY_LASVENTURES,
	FLY_CITY_LOSSANTOS,
	FLY_CITY_VICECITY,
	FLY_CITY_CARCERCITY,
	FLY_CITY_COTTONMOUNTH,
	FLY_CITY_LIBERTYCITY
};

enum eVehPlateCity : char {
	VEH_CITY_SANFIERRO,
	VEH_CITY_LASVENTURES,
	VEH_CITY_LOSSANTOS,
	VEH_CITY_VICECITY,
	VEH_CITY_CARCERCITY,
	VEH_CITY_NEWJERSEY,
	VEH_CITY_LIBERTYCITY
};

struct tVec3 {
	float x, y, z;
};

struct tZoneBox {
	float x1, x2, y1, y2;

	bool inZone(tVec3* pCoords) {
		return pCoords->x >= x1 ? pCoords->x <= x2 ? pCoords->y >= y1 ? pCoords->y <= y2 ? true : false : false : false : false;
	}

};

struct tZoneSphere {
	float x, y, radius;

	bool inZone(tVec3* pCoords) {
		return sqrtf((pCoords->x - x) * (pCoords->x - x) + (pCoords->y - y) * (pCoords->y - y)) <= radius;
	}
};

void __cdecl lc_iv__generatePlate(CExtVeh* veh) {
	DWORD type1 = *(DWORD*)((BYTE*)veh + 0x1350);
	DWORD type2 = *(DWORD*)((BYTE*)veh + 0x1354);

	unsigned __int64 randomorg = fake_rand();
	*((DWORD*)&randomorg + 1) = fake_rand();

	void* pMtx = (BYTE*)veh + 0x20;
	tVec3* pPos = NULL;
	if (pMtx) pPos = (tVec3*)(*(BYTE**)pMtx + 0x30);
	else pPos = (tVec3*)((BYTE*)veh + 0x10);


	if ((!type1 && !type2) || (type1 == 1)) {
		//bool bIsPolice = false;

		WORD wModelIndex = *(WORD*)((BYTE*)veh + 0x2e);

		// is ambulance?
		for (size_t i = 0; i < amb_count; i++) {
			if (amb_models[i].nModelId == wModelIndex) {
				for (size_t i = 0; i < 4; i++)
					veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 10;
				veh->m_nbCharCount = 4;

				veh->m_aPlateTextTint[0] = 9;
				veh->m_aPlateTextTint[1] = 41;
				veh->m_aPlateTextTint[2] = 79;
				veh->m_aPlateTextTint[3] = 255;



				veh->m_aLettersOffset[0] = 0.f;
				veh->m_aLettersOffset[1] = -0.07f;

				veh->m_aLettersScale[0] = 1.25f;
				veh->m_aLettersScale[1] = 0.43f;


				veh->m_nbPlateTexIndex = 10;
				return;
			}
		}

		// is police?
		for (size_t i = 0; i < police_count; i++) {
			if (police_models[i].nModelId == wModelIndex) {
				for (size_t i = 0; i < 4; i++)
					veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 10;
				veh->m_nbCharCount = 4;

				veh->m_aPlateTextTint[0] = 0;
				veh->m_aPlateTextTint[1] = 24;
				veh->m_aPlateTextTint[2] = 148;
				veh->m_aPlateTextTint[3] = 255;
				veh->m_nbPlateTexIndex = 4;

				veh->m_aLettersOffset[0] = 0.f;
				veh->m_aLettersOffset[1] = -0.12f;
				veh->m_aLettersScale[0] = 2.0f;
				veh->m_aLettersScale[1] = 0.5f;
				return;
			}
		}

		// is fbi?
		for (size_t i = 0; i < fbi_count; i++) {
			if (fbi_models[i].nModelId == wModelIndex) {
				for (size_t i = 1; i < 7; i++) {
					bool bIsChar = !(randomorg >> (i * 4) % 3);
					if (bIsChar)veh->m_aLicensePlateText[i] = ((randomorg >> (i * 4) & 0x1f) % 26) + 10;
					else veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 10;
				}
				veh->m_aLicensePlateText[0] = CHAR_SPACE;

				veh->m_nbCharCount = 7;

				veh->m_aPlateTextTint[0] = 0;
				veh->m_aPlateTextTint[1] = 24;
				veh->m_aPlateTextTint[2] = 148;
				veh->m_aPlateTextTint[3] = 255;
				veh->m_nbPlateTexIndex = 6;

				veh->m_aLettersOffset[0] = 0.f;
				veh->m_aLettersOffset[1] = 0.0f;
				veh->m_aLettersScale[0] = 1.4f;
				veh->m_aLettersScale[1] = 0.5f;
				return;
			}
		}

		tZoneBox newjersey_box0{ -3012.708f, -899.104f, -97.561f, 3022.125f };
		tZoneBox newjersey_box1{ -3004.542f,-983.64f, -3083.938f, -88.429f };
		tZoneBox newjersey_box2{ -907.256f,-770.311f, 1764.2f, 3009.105f };
		tZoneBox newjersey_box3{ -902.171f,-708.539f, 1175.113f, 1767.925f };
		tZoneBox newjersey_box4{ -908.282f,-750.397f, 725.196f, 1175.018f };
		tZoneSphere newjersey_sphere0{ -905.92f, 729.632f, 158.152f };
		tZoneSphere newjersey_sphere1{ -1077.884f, -404.634f, 268.055f };


		BYTE nbCity = randomorg % 100;
		//goto FORCE_VC;
		// временно отключаем города для которых я не рисовал номер
		if (nbCity == VEH_CITY_SANFIERRO ||
			//nbCity == VEH_CITY_LASVENTURES ||
			nbCity == VEH_CITY_LOSSANTOS ||
			nbCity == VEH_CITY_CARCERCITY) nbCity = VEH_CITY_NEWJERSEY;
		else if (nbCity > VEH_CITY_NEWJERSEY)nbCity = VEH_CITY_NEWJERSEY;
		
		if ((nbCity == VEH_CITY_NEWJERSEY) != 0) { 
			
			// шанс 20 процентов что номер будет из соседнего штата
			bool b1 = !((((randomorg >> 8) & 0xff) % 100) % 5);

			bool bIsNewJersey = newjersey_box0.inZone(pPos) ||
				newjersey_box1.inZone(pPos) ||
				newjersey_box2.inZone(pPos) ||
				newjersey_box3.inZone(pPos) ||
				newjersey_box4.inZone(pPos) ||
				newjersey_sphere0.inZone(pPos) ||
				newjersey_sphere1.inZone(pPos);

			// если номер из соседнего штата, то просто меняем его
			if (b1) bIsNewJersey = !bIsNewJersey;

			if (bIsNewJersey) {
				BYTE arr[]{ 0,4,5,6 };
				for (size_t i = 0; i < 4; i++)
					veh->m_aLicensePlateText[arr[i]] = ((randomorg >> (arr[i] * 4) & 0x1f) % 26) + 10;
				for (size_t i = 1; i < 3; i++)
					veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 10;
				veh->m_aLicensePlateText[3] = CHAR_NEWJERSEY;

				veh->m_nbCharCount = 7;

				veh->m_aPlateTextTint[0] = 0;
				veh->m_aPlateTextTint[1] = 0;
				veh->m_aPlateTextTint[2] = 0;
				veh->m_aPlateTextTint[3] = 255;
				veh->m_nbPlateTexIndex = 2;

				veh->m_aLettersOffset[0] = 0.f;
				veh->m_aLettersOffset[1] = -0.05f;

				veh->m_aLettersScale[0] = 1.4f;// -0.1318359375f;
				veh->m_aLettersScale[1] = 1.f - 0.594921875f;

			}
			else {
				veh->m_aPlateTextTint[0] = 9;
				veh->m_aPlateTextTint[1] = 41;
				veh->m_aPlateTextTint[2] = 79;
				veh->m_aPlateTextTint[3] = 255;


				if (false) {
					veh->m_aLettersOffset[0] = 0.f;
					veh->m_aLettersOffset[1] = -0.06f;

					veh->m_aLettersScale[0] = 0.93f;
					veh->m_aLettersScale[1] = 0.38f;

					veh->m_aLicensePlateText[0] = CHAR_L;
					veh->m_aLicensePlateText[1] = CHAR_I;
					veh->m_aLicensePlateText[2] = CHAR_B;
					veh->m_aLicensePlateText[3] = CHAR_E;
					veh->m_aLicensePlateText[4] = CHAR_R;
					veh->m_aLicensePlateText[5] = CHAR_T;
					veh->m_aLicensePlateText[6] = CHAR_Y;
					veh->m_aLicensePlateText[7] = CHAR_SPACE;
					veh->m_aLicensePlateText[8] = CHAR_C;
					veh->m_aLicensePlateText[9] = CHAR_I;
					veh->m_aLicensePlateText[10] = CHAR_T;
					veh->m_aLicensePlateText[11] = CHAR_Y;

					veh->m_nbCharCount = 12;
					veh->m_nbPlateTexIndex = 0;

					return;
				}

				for (char i = 0; i < 3; i++)
					veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 26 + 10;
				veh->m_aLicensePlateText[3] = CHAR_NY;
				for (char i = 0; i < 4; i++)
					veh->m_aLicensePlateText[i + 4] = (randomorg >> (i * 4) & 0xf) % 10;
				veh->m_nbCharCount = 8;
				//}

				veh->m_aLettersOffset[0] = 0.f;
				veh->m_aLettersOffset[1] = -0.07f;

				veh->m_aLettersScale[0] = 1.25f;
				veh->m_aLettersScale[1] = 0.43f;


				// is taxi?
				for (size_t i = 0; i < taxi_count; i++) {
					if (taxi_models[i].nModelId == wModelIndex) {
						veh->m_nbPlateTexIndex = 5;
						return;
					}
				}
				// is limo?
				for (size_t i = 0; i < limo_count; i++) {
					if (limo_models[i].nModelId == wModelIndex) {
						veh->m_nbPlateTexIndex = 7;
						return;
					}
				}
				// is official?
				for (size_t i = 0; i < official_count; i++) {
					if (official_models[i].nModelId == wModelIndex) {
						veh->m_nbPlateTexIndex = 8;
						return;
					}
				}
				// is official?
				for (size_t i = 0; i < commercial_count; i++) {
					if (commercial_models[i].nModelId == wModelIndex) {
						veh->m_nbPlateTexIndex = 9;
						return;
					}
				}

				veh->m_nbPlateTexIndex = 0;
			}
		}
		else if(nbCity == VEH_CITY_VICECITY) { // VC
			FORCE_VC:

			veh->m_nbCharCount = 7;
			for (char i = 0; i < 7; i++)
				veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 36;
			veh->m_aLicensePlateText[3] = CHAR_SPACE;


			veh->m_aPlateTextTint[0] = 2;
			veh->m_aPlateTextTint[1] = 46;
			veh->m_aPlateTextTint[2] = 32;
			veh->m_aPlateTextTint[3] = 255;
			veh->m_nbPlateTexIndex = 1;

			veh->m_aLettersOffset[0] = 0.f;
			veh->m_aLettersOffset[1] = -0.04f;

			veh->m_aLettersScale[0] = 1.35f;// -0.1318359375f;
			veh->m_aLettersScale[1] = 0.4f;// -0.544921875f;

		}
		else if(nbCity == VEH_CITY_LASVENTURES) {
			for (size_t i = 4; i < 7; i++)
				veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 36;
			for (size_t i = 0; i < 3; i++)
				veh->m_aLicensePlateText[i] = (randomorg >> (i * 4) & 0x1f) % 10;
			veh->m_aLicensePlateText[3] = CHAR_NEVADA;

			veh->m_nbCharCount = 7;

			veh->m_aPlateTextTint[0] = 0;
			veh->m_aPlateTextTint[1] = 0;
			veh->m_aPlateTextTint[2] = 0;
			veh->m_aPlateTextTint[3] = 255;
			veh->m_nbPlateTexIndex = 3;

			veh->m_aLettersOffset[0] = 0.f;
			veh->m_aLettersOffset[1] = -0.13f;

			veh->m_aLettersScale[0] = 1.4f;// -0.1318359375f;
			veh->m_aLettersScale[1] = 1.f - 0.534921875f;

		}

	}
	else if (type2 == 5 || type2 == 4) {
	//else if(true) {
		// 6% other city
		// 94% - liberty city
		veh->m_nbCharCount = 8;

		BYTE nbCityIndex = randomorg % 100;
		if (nbCityIndex > FLY_CITY_LIBERTYCITY) nbCityIndex = FLY_CITY_LIBERTYCITY;

		veh->m_aLicensePlateText[0] = CHAR_N;
		veh->m_aLicensePlateText[1] = CHAR_LINE;
		switch (nbCityIndex) {
		case FLY_CITY_SANFIERRO:
			veh->m_aLicensePlateText[2] = CHAR_S;
			veh->m_aLicensePlateText[3] = CHAR_F;
			break;
		case FLY_CITY_LASVENTURES:
			veh->m_aLicensePlateText[2] = CHAR_L;
			veh->m_aLicensePlateText[3] = CHAR_V;
			break;
		case FLY_CITY_LOSSANTOS:
			veh->m_aLicensePlateText[2] = CHAR_L;
			veh->m_aLicensePlateText[3] = CHAR_S;
			break;
		case FLY_CITY_VICECITY:
			veh->m_aLicensePlateText[2] = CHAR_V;
			veh->m_aLicensePlateText[3] = CHAR_C;
			break;
		case FLY_CITY_CARCERCITY:
			veh->m_aLicensePlateText[2] = CHAR_C;
			veh->m_aLicensePlateText[3] = CHAR_C;
			break;
		case FLY_CITY_COTTONMOUNTH:
			veh->m_aLicensePlateText[2] = CHAR_C;
			veh->m_aLicensePlateText[3] = CHAR_M;
			break;
		case FLY_CITY_LIBERTYCITY:
			veh->m_aLicensePlateText[2] = CHAR_L;
			veh->m_aLicensePlateText[3] = CHAR_C;
			break;
		}
		// random
		bool bUseOnlyChars = false;
		for (BYTE i = 4; i < 8; i++) {
			BYTE nbSeed = (randomorg >> ((i - 4) * 8)) & 0xff; 
			if (!bUseOnlyChars) {
				bool bIsNum = (nbSeed % 10) < 6;
				if (bIsNum) veh->m_aLicensePlateText[i] = nbSeed % 10;
				else veh->m_aLicensePlateText[i] = nbSeed % 36;
			}
			else veh->m_aLicensePlateText[i] = (nbSeed % 26) + 10;

			if (veh->m_aLicensePlateText[i] >= CHAR_A && veh->m_aLicensePlateText[i] <= CHAR_Z) bUseOnlyChars = true;

			//veh->m_aLicensePlateText[i] = !bUseOnlyChars ? nbSeed % 35 : (nbSeed % 25) + 10; // old
		}
		veh->m_aPlateTextTint[0] = 0xff;
		veh->m_aPlateTextTint[1] = 0xff;
		veh->m_aPlateTextTint[2] = 0xff;
		veh->m_aPlateTextTint[3] = 0xff;
		veh->m_nbPlateTexIndex = 0;

		veh->m_aLettersOffset[0] = 0.f;
		veh->m_aLettersOffset[1] = 0.f;

		veh->m_aLettersScale[0] = (float)PLATE_NUM_CHARS / veh->m_nbCharCount;
		veh->m_aLettersScale[1] = 1.f;
	}
	else if (type1 == 2) {
	//else if (true) {
		// 6% other city
		// 94% - liberty city
		veh->m_nbCharCount = 12;

		BYTE nbCityIndex = randomorg % 100;
		if (nbCityIndex > FLY_CITY_LIBERTYCITY) nbCityIndex = FLY_CITY_LIBERTYCITY;

		switch (nbCityIndex) {
		case FLY_CITY_SANFIERRO:
			veh->m_aLicensePlateText[0] = CHAR_S;
			veh->m_aLicensePlateText[1] = CHAR_F;
			break;
		case FLY_CITY_LASVENTURES:
			veh->m_aLicensePlateText[0] = CHAR_L;
			veh->m_aLicensePlateText[1] = CHAR_V;
			break;
		case FLY_CITY_LOSSANTOS:
			veh->m_aLicensePlateText[0] = CHAR_L;
			veh->m_aLicensePlateText[1] = CHAR_S;
			break;
		case FLY_CITY_VICECITY:
			veh->m_aLicensePlateText[0] = CHAR_V;
			veh->m_aLicensePlateText[1] = CHAR_C;
			break;
		case FLY_CITY_CARCERCITY:
			veh->m_aLicensePlateText[0] = CHAR_C;
			veh->m_aLicensePlateText[1] = CHAR_C;
			break;
		case FLY_CITY_COTTONMOUNTH:
			veh->m_aLicensePlateText[0] = CHAR_C;
			veh->m_aLicensePlateText[1] = CHAR_M;
			break;
		case FLY_CITY_LIBERTYCITY:
			veh->m_aLicensePlateText[0] = CHAR_L;
			veh->m_aLicensePlateText[1] = CHAR_C;
			break;
		}
		veh->m_aLicensePlateText[2] = CHAR_LINE;

		for (BYTE i = 3; i < 9; i++) {
			BYTE nbSeed = (randomorg >> (i * 4)) & 0xff;
			veh->m_aLicensePlateText[i] = nbSeed % 10;
		}

		veh->m_aLicensePlateText[9] = CHAR_LINE;

		for (BYTE i = 10; i < 12; i++) {
			BYTE nbSeed = (randomorg >> (i * 4)) & 0xff;
			veh->m_aLicensePlateText[i] = (nbSeed % 26) + 10;
		}

		veh->m_aPlateTextTint[0] = 0xff;
		veh->m_aPlateTextTint[1] = 0xff;
		veh->m_aPlateTextTint[2] = 0xff;
		veh->m_aPlateTextTint[3] = 0xff;
		veh->m_nbPlateTexIndex = 0;

		veh->m_aLettersOffset[0] = 0.f;
		veh->m_aLettersOffset[1] = 0.f;

		veh->m_aLettersScale[0] = 1.f;
		veh->m_aLettersScale[1] = 1.f;

	}
	else {

		for (char i = 0; i < 8; i++)
			veh->m_aLicensePlateText[i] = (randomorg >> 2) & 0x1f;
		veh->m_nbCharCount = 8;
	}

}

void initNames() {
	//// carefully! Seeing this code may give you brain cancer
	//tModelNameId _police_models[]{ tModelNameId("police2"), tModelNameId("police"), tModelNameId("policeb"), tModelNameId("policea"), tModelNameId("police3"), tModelNameId("police4") };
	//tModelNameId _fbi_models[]{ tModelNameId("fbi") };
	//tModelNameId _taxi_models[]{ tModelNameId("taxi"),tModelNameId("taxi2"),tModelNameId("cabby") };
	//tModelNameId _limo_models[]{ tModelNameId("stretch") };
	//tModelNameId _official_models[]{ tModelNameId("pbus"), tModelNameId("dukes") };
	//tModelNameId _commercial_models[]{ tModelNameId("reversed2") };

	//police_models.changeCapacity(sizeof(_police_models) / sizeof(tModelNameId));
	//if (police_models.wCapacity) {
	//	memcpy(police_models.pNames, _police_models, sizeof(_police_models));
	//	police_models.wCount = police_models.wCapacity;
	//}

	//fbi_models.changeCapacity(sizeof(_fbi_models) / sizeof(tModelNameId));
	//if (fbi_models.wCapacity) {
	//	memcpy(fbi_models.pNames, _fbi_models, sizeof(_fbi_models));
	//	fbi_models.wCount = fbi_models.wCapacity;
	//}

	//taxi_models.changeCapacity(sizeof(_taxi_models) / sizeof(tModelNameId));
	//if (taxi_models.wCapacity) {
	//	memcpy(taxi_models.pNames, _taxi_models, sizeof(_taxi_models));
	//	taxi_models.wCount = taxi_models.wCapacity;
	//}

	//limo_models.changeCapacity(sizeof(_limo_models) / sizeof(tModelNameId));
	//if (limo_models.wCapacity) {
	//	memcpy(limo_models.pNames, _limo_models, sizeof(_limo_models));
	//	limo_models.wCount = limo_models.wCapacity;
	//}

	//official_models.changeCapacity(sizeof(_official_models) / sizeof(tModelNameId));
	//if (official_models.wCapacity) {
	//	memcpy(official_models.pNames, _official_models, sizeof(_official_models));
	//	official_models.wCount = official_models.wCapacity;
	//}

	//commercial_models.changeCapacity(sizeof(_commercial_models) / sizeof(tModelNameId));
	//if (commercial_models.wCapacity) {
	//	memcpy(commercial_models.pNames, _commercial_models, sizeof(_commercial_models));
	//	commercial_models.wCount = commercial_models.wCapacity;
	//}


	//std::ifstream fIn("common/data/licenceplates.txt");
	//if (fIn.is_open()) {
	//	std::string tempstring;

	//	nameIdGroup *pCurrGroup = NULL;

	//	tempstring = "police";

	//	//while (!fIn.eof()) {
	//	police_models.changeCapacity(police_models.wCapacity+ 1);
	//	police_models.pNames[police_models.wCount] = tModelNameId("police");
	//	police_models.wCount++;
	//	while (false) {
	//		fIn >> tempstring;

	//		if (!pCurrGroup) {
	//			if (tempstring == "<police>") pCurrGroup = &police_models;
	//			else if (tempstring == "<fbi>") pCurrGroup = &fbi_models;
	//			else if (tempstring == "<taxi>") pCurrGroup = &taxi_models;
	//			else if (tempstring == "<limo>") pCurrGroup = &limo_models;
	//			else if (tempstring == "<official>") pCurrGroup = &official_models;
	//			else if (tempstring == "<commercial>") pCurrGroup = &commercial_models;
	//			else MessageBoxA(NULL, tempstring.c_str(), NULL, 0x10);
	//		}
	//		else if(tempstring == "<end>") pCurrGroup = NULL;
	//		else {
	//			if (pCurrGroup->wCapacity == pCurrGroup->wCount) pCurrGroup->changeCapacity(pCurrGroup->wCapacity + 1);
	//		
	//	//		MessageBoxA(NULL, tempstring.c_str(), NULL, 0x10);
	//			
	//			pCurrGroup->pNames[pCurrGroup->wCount] = tModelNameId(tempstring.c_str());
	//			pCurrGroup->wCount++;
	//		}

	//	}
	//	fIn.close();
	//}
	//else {
	//	MessageBoxA(NULL, "Brain cancer. Exit", NULL, 0x10);
	//	ExitProcess(0);
	//}


}

int DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	//int* new3 = new int[0x100];
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		srand(time(NULL));
		initNames();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return 1;
}

