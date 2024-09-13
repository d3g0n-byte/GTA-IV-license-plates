//#include <Windows.h>
//#include "injector/injector.hpp"
//#include "Hooking.Patterns-master/Hooking.Patterns.h"
//
//float* pWindVec = *(float**)hook::pattern(
//	"F3 0F 10 05 ? ? ? ? F3 0F 11 44 24 ? F3 0F 10 05 ? ? ? ? F3 0F 11 44 24 ? F3 0F 10 05 ? ? ? ? 8D 44 24 20 50 F3 0F 11 44 24 ? F3 0F 10 05 ? ? ? ? 68 ? ? ? ? F3 0F 11 44 24 ? E8 ? ? ? ? 83 C4 08 E9 ?")
//.get_first(4); // unused in gta_trees.fxc
//
//float* pWindSpeed = *(float**)hook::pattern(
//	"D9 1D ? ? ? ? 83 05 ? ? ? ? ? 33 D2 38 15 ? ? ? ? 0F 84 ? ? ? ? F3 0F 10 15 ? ? ? ? F3 0F 10 0D ? ? ? ? 88 15 ? ? ? ? 33 C9 B8 ? ? ? ? 8B FF 83 CE FF 39 70 FC 74 33 8B 1D ? ? ? ?")
//	.get_first(2); // noise
//
//size_t addrUnk;
//
//int _cdecl sub_89EFA0(DWORD _pfn, float* pVec4) {
//	float vWindVals[]{ pWindVec[0],pWindVec[1] ,pWindVec[2], *pWindSpeed };
//
//	return ((int(_cdecl*)(DWORD, float*))addrUnk)(_pfn, vWindVals);
//	return NULL;
//}
//
//void test() {
//
//	hook::pattern p("E8 ? ? ? ? 83 C4 08 68 ? ? ? ? E8 ? ? ? ? 8D 44 24 17 50 68 ? ? ? ? C6 44 24 ? ? E8 ? ? ? ?");
//	if (!p.empty()) {
//		size_t addr = (size_t)p.get_first();
//
//		addrUnk = (*(size_t*)(addr + 1) + (addr + 5));
//
//		injector::MakeCALL(addr, sub_89EFA0);
//	}
//	else {
//		MessageBoxA(NULL, "!TEST_m", NULL, 0x10);
//		ExitProcess(0);
//	}
//}
//
//
//
//namespace treeswind {
//	void init() {
//		test();
//	}
//}