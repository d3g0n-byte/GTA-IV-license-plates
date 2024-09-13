#include <Windows.h>
#include "Hooking.Patterns-master/Hooking.Patterns.h"
#include "modelinfo.h"

CBaseModelInfo** g_ppModelInfoPtrs = *(CBaseModelInfo***)hook::pattern("8B 14 8D ? ? ? ? 8B 8A ? ? ? ? 6A 01 6A 00 6A 00 6A 00 50 53 8D 44 24 28").get_first(3); // CBaseModelInfo* g_ppModelInfoPtrs[31000]
