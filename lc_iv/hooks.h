#pragma once

class CVehicle;
namespace hooks {
	namespace CVehicleFactoryNY__create { void add(void(*fn)(CVehicle* baseVeh)); }
	namespace CVehicle__CVehicle { void add(void(*fn)(CVehicle* baseVeh)); }
	namespace CAutomobile__processControl { void add(void(*fn)(CVehicle* baseVeh)); }
	namespace CAutomobile__prerender { void add(void(*fn)(CVehicle* baseVeh)); }
	void init();
}