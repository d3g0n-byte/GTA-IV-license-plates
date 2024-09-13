#pragma once
#include "Windows.h"

#define PLATE_NUM_CHARS 12

class CVehicle {
public:
	BYTE __0[0x20d0];
};

class CExtVeh : public CVehicle {
public:
	char m_aLicensePlateText[PLATE_NUM_CHARS];
	BYTE m_nbCharCount;
	BYTE m_aPlateTextTint[4];
	BYTE m_nbPlateTexIndex; // if m_nbPlateTexIndex lt g_nbPlateTexCount then m_nbPlateTexIndex = g_nbPlateTexCount
	float m_aLettersScale[2];
	float m_aLettersOffset[2];
	float m_fCrankshaftRot;
	float m_aTrackSpeed[2]; // l - r
	float m_aFakeTrackWheelsRot[2];

	union {
		struct {
			BYTE bHaveNewParts : 1;
		};
		BYTE value;
	} m_ExtFlags;

	struct {
		BYTE m_nbFakeTankLWheels[0x10];
		BYTE m_nbFakeTankRWheels[0x10];
	} m_newPartsIds;
};