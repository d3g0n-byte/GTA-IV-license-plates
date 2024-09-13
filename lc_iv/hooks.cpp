#include <Windows.h>
#include "CBaseVeh.h"
#include "injector/injector.hpp"
#include "Hooking.Patterns-master/Hooking.Patterns.h"
#include "helpers.h"
#include <list>


namespace hooks {
	namespace CVehicleFactoryNY__create {
		size_t vmtOffset;

		std::list<void(*)(CExtVeh*)> fns;

		struct CTmp : CExtVeh {
			size_t* __vmt;

			void run() {
				((void(__thiscall*)(CExtVeh*))(*(void***)this)[vmtOffset])(this); // from game
				for (auto& fn : fns) fn(this);
			}

		};

		void add(void(*fn)(CVehicle* baseVeh)) { fns.emplace_back((void(*)(CExtVeh*))fn); }

		void hook() {
			hook::pattern p("8B 16 8B 42 38 57 8B CE FF D0 8B 16 8B 82 ? ? ? ? 8B CE FF D0");
			if (!p.empty()) {
				//size_t call_addr = (size_t)p.get_first(0x14);
				vmtOffset = *p.get_first<size_t>(0xc + 0x2) / sizeof(size_t);
				injector::WriteMemory(p.get_first(0xa), injector::ReadMemory<WORD>(p.get_first(0x12))); // mov ecx, esi
				injector::MakeCALL(p.get_first(0xa + 2), getThisCallFuncAddr(&CTmp::run));
				injector::MakeNOP(p.get_first(0xa + 2 + 0x5), 0x5);

				//char msg[0x1ff];
				//sprintf(msg, "%p", call_addr);
				//MessageBoxA(NULL, msg, NULL, 0x10);

				//injector::MakeCALL(call_addr, getThisCallFuncAddr(&CTmp::run));
			}
			else {
				MessageBoxW(NULL, L"хук для CVehicleFactoryNY::create не найдено", NULL, 0x10);
					ExitProcess(0);
			}
		}


	}

	namespace CAutomobile__processControl {
		std::list<void(*)(CVehicle*)> fns;
		void add(void(*fn)(CVehicle* baseVeh)) { fns.emplace_back(fn); }

		size_t origcall;

		struct tVeh : CVehicle {
			void run() {
				((void(__thiscall*)(CVehicle*))origcall)(this);
				for (auto& fn : fns) fn(this);
			}
		};

		void hook() {
			hook::pattern p = hook::pattern("E8 ? ? ? ? 8B CE E8 ? ? ? ? 8B CE E8 ? ? ? ? 8B CE E8 ? ? ? ? 8B CE E8 ? ? ? ? 8B 16 8B 82 ? ? ? ? 8B CE FF D0 8B 16");
			if (!p.empty()) {
				size_t addr = (size_t)p.get_first();
				origcall = injector::ReadMemory<size_t>(addr + 1) + addr + 5;
				injector::MakeCALL(addr, getThisCallFuncAddr(&tVeh::run));
			}
			else {
				MessageBoxW(NULL, L"хук для CAutomobile::processControl не найдено", NULL, 0x10);
				ExitProcess(0);
			}
		}
	}

	namespace CAutomobile__prerender {
		std::list<void(*)(CVehicle*)> fns;
		void add(void(*fn)(CVehicle* baseVeh)) { fns.emplace_back(fn); }

		size_t origcall;

		struct tVeh : CVehicle {
			int run() {
				auto ret = ((int(__thiscall*)(CVehicle*))origcall)(this);
				for (auto& fn : fns) fn(this);
				return ret;
			}
		};

		void hook() {
			hook::pattern p = hook::pattern("6A 00 6A 00 6A 01 51 D9 1C 24 6A 00 6A 51 8B CE E8 ? ? ? ? 8B CE E8 ? ? ? ? 5F 5E 5B 8B E5 5D C3");
			if (!p.empty()) {
				size_t addr = (size_t)p.get_first(0x17);
				origcall = injector::ReadMemory<size_t>(addr + 1) + addr + 5;
				injector::MakeCALL(addr, getThisCallFuncAddr(&tVeh::run));
			}
			else {
				MessageBoxW(NULL, L"хук для CAutomobile::prerender не найдено", NULL, 0x10);
				ExitProcess(0);
			}
		}
	}

	namespace CVehicle__CVehicle {

		std::list<void(*)(CVehicle*)> fns;
		size_t origcall;

		void add(void(*fn)(CVehicle* baseVeh)) { fns.emplace_back((void(*)(CVehicle*))fn); }

		struct tVeh : CVehicle {

			void run(float hp) {
				((void(__thiscall*)(CVehicle*, float))origcall)(this, hp);
				for (auto& fn : fns) fn(this);

			}
		};

		void hook() {
			hook::pattern p = hook::pattern("E8 ? ? ? ? C6 86 ? ? ? ? ? 89 9E ? ? ? ? 33 C0 8D");
			if (!p.empty()) {
				size_t addr = (size_t)p.get_first();
				origcall = injector::ReadMemory<size_t>(addr + 1) + addr + 5;
				injector::MakeCALL(addr, getThisCallFuncAddr(&tVeh::run));
			}
			else {
				MessageBoxW(NULL, L"хук для CVehicle::CVehicle не найдено", NULL, 0x10);
				ExitProcess(0);
			}


		}
	}

	//namespace loading {
	//	
	//	size_t findInDictionary_addr
	//	template <typename _Type> _Type* findInDictionary(void* pDict, DWORD dwHash) {

	//	}

	//	namespace texturesVehshare {

	//	}
	//}

	void init() {
		CVehicleFactoryNY__create::hook();
		CVehicle__CVehicle::hook();
		CAutomobile__processControl::hook();
		CAutomobile__prerender::hook();
	}
}