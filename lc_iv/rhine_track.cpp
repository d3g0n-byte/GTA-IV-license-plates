#include <Windows.h>
#include "hooks.h"
#include "CBaseVeh.h"
#include "timer.h"

class CHandlingData;

namespace rage {
	struct Vector3 {
		float x, y, z,w;
	};
}

class CWheel {
	/*
	*	свободное место в этом классе:
	*	pos		size
	*	0x6		0x2
	*	0x5c	0x4
	*	0x6c	0x4
	*	0xe8	0x8
	*	0xfc	0x4
	*
	*/
public:
	int m_nBoneId;					// 000-004 in the order from tVehicleStruct
	short m_nGroupId;				// 004-006 group for detach, needs to be higher than 0
	BYTE __6[2];						// used if flags._f11 != 0
	float m_fWheelRadius;							// радиус переднего колеса? если менять то задные колеса меняли размер 
	float m_fDiscRadius;							// радиус колеса из veh.ide
	float m_fWheelWidth;							// Margin или AABB * 2. Скорее всего AABB ширина. 
	CHandlingData* m_pHandling;						// поинтер?
	float _f18;							// одно и то же для колес из одной стороны(r и r, f и f )
	float _f1c;							// 0.38? posZ?
	float _f20; // rotation, for hydraulic							// текущая высота подвески?
	float m_fTyreStability;			// m_fSusStiffnessCoefficient?				// устойчивость колеса? текущая высота подвески?
	float m_fTyreStabilityPercent;							// 1/_f24 
	float _f2c;							// масштаб по x? 1 в l колесах и -1 в r						ПРОВЕРИТЬ В ПЕРВУЮ ОЧЕРЕДЬ
	rage::Vector3 matrix_right;				// размер по y ? 1. реагирует на удар(на колеса). или елемент матрицы. 1
	rage::Vector3 matrix_at;					// елемент мытрицы

	rage::Vector3 m_vCollisionOffset;
	rage::Vector3 m_vModelOffset;

	//rage::Vector3 _f50;						// копирует позицию?
	//rage::Vector3 m_vPosition;				// 060-070
	float m_fCompression;							// default = -101
	float _f74;							// m_fCompression2? default = -101
	float m_fTyreWearRate;			// m_fSusLength?				
	float m_fRotationX;
	float m_fRpm;						// скорость вращения?
	float m_fRpmOld;							// скорость вращения?
	float m_fRotSlipRatio;							// значения, которое зависит от скорости
	float m_fTyreTemp; // time on fire?
	rage::Vector3 _f90;
	rage::Vector3 _fa0; // pos2?
	rage::Vector3 _fb0;
	rage::Vector3 _fc0;
	void* m_pReference;
	void* _fd4;
	DWORD m_dwHitMaterialId;
	DWORD _fdc;
	float _fe0;	// 1.0 is default
	float _fe4;	// 1.0 is default
	BYTE __e8[0x8];
	float _ff0;						// имеет отношение к подвеске. 0 если колесо в воздухе
	float _ff4;						// имеет отношение к подвеске. 0 если колесо в воздухе
	float _ff8;						// имеет отношение к подвеске. 0 если колесо в воздухе
	BYTE __fc[4];					// 0xcdcdcdcd
	float _f100;
	float _f104;
	float _f108;					// имеет отношение к подвеске. 0 если колесо в воздухе
	float _f10c;					// имеет отношение к подвеске. 0 если колесо в воздухе
	rage::Vector3 m_vGroundVelocity;					// w - always -1?
	rage::Vector3 m_vTyreContactVelocity;
	float m_fSidemarkIntensity;					// интенсивность следов |velocity? if == 0 then no tyre track on floor
	float _f134;					// m_fTyreStabilityCoefficient?
	float m_fPhysicsOffsetLeftRight;					// реагирует на поворот влево lf
	float m_fRotationZ;				// 13C-140 rotation in z-axis.
	float m_fBrakeForce;
	float m_fDriveForce;
	float _f148;
	float m_fRotationZOld;			// дублирует?
	float m_fBrakeForceOld;
	float m_fDriveForceOld;
	float m_fFrictionDamage; // maybe
	float m_fHealth;				// 15C-160
	float m_fTyreHealth;			// 160-164
	union {
		struct {
			DWORD bSpringsAreActive : 1;
			DWORD bTouchesGround : 1;
			DWORD bCurrentIsDrive : 1; // if throttle > 0.f?
			DWORD bWheelIsOnFire : 1;
			DWORD _f4 : 1; // without track?
			DWORD _f5 : 1; // without track?
			DWORD _f6 : 1;
			DWORD _f7 : 1;

			DWORD _f8 : 1;
			DWORD _f9 : 1;
			DWORD _fa : 1; // заниження?
			DWORD _fb : 1; // used with fragInstGta from CVehicle | is left or right wheel? mirror?
			DWORD bIsRearWheel : 1; // якщо поставити 1 то в ньому буде подвійний слід якщо він є на задньому
			DWORD bFrontWheelSteer : 1;
			DWORD bRearWheelSteer : 1;
			DWORD bDriveWheel : 1;

			DWORD bSuspensionMovementOfTheRightWheel : 1;
			DWORD bSuspensionMovementOfTheLeftWheel : 1;
			DWORD _f12 : 1; // low gravity? 1 if r have separate model
			DWORD _f13 : 1; // 1 if r have separate model
			DWORD _f14 : 1;
			DWORD _f15 : 1;
			DWORD bMirrorWheel : 1; // mirror wheel model
			DWORD _f17 : 1;

			DWORD bScaleWheelByCollision : 1;	// for example, the rear wheel airtug is larger than the front
			DWORD _f19 : 1; // stranniy shum pri ezde. Voda?
			DWORD bRaiseSuspension1 : 1;
			DWORD bRaiseSuspension2 : 1;
			DWORD bBurnout : 1; // on idle
			DWORD _f1d : 1;
			DWORD _f1e : 1;
			DWORD _f1f : 1;
		};
		DWORD value;
	} m_dwFlags;
	float _f168; // if _f168 < 0 then _f168 = 
	float _f16c; // if m_flags.bWheelIsOnFire then _f16c = 0

};

static_assert(sizeof(CWheel) == 0x170, "Invalid CWheel structure size");

namespace rhine_track {


	void processSpeed(CExtVeh* veh) {
		CWheel* pWheels = *(CWheel**)((BYTE*)veh + 0xfd0);
		DWORD* pWheelCount = (DWORD*)((BYTE*)veh + 0xfd4);

		float lwheel[5];
		BYTE lcount = 0;
		float rwheel[5];
		BYTE rcount = 0;

		for (size_t i = 0; i < *pWheelCount; i++) {

			if (pWheels[i].m_nBoneId == 10 ||pWheels[i].m_nBoneId == 12 ||pWheels[i].m_nBoneId == 14) { // R
				rwheel[rcount] = pWheels[i].m_fRpm;
				rcount++;
			}
			else {
				lwheel[lcount] = pWheels[i].m_fRpm;
				lcount++;
			}

		}
		veh->m_aTrackSpeed[1] = veh->m_aTrackSpeed[0] = 0.f;
		for (BYTE i = 0; i < rcount; i++) veh->m_aTrackSpeed[1] += rwheel[i];
		veh->m_aTrackSpeed[1] /= rcount;
		for (BYTE i = 0; i < lcount; i++) veh->m_aTrackSpeed[0] += lwheel[i];
		veh->m_aTrackSpeed[0] /= lcount;

		//veh->m_aTrackSpeed[0] = 10.f;
	//	veh->m_aTrackSpeed[1] = -10.f;

		for (size_t i = 0; i < 2; i++) {
			veh->m_aFakeTrackWheelsRot[i] += veh->m_aTrackSpeed[i] * *timer::g_pTimestep;

			while (veh->m_aFakeTrackWheelsRot[i] > 3.14159f) veh->m_aFakeTrackWheelsRot[i] -= 6.28319f; // 6.28319;
			while (veh->m_aFakeTrackWheelsRot[i] < -3.14159f) veh->m_aFakeTrackWheelsRot[i] += 6.28319f;
		}
	}

	void _new(CExtVeh* veh) {
		veh->m_aFakeTrackWheelsRot[0] = 0.f;
		veh->m_aFakeTrackWheelsRot[1] = 0.f;
	}

	void init() {
		hooks::CAutomobile__processControl::add((void(*)(CVehicle*))processSpeed);
		hooks::CVehicle__CVehicle::add((void(*)(CVehicle*))_new);

	}
}