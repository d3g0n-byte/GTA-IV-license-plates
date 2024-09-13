#include "timer.h"

namespace timer {
	float* g_pTimestep = (float*)injector::ReadMemory<size_t>(hook::pattern("F3 0F 10 05 ? ? ? ? 8D 4C 24 10 51 8B CE F3 0F 11 44 24 ? 76 2B FF D2 F3 0F 10 08 F3 0F 10 40 ? 0F 28 D1").get_first(4));
}