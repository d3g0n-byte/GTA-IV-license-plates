#pragma once

#include "Windows.h"
DWORD atStringHash(const char* pszString, DWORD dwExistingHash);

	template<typename T> size_t getThisCallFuncAddr(T func) { return (size_t)(void*&)func; }
	float smoothValue(float val, float old, float scale);
