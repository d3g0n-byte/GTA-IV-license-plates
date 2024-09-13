#include "CBaseVeh.h"
#include "helpers.h"
#include "injector/injector.hpp"
#include "Hooking.Patterns-master/Hooking.Patterns.h"


struct tNewVehShaderParams {
	float m_vEngineBeltOffset[2];
	int m_nEngineBeltIndex;
	float m_vRhinoTrackOffset[2];
	int m_nRhinoTrackIndex;

	char m_aLicensePlateText[PLATE_NUM_CHARS];
	int m_nLicensePlateTextIndex0;
	int m_nLicensePlateTextIndex1;
	int m_nLicensePlateTextIndex2;

	float m_vLicensePlateValues[4]; // x - offset
	int m_nLicensePlateValuesIndex;

	BYTE m_aLettersTint[4];
	int m_nLettersTintIndex;

	float m_aLettersScale[2];
	int m_nLettersScaleIndex;

	float m_aLettersOffset[2];
	int m_nLettersOffsetIndex;

	BYTE nbPlateTextureNum;
	int m_nPlateDiffuseTxdIndex;
	int m_nPlateNormalTxdIndex;
	int m_nPlateSpecTxdIndex;

	// wtf!?
	float m_fCringeLevel;
	int m_nCringeIndex;
};

void patchCVehFxSize() {
	hook::pattern p = hook::pattern("81 C6 ? ? ? ? 83 E6 F0 56 89 44 24 20 89 4C 24 28");
	if (p.size() == 1) {
	//if (!p.empty()) {
		size_t * pSize = p.get_first<size_t>(2);
		injector::WriteMemory(pSize, *pSize + ((sizeof(tNewVehShaderParams) * 2 + 0xf) & ~0xf));

	}
	else {
		MessageBoxW(NULL, L"Не удалось найти область памяти с базовым размером CCustomShaderEffectVehFx. Процесс будет закрыт.", NULL, 0x10);
		ExitProcess(0);
	}
}

size_t g_updateOrigCall = NULL;
size_t g_setShadersVarsOrigCall = NULL;
size_t g_registerShadersVarsOrigCall = NULL;

size_t* g_pVmt = NULL;

void findVmt() {
	//	//hook::pattern p = hook::pattern("8B 44 24 04 56 50 8B F1 E8 ? ? ? ? 33 C0 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? 8B C6 5E C2 04 00");
	//	hook::pattern p = hook::pattern("C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ?");
	///*	if (p.size() == 1)
	//		g_pVmt = *p.get(0).get<size_t>(2);
	//	else */if (!p.empty()) {
	//		//hook::pattern p2 = hook::pattern("8B 44 24 04 56 50 8B F1 E8 ? ? ? ? 33 C0 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? 8B C6 5E C2 04 00");
	//		hook::pattern p2 = hook::pattern("E8 ? ? ? ? 8B F8 EB 02 33 FF 85 DB"); // fn
	//		if (p2.size() == 1) {
	//			size_t start = (size_t)p2.get_first<size_t>();
	//			size_t end = start + 0x37;
	//			for (int i = 0; i < p2.size(); i++) {
	//				size_t addr = (size_t)p.get(i).get<size_t>();
	//				if (addr >= start && addr <end)
	//					g_pVmt = addr + 2;
	//			}
	//		}
	//		else return false;
	//
	//	}
		//else return false;
	hook::pattern p = hook::pattern("8B 44 24 04 56 50 8B F1 E8 ? ? ? ? 33 C0 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? 89 86 ? ? ? ? C7 06 ? ? ? ? C7 86 ? ? ? ? ? ? ? ? 8B C6 5E C2 04 00");
	if (p.size() == 1)
		g_pVmt = *p.get(0).get<size_t*>(2 + 0x27);


	else {
		MessageBoxW(NULL, L"Не найдено виртуальную таблицу CCustomShaderEffectVehFx. Процесс будет закрыт", NULL, 0x10);
		ExitProcess(0);
	}

}


size_t tShaderGroup__setGroupGroupVarFloatAddr = (size_t)hook::pattern("53 56 57 8B F9 0F B7 5F 0C 33 F6 85 DB 7E 4B F3 0F 10 44 24 ? 55 8B 6C 24 14 8D 9B ? ? ? ? 8B").get_first();
size_t tShaderGroup__setGroupGroupVarVector4Addr =
(size_t)hook::pattern("55 56 57 8B F9 0F B7 6F 0C 33 F6 85 ED 7E 39 53 8B 5C 24 18 8B 4C 24 14 8B 47 38 8B 54 C8 F8 8B 0C B2 85 C9 74 1A 8B 47 08 8B 04 B0 6A 05 6A 01 6A 10").get_first();
size_t tShaderGroup__getGroupGroupVarIndexAddr = (size_t)hook::pattern("55 8B EC 83 EC 18 53 8B D9 66 8B 43 14 66").get_first();

struct tVector4 {
	float x, y, z, w;
}
;struct tTexPC {
	//BYTE __0[0x50];
	BYTE __0[0x4];
	BYTE __4[0x4];
	BYTE __8[0x2];
	WORD m_wUsageCount;
	BYTE __c[0x4];
	BYTE __10[0x40];
};

struct tShader {
	struct tEntry {
		union {
			tVector4* pVec;
			tTexPC* pTxd;
		};
	};

	BYTE __0[0x14];
	tEntry* m_pEntries;
	void* m_pEffect;
	DWORD m_dwParamsCount;
	DWORD _f20;
	BYTE* m_pTypes;
	DWORD m_dwNameHash;
	BYTE __2c[0x8];
	DWORD* m_pNames;// hash!

};

#define MAX_PLATE_COUNT 0x10

tTexPC* g_apPlatesTxd_d[MAX_PLATE_COUNT];
tTexPC* g_apPlatesTxd_n[MAX_PLATE_COUNT];
tTexPC* g_apPlatesTxd_s[MAX_PLATE_COUNT];
BYTE g_nbPlateTexCount = 0;

struct tShaderGroup_obj1 {
	DWORD* _f0;
	BYTE __4[4];
};

struct tShaderGroup {
	BYTE __0[0x8];
	tShader** m_ppShaders;
	WORD m_wShaderCount;
	BYTE __e[0x2];
	BYTE __10[0x10];
	BYTE __20[0x10];
	BYTE __30[0x8];
	tShaderGroup_obj1* m_shaderGroupHashes;
	WORD m_wShaderGroupHashesCount;
	BYTE __3e[0x2];

	void setGroupGroupVarFloat(int nParamIndex, float fValue) {
		((void(__thiscall*)(tShaderGroup*, int, float))tShaderGroup__setGroupGroupVarFloatAddr)(this, nParamIndex, fValue);
	}

	void setGroupGroupVarFloat_2(int nParamIndex, float fValue) {
		for (WORD i = 0; i < m_wShaderCount; i++) {
			if (m_shaderGroupHashes[nParamIndex - 1]._f0[i]) {
				auto pShader = m_ppShaders[i];
				auto pUnk = m_shaderGroupHashes[nParamIndex - 1]._f0[i];


			}

		}
	}

	void setGroupGroupVarVector4(int nParamIndex, void* pValue) {
		((void(__thiscall*)(tShaderGroup*, int, void*)) tShaderGroup__setGroupGroupVarVector4Addr)(this, nParamIndex, pValue);
	}

	int getGroupGroupVarIndex(const char* pszParamName, BYTE bRequired) {
		return ((int(__thiscall*)(tShaderGroup*, const char*, BYTE)) tShaderGroup__getGroupGroupVarIndexAddr)(this, pszParamName, bRequired);
	}

	void setTexture(int shaderGroupIndex, tTexPC* pTex) {
	//void setTexture(DWORD dwParamNameHash, DWORD fxcHash, tTexPC* pTex) {
		for (WORD i = 0; i < m_wShaderCount; i++) {
			auto j = m_shaderGroupHashes[shaderGroupIndex - 1]._f0[i];
			if (j) {
				j--;
				if (m_ppShaders[i]->m_pEntries[j].pTxd)
					m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount--;

				m_ppShaders[i]->m_pEntries[j].pTxd = pTex;
				if (!m_ppShaders[i]->m_pEntries[j].pTxd) {
					MessageBoxA(NULL, "m_ppShaders[i]->m_pEntries[j].pTxd is NULL", NULL, 0x10);
					ExitProcess(0);
				}
				if (m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount == 0xffff) {
					MessageBoxA(NULL, "m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount is full", NULL, 0x10);
					ExitProcess(0);
				}
				m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount++;

			}

		}

			//for (WORD i = 0; i < m_wShaderCount; i++) {
		//	if (fxcHash != m_ppShaders[i]->m_dwNameHash) continue;

		//	for (DWORD j = 0; j < m_ppShaders[i]->m_dwParamsCount; j++)
		//		if (!m_ppShaders[i]->m_pTypes[j] && m_ppShaders[i]->m_pNames[j] == dwParamNameHash) {
		//			
		//			if (m_ppShaders[i]->m_pEntries[j].pTxd)
		//				m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount--;

		//			m_ppShaders[i]->m_pEntries[j].pTxd = pTex;
		//			if (!m_ppShaders[i]->m_pEntries[j].pTxd) {
		//				MessageBoxA(NULL, "m_ppShaders[i]->m_pEntries[j].pTxd is NULL", NULL, 0x10);
		//				ExitProcess(0);
		//			}
		//			if (m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount == 0xffff) {
		//				MessageBoxA(NULL, "m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount is full", NULL, 0x10);
		//				ExitProcess(0);
		//			}
		//			m_ppShaders[i]->m_pEntries[j].pTxd->m_wUsageCount++;
		//			break;

		//		}
		//}
	}

};

class CCustomShaderVehFx {
public:
	size_t __0;
	WORD m_wSize;
	BYTE __6[0x70 - 6];
	tShaderGroup* m_pShaderGroup;
	BYTE __74[0x6c];
	WORD m_wSize2; // ???
	

	char update(CExtVeh* veh) {
		char ret = ((char(__thiscall*)(CCustomShaderVehFx*, CExtVeh*))g_updateOrigCall)(this, veh);
		//return ret;

		tNewVehShaderParams* params = (tNewVehShaderParams*)((size_t)this + m_wSize - sizeof(tNewVehShaderParams));

		params->m_vEngineBeltOffset[1] = 0.f;
		params->m_vEngineBeltOffset[0] = veh->m_fCrankshaftRot * 0.15915507752f; //+= veh->m_transmission.m_fRevs * timer::ms_fTimeStep * 5.f;//0.2f;

		params->m_vRhinoTrackOffset[0] = veh->m_aFakeTrackWheelsRot[0] * 0.15915507752f;
		params->m_vRhinoTrackOffset[1] = veh->m_aFakeTrackWheelsRot[1] * 0.15915507752f;

		for (char i = 0; i < PLATE_NUM_CHARS; i++)
			params->m_aLicensePlateText[i] = veh->m_aLicensePlateText[i];

		params->m_vLicensePlateValues[0] = PLATE_NUM_CHARS - veh->m_nbCharCount;
		params->m_vLicensePlateValues[1] = veh->m_nbCharCount;

		if (veh->m_nbPlateTexIndex >= g_nbPlateTexCount) {
			MessageBoxA(NULL, "veh->m_nbPlateTexIndex >= g_nbPlateTexCount. Будет установлено стандартное значение", NULL, 0x10);
			veh->m_nbPlateTexIndex = g_nbPlateTexCount - 1;
		}
		params->nbPlateTextureNum = veh->m_nbPlateTexIndex;

		memcpy(params->m_aLettersTint, veh->m_aPlateTextTint, 4);

		params->m_aLettersOffset[0] = veh->m_aLettersOffset[0];
		params->m_aLettersOffset[1] = veh->m_aLettersOffset[1];
		params->m_aLettersScale[0] = veh->m_aLettersScale[0]; // *(float*)((BYTE*)veh + 0x10e0 + 4) * 2;
		params->m_aLettersScale[1] = veh->m_aLettersScale[1];

		return ret;
	}

	int setShadersVars() {
		tShaderGroup* sg = m_pShaderGroup;

		int ret = ((int(__thiscall*)(CCustomShaderVehFx*))g_setShadersVarsOrigCall)(this);
		//return ret;

		tNewVehShaderParams* params = (tNewVehShaderParams*)((size_t)this + m_wSize - sizeof(tNewVehShaderParams));



		if (params->m_nEngineBeltIndex > 0)
			sg->setGroupGroupVarFloat(params->m_nEngineBeltIndex, params->m_vEngineBeltOffset[0]);

		if (params->m_nRhinoTrackIndex > 0) {
			float p[4];
			p[0] = params->m_vRhinoTrackOffset[0];
			p[1] = params->m_vRhinoTrackOffset[1];
			p[2] =0.f;
			p[3] = 0.f;
			sg->setGroupGroupVarVector4(params->m_nRhinoTrackIndex, p);
		}

		if (params->m_nLicensePlateTextIndex0 > 0 && params->m_nLicensePlateTextIndex1 > 0&& params->m_nLicensePlateTextIndex2 > 0) {
			float p[4];
			p[0] = params->m_aLicensePlateText[0];
			p[1] = params->m_aLicensePlateText[1];
			p[2] = params->m_aLicensePlateText[2];
			p[3] = params->m_aLicensePlateText[3];
			sg->setGroupGroupVarVector4(params->m_nLicensePlateTextIndex0, p);

			p[0] = params->m_aLicensePlateText[4];
			p[1] = params->m_aLicensePlateText[5];
			p[2] = params->m_aLicensePlateText[6];
			p[3] = params->m_aLicensePlateText[7];
			sg->setGroupGroupVarVector4(params->m_nLicensePlateTextIndex1, p);

			p[0] = params->m_aLicensePlateText[8];
			p[1] = params->m_aLicensePlateText[9];
			p[2] = params->m_aLicensePlateText[10];
			p[3] = params->m_aLicensePlateText[11];
			sg->setGroupGroupVarVector4(params->m_nLicensePlateTextIndex2, p);
		}

		if (params->m_nLicensePlateValuesIndex > 0) {
		//	sg->setGroupGroupVarVector4(params->m_nLicensePlateValuesIndex, &params->m_vLicensePlateValues);
			sg->setGroupGroupVarVector4(params->m_nLicensePlateValuesIndex, params->m_vLicensePlateValues);
		}

		if (params->m_nLettersTintIndex > 0) {
			float aTint[4]{params->m_aLettersTint[0] * 0.00392156f,params->m_aLettersTint[1] * 0.00392156f,params->m_aLettersTint[2] * 0.00392156f,params->m_aLettersTint[3] * 0.00392156f };
			sg->setGroupGroupVarVector4(params->m_nLettersTintIndex, aTint);
		}

		if (params->m_nLettersScaleIndex > 0) {
			float values[4]{ params->m_aLettersScale[0],params->m_aLettersScale[1],1.f,1.f };
			sg->setGroupGroupVarVector4(params->m_nLettersScaleIndex, values);
		}

		if (params->m_nLettersOffsetIndex > 0) {
			float values[4]{ params->m_aLettersOffset[0],params->m_aLettersOffset[1],1.f,1.f };
			sg->setGroupGroupVarVector4(params->m_nLettersOffsetIndex, values);
		}

		if (params->m_nPlateDiffuseTxdIndex > 0)
			sg->setTexture(params->m_nPlateDiffuseTxdIndex, g_apPlatesTxd_d[params->nbPlateTextureNum]);
		if (params->m_nPlateNormalTxdIndex > 0)
			sg->setTexture(params->m_nPlateNormalTxdIndex, g_apPlatesTxd_n[params->nbPlateTextureNum]);
		if (params->m_nPlateSpecTxdIndex > 0)
			sg->setTexture(params->m_nPlateSpecTxdIndex, g_apPlatesTxd_s[params->nbPlateTextureNum]);

		return ret;
	}

	char registerShadersVars(void* pDrawable) {
		char ret = ((char(__thiscall*)(CCustomShaderVehFx*, void*))g_registerShadersVarsOrigCall)(this, pDrawable);
		//	return ret;

		tShaderGroup* sg = m_pShaderGroup;
		if (!sg)
			return ret;

		tNewVehShaderParams* params = (tNewVehShaderParams*)((size_t)this + m_wSize - sizeof(tNewVehShaderParams));
		memset(params, 0x0, sizeof(tNewVehShaderParams));

		params->m_nEngineBeltIndex = sg->getGroupGroupVarIndex("track", 0);
		params->m_nRhinoTrackIndex = sg->getGroupGroupVarIndex("rhinouv", 0);

		params->m_nLicensePlateTextIndex0 = sg->getGroupGroupVarIndex("char0123", 0);
		params->m_nLicensePlateTextIndex1 = sg->getGroupGroupVarIndex("char4567", 0);
		params->m_nLicensePlateTextIndex2 = sg->getGroupGroupVarIndex("char891011", 0);

		params->m_nLicensePlateValuesIndex = sg->getGroupGroupVarIndex("LicensePlateValues", 0);

		params->m_nLettersScaleIndex = sg->getGroupGroupVarIndex("lettersScale", 0);
		params->m_nLettersOffsetIndex = sg->getGroupGroupVarIndex("lettersOffset", 0);

		params->m_nPlateDiffuseTxdIndex  = sg->getGroupGroupVarIndex("plateTextureSampler", 0);
		params->m_nPlateNormalTxdIndex  = sg->getGroupGroupVarIndex("plateBumpSampler", 0);
		params->m_nPlateSpecTxdIndex  = sg->getGroupGroupVarIndex("plateSpecSampler", 0);

		params->m_nLettersTintIndex = sg->getGroupGroupVarIndex("lettersTint", 0);
		

		return ret;
	}
};


// find textures
namespace findTxd {
	size_t origCall;
	
	size_t addr_CTxdStore__getIndexByName;

	void initAddrs() {
		//hook::pattern p("E8 ? ? ? ? 83 C4 04 56 50 E8 ? ? ? ? 8B 4C 24 14");
		//if (!p.empty()) {
		//	size_t addr = (size_t)p.get_first();
		//	
		//	addr_CTxdStore__getIndexByName = injector::ReadMemory<size_t>(addr + 1) + addr + 5;
		//}
		//else {
		//	MessageBoxA(NULL, "addr_CTxdStore__getIndexByName", NULL, 0x10);
		//	ExitProcess(0);
		//}

	}

	//DWORD CTxdStore__getIndexByName(const char* pszName) {

	//}

	struct tTmp {
		size_t run(DWORD dwHash) {
			size_t(__thiscall * pgDict__lookup)(void*, DWORD) = (size_t(__thiscall*)(void*, DWORD))origCall;

			size_t ret = pgDict__lookup(this, dwHash);

			char msg[0x80];
			for (size_t i = 0; i < MAX_PLATE_COUNT; i++) {
				sprintf(msg, "plate%u_d", i);
				size_t diffuse = pgDict__lookup(this, atStringHash(msg, 0));
				sprintf(msg, "plate%u_n", i);
				size_t nrm = pgDict__lookup(this, atStringHash(msg, 0));
				sprintf(msg, "plate%u_s", i);
				size_t spec = pgDict__lookup(this, atStringHash(msg, 0));
				if (diffuse && nrm && spec) {
					g_apPlatesTxd_d[i] = (tTexPC*)diffuse;
					g_apPlatesTxd_n[i] = (tTexPC*)nrm;
					g_apPlatesTxd_s[i] = (tTexPC*)spec;
					g_nbPlateTexCount++;

					//sprintf(msg, "%p\t%p\t%p", diffuse, nrm, spec);
					//MessageBoxA(NULL, msg, NULL, 0x10);
				}
				else {
					break;
				}

			}
			if (!g_nbPlateTexCount) {
				MessageBoxW(NULL, L"Не найдно текстуры для номеров. Убедитесь, что в vehshare.#td есть diffuse, normal и specular в формате 'plate%u'", NULL, 0x10);
				ExitProcess(0);
			}

			sprintf(msg, "loaded %u plates", (DWORD)g_nbPlateTexCount);
			//MessageBoxA(NULL, msg, NULL, 0x10);
			//ExitProcess(0);
			return ret;
		}
		
	};

	void init() {
		hook::pattern p("E8 ? ? ? ? A3 ? ? ? ? B0 01 5E C3");
		if (!p.empty()) {
			size_t addr = (size_t)p.get_first();

			origCall = (*(size_t*)(addr + 1) + (addr + 5));

			injector::MakeCALL(addr, getThisCallFuncAddr(&tTmp::run));
		}
		else {
			MessageBoxA(NULL, "findTxd error", NULL, 0x10);
			ExitProcess(0);
		}
	}
}


namespace new_shader_variables {

	void init() {
		findVmt();
		patchCVehFxSize();

		g_updateOrigCall = g_pVmt[3];
		g_setShadersVarsOrigCall = g_pVmt[4];
		g_registerShadersVarsOrigCall = g_pVmt[1];

		injector::WriteMemory(g_pVmt + 3, getThisCallFuncAddr(&CCustomShaderVehFx::update));
		injector::WriteMemory(g_pVmt + 1, getThisCallFuncAddr(&CCustomShaderVehFx::registerShadersVars));
		injector::WriteMemory(g_pVmt + 4, getThisCallFuncAddr(&CCustomShaderVehFx::setShadersVars));

		findTxd::init();

	}
}