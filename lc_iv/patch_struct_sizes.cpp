#include "injector/injector.hpp"
#include "Hooking.Patterns-master/Hooking.Patterns.h"
#include "CBaseVeh.h"

void patchBaseVehSize() {
	hook::pattern p = hook::pattern("68 ? 20 00 00 68 ? ? ? ? 68 ? ? ? ? 8B C8 E8 ? ? ? ?");
	//hook::pattern p = hook::pattern("68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 8B C8 E8 ? ? ? ?");
	if (!p.empty()) {

		size_t* pSize = p.get_first<size_t>(1);

		// align
		if constexpr (sizeof(CExtVeh) < 0x10) injector::WriteMemory(pSize, (sizeof(CExtVeh) + 0x3) & ~0x3);
		else injector::WriteMemory(pSize, (sizeof(CExtVeh) + 0xf) & ~0xf);

	}
	else {
		MessageBoxW(NULL, L"Не найдено область памяти с размером базового автомобиля. Процесс будет закрыт.", NULL, 0x10);
		ExitProcess(0);
	}
}

namespace patch_struct_size {
	void init() {
		patchBaseVehSize();
	}
}