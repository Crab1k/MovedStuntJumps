#pragma once


#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

// Script variables are 8 bytes on PC, with 4 bytes of padding at the end
struct PaddedInt {
	int val = 0;
	DWORD _padding;

	operator int() const { return val; }
	void operator=(int value) { val = value; }
};

// Script variables are 8 bytes on PC, with 4 bytes of padding at the end
struct PaddedFloat {
	float val = 0;
	DWORD _padding;

	operator float() const { return val; }
	void operator=(float value) { val = value; }
};

// Names in STORED_CAR_DATA savefile struct in standard_global_reg.sc
struct __declspec(align(16)) STORED_CAR_DATA
{
	PaddedInt model;
	PaddedInt modelTrailer;
	PaddedFloat fDirtLevel;
	PaddedInt fHealth;
	PaddedInt iColourCombo;
	PaddedInt iColour1;
	PaddedInt iColour2;
	PaddedInt iColourExtra1;
	PaddedInt iColourExtra2;
	PaddedInt bColourCombo;
	PaddedInt bColourExtra;
	PaddedInt bExtraOn[13];
	PaddedInt bConvertible;
	PaddedInt iRadioIndex;
	PaddedInt iPlateBack;
	char tlNumberPlate[16];
	char _padding0[16];
	PaddedInt vehModIdsCnt; // Every script array has its size at the start
	PaddedInt vehModIds[25]; // 49 on latest patch
	PaddedInt vehModVariationsCnt; // Every script array has its size at the start
	PaddedInt vehModVariations[2];
	PaddedInt iTyreR, iTyreG, iTyreB;
	PaddedInt iWindowTintColour;
	PaddedInt bTyresCanBurst;
	PaddedInt iLivery;
	PaddedInt iWheelType;
	PaddedInt eType;

	PaddedInt iFlags;
	PaddedInt iNeonR, iNeonG, iNeonB;

	PaddedFloat fEnvEff;

	PaddedInt bIsPlayerVehicle;
};

struct HospitalRespawnLocations {
	Vector3 RespawnCoords;
	Vector3 VehicleRespawnCoords;
	float VehicleRespawnHeading;
};

#define M_PI 3.14159265358979323846  /* pi */
#define RAD_TO_DEG(x) (x * 180.0f / M_PI)

const Vector3 MakeVector3(float x, float y, float z) {
	Vector3 result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}

HospitalRespawnLocations hospitals[5] = {
	/*{Hospital Respawn Point}                              , {Vehicle Respawn Point}                                  , {Vehicle Respawn Heading}*/
	 {MakeVector3(-449.7836,-341.3995,33.5024), MakeVector3(-457.7702, -330.089142, 33.3686371), RAD_TO_DEG(1.3962f) }, /* Mount Zonah Medical Center, Rockford Hills */
	 {MakeVector3(341.4144, -1396.2910, 31.5098), MakeVector3(312.535339, -1374.96033, 30.78285), RAD_TO_DEG(-0.6981f)}, /* Pillbox Hill Medical Center	*/
	 {MakeVector3(360.7675,  -583.4315, 27.8269), MakeVector3(358.012756, -598.343, 27.6180439), RAD_TO_DEG(2.8797f) }, /* Central Los Santos Medical Center */
	 {MakeVector3(1838.4948, 3672.2222, 33.2783), MakeVector3(1835.35974, 3665.93848, 32.74462), RAD_TO_DEG(-2.6179f) }, /* Sandy Shores */
	 {MakeVector3(-242.2981, 6325.2334, 31.4271), MakeVector3(-228.2992, 6314.09131, 30.3608074), RAD_TO_DEG(2.3561) }, /* Paleto Bay */
};

int HasRollCage(int iParam0) /* From agency_heist1.c::func_415  +95 others */
{
	int iVar0;
	int iVar1;
	int iVar2;

	if ((ENTITY::DOES_ENTITY_EXIST(iParam0) && VEHICLE::IS_VEHICLE_DRIVEABLE(iParam0, 0)) && VEHICLE::GET_NUM_MOD_KITS(iParam0) > 0)
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(iParam0, 0);
		iVar0 = 0;
		while (iVar0 < 25)
		{
			iVar1 = iVar0;
			if (((((iVar1 == 17 || iVar1 == 18) || iVar1 == 19) || iVar1 == 20) || iVar1 == 21) || iVar1 == 22)
			{
			}
			else if (VEHICLE::GET_VEHICLE_MOD(iParam0, iVar1) != -1)
			{
				iVar2 = GAMEPLAY::GET_HASH_KEY(VEHICLE::GET_MOD_TEXT_LABEL(iParam0, iVar1, VEHICLE::GET_VEHICLE_MOD(iParam0, iVar1)));
				if (iVar2 != 0)
				{
					if (iVar2 == GAMEPLAY::GET_HASH_KEY("MNU_CAGE") || iVar2 == GAMEPLAY::GET_HASH_KEY("SABRE_CAG"))
					{
						return 1;
					}
				}
			}
			iVar0++;
		}
	}
	return 0;
}

int SetVehicleMods(int vehicle, PaddedInt modCnt, PaddedInt uParam1[25], PaddedInt uParam2[2]) /* From agency_heist1.c::func_414  +62 others */
{
	int iVar0;
	int iVar1;

	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle, 0))
	{
		return 0;
	}
	if (VEHICLE::GET_NUM_MOD_KITS(vehicle) == 0)
	{
		return 0;
	}
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	iVar0 = 0;
	while (iVar0 < modCnt)
	{
		iVar1 = iVar0;

		int modIndex = uParam1[iVar0];

		if (((((iVar1 == 17 || iVar1 == 18) || iVar1 == 19) || iVar1 == 20) || iVar1 == 21) || iVar1 == 22)
		{
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, iVar1, modIndex > 0);
		}
		else if (VEHICLE::GET_VEHICLE_MOD(vehicle, iVar1) != (modIndex - 1))
		{
			VEHICLE::REMOVE_VEHICLE_MOD(vehicle, iVar1);
			if (modIndex > 0)
			{
				if (iVar0 == 23)
				{
					VEHICLE::SET_VEHICLE_MOD(vehicle, iVar1, (modIndex - 1), uParam2[0] > 0);
				}
				else if (iVar0 == 24)
				{
					VEHICLE::SET_VEHICLE_MOD(vehicle, iVar1, (modIndex - 1), uParam2[1] > 0);
				}
				else
				{
					VEHICLE::SET_VEHICLE_MOD(vehicle, iVar1, (modIndex - 1), false);
				}
			}
		}
		iVar0++;
	}
	if (HasRollCage(vehicle))
	{
		VEHICLE::SET_VEHICLE_STRONG(vehicle, 1);
		VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(vehicle, 1);
	}
	return 1;
}

// If the player has not customized a vehicle, this will return the default one.
// character: 0 = Michael, 1 = Franklin, 2 = Trevor
// vehicleType: 1 = car, 2 = bike
STORED_CAR_DATA GetDefaultCharacterVehicle(int character, int vehicleType) {
	STORED_CAR_DATA result;
	result.bTyresCanBurst = TRUE;
	result.iTyreR = 255;
	result.iTyreG = 255;
	result.iTyreB = 255;

	result.bIsPlayerVehicle = TRUE;

	result.fHealth = 1000;

	result.modelTrailer = 0;

	Hash iVar0;
	switch (character)
	{
	case 0:
		iVar0 = GAMEPLAY::GET_HASH_KEY("tailgater");
		/*if (Global_SAVE_DATA.FLOW_STRUCT.MF_CONTROLS_STRUCT.f_58[128] && !Global_SAVE_DATA.FLOW_STRUCT.MF_CONTROLS_STRUCT.f_58[131])
		{
			iVar0 = GAMEPLAY::GET_HASH_KEY("premier");
		}*/
		if (iVar0 == GAMEPLAY::GET_HASH_KEY("tailgater")) {
			result.model = iVar0;
			result.fDirtLevel = 3.0f;
			result.iColourCombo = 0;
			result.bColourCombo = 1;
			result.bExtraOn[0] = 1;
			strncpy(result.tlNumberPlate, "5MDS003", 8);
		}
		else if (iVar0 == GAMEPLAY::GET_HASH_KEY("premier"))
		{
			result.model = iVar0;
			result.fDirtLevel = 14.9f;
			result.iColour1 = 43;
			result.iColour2 = 43;
			result.iColourExtra1 = 0;
			result.iColourExtra2 = 156;
			result.bColourCombo = 0;
			strncpy(result.tlNumberPlate, "880HS955", 9);
		}
		break;

	case 2:
		result.model = GAMEPLAY::GET_HASH_KEY("bodhi2");
		result.fDirtLevel = 14.0f;
		result.iColour1 = 32;
		result.iColour2 = 0;
		result.iColourExtra1 = 0;
		result.iColourExtra2 = 156;
		strncpy(result.tlNumberPlate, "BETTY 32", 9);
		/*if (Global_SAVE_DATA.FLOW_STRUCT.MF_CONTROLS_STRUCT.f_58[119])
		{
			result.f_11[1] = 1;
		}*/

		break;

	case 1:
		if (vehicleType == 1)
		{
			iVar0 = GAMEPLAY::GET_HASH_KEY("buffalo2");
		}
		else if (vehicleType == 2)
		{
			iVar0 = GAMEPLAY::GET_HASH_KEY("bagger");
		}
		else
		{
			iVar0 = GAMEPLAY::GET_HASH_KEY("buffalo2");
		}

		if (iVar0 == GAMEPLAY::GET_HASH_KEY("bagger")) {
			result.model = iVar0;
			result.fDirtLevel = 6.0f;
			result.iColour1 = 53;
			result.iColour2 = 0;
			result.iColourExtra1 = 59;
			result.iColourExtra2 = 156;
			strncpy(result.tlNumberPlate, "FC88", 5);

		}
		else if (iVar0 == GAMEPLAY::GET_HASH_KEY("buffalo2")) {
			result.model = iVar0;
			result.fDirtLevel = 0.0f;
			result.iColour1 = 111;
			result.iColour2 = 111;
			result.iColourExtra1 = 0;
			result.iColourExtra2 = 156;
			result.bColourExtra = 1;
			strncpy(result.tlNumberPlate, "FC1988", 7);
			result.bExtraOn[0] = 1;
			result.bExtraOn[1] = 1;
			result.bExtraOn[2] = 1;
			result.bExtraOn[3] = 1;
			result.bExtraOn[4] = 1;
			result.bExtraOn[5] = 1;
			result.bExtraOn[6] = 1;
			result.bExtraOn[7] = 1;
			result.bExtraOn[8] = 1;
		}
		break;
	default:
		break;
	}

	return result;
}

void SetCarAsPersonal(Vehicle vehicleId, int character) {
	int iVar0;

	iVar0 = 0;
	while (iVar0 < 9)
	{
		PaddedInt* playerVehs1 = (PaddedInt*)getGlobalPtr(87698);
		PaddedInt* playerVehs2 = (PaddedInt*)getGlobalPtr(87718);
		PaddedInt* vehiclePlayer = (PaddedInt*)getGlobalPtr(87708);
		PaddedInt* playerUnkown = (PaddedInt*)getGlobalPtr(87746);
		if (!ENTITY::DOES_ENTITY_EXIST(playerVehs1[iVar0]))
		{
			playerVehs1[iVar0] = vehicleId;
			vehiclePlayer[iVar0] = character;
			playerVehs2[iVar0] = ENTITY::GET_ENTITY_MODEL(vehicleId);
			//if (VEHICLE::IS_THIS_MODEL_A_CAR(playerVehs2[iVar0]))
			//{
			//	playerUnkown[character /*3*/][0] = -1;
			//}
			//else
			//{
			//	playerUnkown[character /*3*/][1] = -1;
			//}
			iVar0 = 9;
		}
		if (iVar0 == 8)
		{
		}
		iVar0++;
	}
}

// Read the global struct for any character's personal vehicle and creates an instance of it.
// This will only work on 1.27. 
// character: 0 = Michael, 1 = Franklin, 2 = Trevor
// vehicleType: 1 = car, 2 = bike
void CreatePersonalVehicleOnCoords(int character, int vehicleType, Vector3 coords, float heading) {

	// Global_SaveData.COMPONENTS_ARRAY.PP_INFO_STRUCT.STORED_CAR_DATA[vehicleType /*223*/][character /*74*/]
	// vehicleType here is 0 for car, 1 for bike, unlike in the default vehicle function, so we subtract one
	// 1.27: 97353+1729+539+1635 + (1 + vehicleType*223) + (1 + character*74) = 101258 + (vehicleType*223)+(character*74). 
	// Every script array has its size at the start so add 1 to access the first element.
	STORED_CAR_DATA* g_SavedVehicle = (STORED_CAR_DATA*)getGlobalPtr(101256 + (1 + (vehicleType-1) * 223) + (1 + character * 74));

	STORED_CAR_DATA defaultVehicle = GetDefaultCharacterVehicle(character, vehicleType);
	if (defaultVehicle.model != g_SavedVehicle->model) { // No saved car
		g_SavedVehicle = &defaultVehicle;
	}

	STREAMING::REQUEST_MODEL(g_SavedVehicle->model);
	if (STREAMING::HAS_MODEL_LOADED(g_SavedVehicle->model))
	{

		int ReturnVeh = VEHICLE::CREATE_VEHICLE(g_SavedVehicle->model, coords.x, coords.y, coords.z, heading, false, false);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(ReturnVeh);
		VEHICLE::_0xAB04325045427AAE(ReturnVeh, false); // SET_VEHICLE_NOT_STEALABLE_AMBIENTLY
		VEHICLE::_0x428BACCDF5E26EAD(ReturnVeh, false); // SET_VEHICLE_CAN_SAVE_IN_GARAGE
		VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(ReturnVeh, true);

		ENTITY::SET_ENTITY_HEALTH(ReturnVeh, 1250);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(ReturnVeh, 1250);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(ReturnVeh, 1250);
		VEHICLE::SET_VEHICLE_COLOURS(ReturnVeh, g_SavedVehicle->iColour1, g_SavedVehicle->iColour2);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(ReturnVeh, g_SavedVehicle->iColourExtra1, g_SavedVehicle->iColourExtra2);

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(ReturnVeh, g_SavedVehicle->fDirtLevel);

		for (int iExtra = 0; iExtra < 12; iExtra++) {
			VEHICLE::SET_VEHICLE_EXTRA(ReturnVeh, iExtra + 1, (!g_SavedVehicle->bExtraOn[iExtra]));
		}
		if (g_SavedVehicle->bConvertible) {
			VEHICLE::SET_CONVERTIBLE_ROOF(ReturnVeh, g_SavedVehicle->bConvertible);
		}

		// CBA to do the iFruit license plates
		//if (Get iFruit plate)
		//{
		//	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(ReturnVeh, tlPlateText);
		//	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(ReturnVeh, iPlateBack);
		//}
		//else
		{
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(ReturnVeh, g_SavedVehicle->tlNumberPlate);
			if (g_SavedVehicle->iPlateBack >= 0
				&& g_SavedVehicle->iPlateBack < VEHICLE::GET_NUMBER_OF_VEHICLE_NUMBER_PLATES())
			{
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(ReturnVeh, g_SavedVehicle->iPlateBack);
			}
		}

		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(ReturnVeh, g_SavedVehicle->iTyreR, g_SavedVehicle->iTyreG, g_SavedVehicle->iTyreB);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(ReturnVeh, g_SavedVehicle->bTyresCanBurst);

		VEHICLE::SET_VEHICLE_WINDOW_TINT(ReturnVeh, g_SavedVehicle->iWindowTintColour);

		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(ReturnVeh, g_SavedVehicle->iNeonR, g_SavedVehicle->iNeonG, g_SavedVehicle->iNeonB);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(ReturnVeh, 2, g_SavedVehicle->iFlags & (1 << 28));
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(ReturnVeh, 3, g_SavedVehicle->iFlags & (1 << 29));
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(ReturnVeh, 0, g_SavedVehicle->iFlags & (1 << 30));
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(ReturnVeh, 1, g_SavedVehicle->iFlags & (1 << 31));

		if (VEHICLE::GET_VEHICLE_LIVERY_COUNT(ReturnVeh) > 1
			&& g_SavedVehicle->iLivery >= 0)
		{
			VEHICLE::SET_VEHICLE_LIVERY(ReturnVeh, g_SavedVehicle->iLivery);
		}

		if (g_SavedVehicle->iWheelType > -1) {
			if (!VEHICLE::IS_THIS_MODEL_A_BICYCLE(ENTITY::GET_ENTITY_MODEL(ReturnVeh))) {
				if (VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(ReturnVeh))) {
					if (g_SavedVehicle->iWheelType == 6) {
						VEHICLE::SET_VEHICLE_WHEEL_TYPE(ReturnVeh, g_SavedVehicle->iWheelType);
					}
				}
				else {
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(ReturnVeh, g_SavedVehicle->iWheelType);
				}
			}
		}

		SetVehicleMods(ReturnVeh, g_SavedVehicle->vehModIdsCnt, g_SavedVehicle->vehModIds, g_SavedVehicle->vehModVariations);
		VEHICLE::_SET_VEHICLE_PAINT_FADE(ReturnVeh, g_SavedVehicle->fEnvEff);

		// CBA doing Mr Raspberry Jam
		//if (character == 2) {
		//	if (ENTITY::GET_ENTITY_MODEL(ReturnVeh) == GAMEPLAY::GET_HASH_KEY("bohdi2") {
		//		Add Mr Raspberry Jam
		//	}
		//}
		SetCarAsPersonal(ReturnVeh, character);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(g_SavedVehicle->model);
	}
}

bool isPlayerAlive = true;
int lastPlayerRespawnTime = 0;

void handleCharacterRespawn() {
	Ped playerPed = PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID());
	if (!isPlayerAlive && !PED::IS_PED_DEAD_OR_DYING(playerPed, 1)) { // player was dead last frame but respawned
		if (GAMEPLAY::GET_GAME_TIMER() - lastPlayerRespawnTime > 2000) { // last respawn was more than 2 seconds ago (the above if triggers multiple times per respawn).
			lastPlayerRespawnTime = GAMEPLAY::GET_GAME_TIMER();
			for (const HospitalRespawnLocations& hospital : hospitals) {
				if (ENTITY::IS_ENTITY_AT_COORD(playerPed, hospital.RespawnCoords.x, hospital.RespawnCoords.y, hospital.RespawnCoords.z, 20.0f, 20.0f, 20.0f, 0, 0, 0)) {
					// Found the hospital the player respawned at, spawn their personal vehicle
					CreatePersonalVehicleOnCoords(1, 1, hospital.VehicleRespawnCoords, hospital.VehicleRespawnHeading);
				}
			}
		}
	}

	isPlayerAlive = !PED::IS_PED_DEAD_OR_DYING(playerPed, 1);
	WAIT(0);
}