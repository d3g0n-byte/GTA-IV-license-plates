// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "Hooking.Patterns-master/Hooking.Patterns.h"
#include "injector/injector.hpp"
#include "CBaseVeh.h"
#include "helpers.h"
#include "hooks.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>

#include "garage_car_patch.h"
#include "patch_struct_sizes.h"
#include "new_shader_variables.h"
#include "car_plate_generator.h"
#include "engine_crankshaft.h"
#include "rhino_track.h"
#include "new_parts.h"

#include "treeswind.h"

void initMain() {
	hooks::init();
	garage_car_patch::init();
	patch_struct_size::init();
	new_shader_variables::init();
	car_plate_generator::init();
	engine_crankshaft::init();
	rhine_track::init();
	//new_parts::init();

	// ???
	//treeswind::init();
}
//
//size_t addrUnk;
//size_t setDayNightEffectOrigCall;
//
//void _cdecl setColorizeGlobalParam(float* pVec) {
//	//MessageBoxA(NULL, "cock", NULL, 0x10);
//
//
//}
//
//DWORD g_windSeed = 0;
//
//int _cdecl sub_89EFA0(DWORD _pfn, float* pVec4) {
//	//injector::WriteMemory(pVec4, pVec4[0] + 0.1f);
//	//injector::WriteMemory(pVec4 + 1, pVec4[1] + 0.1f);
//	//injector::WriteMemory(pVec4 + 2, pVec4[2] + 0.1f);
//	//injector::WriteMemory(pVec4 + 3, pVec4[3] + 0.1f);
//	g_windSeed++;
//	injector::WriteMemory(pVec4, g_windSeed * 0.1f);
//	injector::WriteMemory(pVec4 + 1, g_windSeed * 0.1f);
//	injector::WriteMemory(pVec4 + 2, g_windSeed * 0.1f);
//	injector::WriteMemory(pVec4 + 3, g_windSeed * 0.1f);
//
//	return ((int(_cdecl*)(DWORD, float*))addrUnk)(_pfn, pVec4);
//	return NULL;
//}
//
//float* pColorizeVec = *(float**)hook::pattern("E8 ? ? ? ? 6A 01 6A 02 E8 ? ? ? ? 6A 01 6A 08").get_first(0x54 + 4);
//
//void _cdecl fake_sub_411680(int a, int b, int c) {
//
//
//	((void(_cdecl*)(int, int, int))setDayNightEffectOrigCall)(a, b, c);
//
//}
//
//void setDayNightEffectHook() {
//	hook::pattern p("F3 0F 11 05 ? ? ? ? E8 ? ? ? ? 83 C4 10 8B E5");
//
//	if (!p.empty()) {
//		size_t addr = (size_t)p.get_first(8);
//
//		setDayNightEffectOrigCall = (*(size_t*)(addr + 1) + (addr + 5));
//
//		injector::MakeCALL(addr, fake_sub_411680);
//	}
//	else {
//		MessageBoxA(NULL, "!TEST2", NULL, 0x10);
//		ExitProcess(0);
//	}
//}
//
//void test() {
//	setDayNightEffectHook();
//
//	hook::pattern p("E8 ? ? ? ? 83 C4 08 E9 ? ? ? ? 83 FF 02 75 A2 8D 44 24 13 50");
//	if (!p.empty()) {
//		//addrUnk = injector::ReadMemory<size_t>(p.get_first(1)) + (size_t)p.get_first() + 5;
//		size_t addr = (size_t)p.get_first();
//
//		addrUnk = (*(size_t*)(addr + 1) + (addr + 5));
//		
//	//	injector::MakeCALL(p.get_first(), sub_89EFA0);
//	}
//	else {
//		MessageBoxA(NULL, "!TEST", NULL, 0x10);
//		ExitProcess(0);
//	}
//
////	hook::pattern p("E8 ? ? ? ? 8B 15 ? ? ? ? 6A 05 6A 01 68 ? ? ? ? 52 E8 ? ? ? ? A1 ? ? ? ? 6A 05 6A 01 68 ? ? ? ? 50 E8 ? ? ? ? 83 C4 34 C3 ");
////	if (!p.empty()) {
////		//size_t addr = *(size_t*)p.get_first(1) + (size_t)p.get_first() + 5;
////		//injector::MakeJMP(addr, setColorizeGlobalParam);
////
////		addrUnk = injector::ReadMemory<size_t>(p.get_first(1)) + p.get_first(1) + 5;
////
////
////	}
////	else {
////	MessageBoxA(NULL, "!TEST", NULL, 0x10);
////	ExitProcess(0);
////}
//
//	//hook::pattern pp[]{
//	//	hook::pattern("68 ? ? ? ? F3 0F 11 44 24 ? E8 ? ? ? ? 83 C4 08 E9 ? ? ? ? 83 FF 02 75 A2 8D 44 24 13 50 68 ? ? ? ?"),
//	//	hook::pattern("68 ? ? ? ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? E8 ? ? ? ? 83 C4 08 68 ? ? ? ? E8 ? ? ? ? 8D 44 24 17 50 68 ? ? ? ? C6 44 24 ? ? "),
//	//	hook::pattern("68 ? ? ? ? F3 0F 11 44 24 ? E8 ? ? ? ? 83 C4 08 8B 4D 0C 33 FF 66 39 7C CB ? 76 5C 8B 55 0C 8B 44 D3 10 8B 34 F8 0F B6 46 62 "),
//	//	hook::pattern("68 ? ? ? ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? F3 0F 11 44 24 ? E8 ? ? ? ? 83 C4 08 E9 ? ? ? ? 66 39 7C C3 ? 89 7C 24 10 0F 86 ? ? ? ? 8B 7D 10 EB 06 "),
//	//	hook::pattern("E8 ? ? ? ? 6A 01 6A 02 E8 ? ? ? ? 6A 01 6A 08 E8 ? ? ? ? 83 C4 1C E8 ? ? ? ? 83 FF FF 8B D8 75 09 8B 0D ? ? ? ? 51 EB 07 "),
//	//};
//
//	//for (size_t i = 0; i < 1; i++) {
//	//	hook::pattern p = pp[i];
//	//	if (!p.empty()) {
//	//		size_t addr = (size_t)p.get_first(1);
//	//		injector::WriteMemory(addr, setColorizeGlobalParam);
//
//	//	}
//	//	else {
//	//		MessageBoxA(NULL, "!TEST", NULL, 0x10);
//	//		ExitProcess(0);
//	//	}
//
//	//}
//
//}

void test() {
	hook::pattern p4;
	size_t addr;

	// preCE
	p4 = hook::pattern("C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? 5E 59 C3");
	if (!p4.empty()) {
		addr = (size_t)p4.get_first(3);
	}
	// CE
	else {
		p4 = hook::pattern("C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? E8 ? ? ? ? 8B 44 24 44");
		if (p4.empty()) return;
		addr = (size_t)p4.get_first(4);
	}

	DWORD* pU = *(DWORD**)addr;
	for (size_t i = 0; i < (0x180c / 4) / 3; i++) {
		float* pVertex = (float*)(pU + i * 3);
		if (i < 32) {
			if (pVertex[1] < 0.f) {
				injector::WriteMemory<float>(pVertex, 0.f);
				injector::WriteMemory<float>(pVertex + 2, 0.f);
			}
		}
		else injector::WriteMemory<float>(pVertex + 1, (pVertex[1] - 0.16037700f) * 1.78f - 1.f);
	}

	return;

//	std::ofstream fOut;
//
//	fOut.open("idx.txt");
//	fOut << std::setprecision(8) << std::fixed;
//	//idx?
//	hook::pattern p("8B 04 9D ? ? ? ? 8B 57 10 8D 0C 40 F3 0F 10 04 8A 8D 04 8A 8B 4C 24 08 F3 0F 11 44 24 ? F3 0F 10 40 ? F3 0F 11 44 24 ? F3 0F 10 40 ? 8D 44 24 30 50 51");
//	size_t addr = (size_t)p.get_first();
//	DWORD* pUnkArr = *(DWORD**)(addr + 3);
//	for (size_t i = 0; i < 5952; i++) {
//	//	injector::WriteMemory(pUnkArr + i, 0);
//		fOut << *(DWORD*)(pUnkArr + i);
//		if (!((i + 1) % 16)) fOut << std::endl;
//		else  fOut << " ";
//	}
//	fOut.close();
//
//	fOut.open("uv0.txt");
//	fOut << std::setprecision(8)<<std::fixed;
//			// uvs?
//	hook::pattern p2("C7 44 84 ? ? ? ? ? 8B 4C 24 44 6A 05 BE ? ? ? ? 89 74 8C 38 01 7C 24 4C 01 7C 24 48 57 57 8D 4C 24 14 E8 ? ? ? ? 8B 54 24 44 64 8B 0D ? ? ? ? C7 44 94 ? ? ? ? ? 8B 44 24 44 8B 11");
//	addr = (size_t)p2.get_first();
//
//	pUnkArr = *(DWORD**)(addr + 4);
//	for (size_t i = 0; i < 4104 / 4; i++) {
//		//injector::WriteMemory<float>(pUnkArr + i, *(float*)(pUnkArr + i) + (rand() % 100) * 0.01);
//		//	injector::WriteMemory(pUnkArr + i, 0);
//		fOut << *(float*)(pUnkArr + i);
//		if (!((i + 1) % 2)) fOut << std::endl;
//		else  fOut << "\t";
//	}
//
//	fOut.close();
//
//	fOut.open("uv1.txt");
//	fOut << std::setprecision(8) << std::fixed;
//	// ?
//	hook::pattern p3("C7 44 94 ? ? ? ? ? 8B 44 24 44 8B 11 8B 4A 08 89 74 84 34 01 7C 24 48 01 7C 24 44 6A 00 C6 44 24 ? ? 8B 01 8B 50 08 ");
//	addr = (size_t)p3.get_first();
//
//	pUnkArr = *(DWORD**)(addr + 4);
//	for (size_t i = 0; i < 4104 / 4; i++) {
//	//	injector::WriteMemory<float>(pUnkArr + i, 0);
//		//	injector::WriteMemory(pUnkArr + i, *(float*)(pUnkArr + i) + (rand() %100) * 0.01);
//		fOut << *(float*)(pUnkArr + i);
//		if (!((i + 1) % 2)) fOut << std::endl;
//		else  fOut << "\t";
//	}
//
//	fOut.close();
//	// ?
//	hook::pattern p4("C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? 5E 59 C3");
//	addr = (size_t)p4.get_first();
//
////	fOut.open("pos.txt");
////	fOut << std::setprecision(8)<<std::fixed;
//	
//	pUnkArr = *(DWORD**)(addr + 3);
//
//
//
//	for (size_t i = 0; i < (0x180c / 4); i+=3) {
//		float* pVertex = (float*)(pUnkArr + i);
//		if (pVertex[1] < 0.f) {
//			pVertex[0] = pVertex[2] = 0.0f;
//		}
//
//		//injector::WriteMemory<float>(pUnkArr + i, *(float*)(pUnkArr + i) + (rand() % 100) * 0.01f);
//		//fOut << *(float*)(pUnkArr + i);
//		//if (!((i + 1) % 3)) fOut << std::endl;
//		//else  fOut << "\t";
//	}
//	//fOut.close();
//	//ExitProcess(0);
}

int DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	//	test();
		initMain();
		//test();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return 1;
}

