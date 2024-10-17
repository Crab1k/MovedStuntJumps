#include "../../inc/types.h"
#include "vectors.h"

#include <list>

typedef struct
{
	Vector3 start;
	Vector3 end;
	float width;
} RotatableCuboid;

struct StuntJump {
	RotatableCuboid jump;
	RotatableCuboid landing;
};

std::list<StuntJump> stunts = {};

// Shims so that we can just paste the (processed) decompiled game script into this and extract the jump data

RotatableCuboid processAxisAlignedBB(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float centreX = (x1 + x2) / 2.f;
	float width = abs(x1 - x2);

	return {
		v3(centreX, min(y1, y2), min(z1, z2)),
		v3(centreX, max(y1, y2), max(z1, z2)),
		width
	};
}

namespace JUMPS_MISC { // So we don't conflict with the ScriptHook natives, but it's easy to find and replace 'MISC::' with 'JUMPS_MISC::'
	void ADD_STUNT_JUMP(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float camX, float camY, float camZ, int unk1, int unk2 = 0, int unk3 = 0)
	{
		RotatableCuboid jump = processAxisAlignedBB(x1, y1, z1, x2, y2, z2);
		RotatableCuboid landing = processAxisAlignedBB(x3, y3, z3, x4, y4, z4);

		stunts.push_back({
			jump,
			landing
			});
	}

	void ADD_STUNT_JUMP_ANGLED(float x1, float y1, float z1, float x2, float y2, float z2, float radius1, float x3, float y3, float z3, float x4, float y4, float z4, float radius2, float camX, float camY, float camZ, int unk1, int unk2, int unk3 = 0)
	{
		RotatableCuboid jump = {
			v3(x1, y1, z1),
			v3(x2, y2, z2),
			radius1
		};

		RotatableCuboid landing = {
			v3(x3, y3, z3),
			v3(x4, y4, z4),
			radius2
		};

		stunts.push_back({
			jump,
			landing
			});
	}
}

void initializeJumps() {
	stunts.clear();

	// moved jumps
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(153.6544, 1661.692, 227.3657, 165.3773, 1674.75, 235.2305, 6.f, 205.5174, 2114.674, 54, 383.3433, 2330.24, 123, 100.f, 247.59, 1919.89, 254.25, 150, 0); // lamar down
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-764.2935, 945.8024, 228.5571, -723.0186, 930.0048, 240, 6.f, -857.0611, 441.225, 82, -852.3926, 315.6501, 90, 26.f, -815.92, 860.92, 268.11, 150, 0); // big score
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-955.576, -181.6195, 38.56944, -962.9437, -180.3027, 48.76974, 6.f, -1052.138, 10.01165, 52, -991.4943, -123.7049, 35.35284, 65.f, -976.843, -178.2526, 48.65004, 150, 0); // golf
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-236.6489, 336.4734, 106.5948, -217.772, 341.277, 111.2053, 6.f, -202.9962, 530.1286, 162.5095, -204.7086, 562.7252, 188.1333, 70.f, -164.0033, 468.7388, 161.6555, 150, 0); // vinewood bounce *
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(123.1099, 602.2518, 206.8295, 129.4429, 604.0329, 203.0355, 6.f, 130.4396, 246.0203, 118.1499, 153.364, 306.8955, 130, 80.f, 178.3878, 265.1066, 143.2553, 150, 0); // vinewood ramp
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(2001.78, 3895.777, 33.85062, 1997.11, 3906.627, 40.49184, 6.f, 2341.978, 4102.864, 26.0314, 2462.083, 4171.103, 40.5618, 70.f, 2115.721, 4150.492, 70.10548, 150, 0); // sandy shores lake
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-1265.194, -380.3223, 58.71485, -1263.002, -373.3664, 65.81985, 2.5, -1451.292, -381.7545, 48.42518, -1402.532, -342.3726, 33.79811, 50.f, -1418.419, -387.0577, 50.1657, 150, 0); // parking lot to road
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-1306.279, -188.8446, 60.90955, -1300.866, -185.6255, 65.49306, 2.5, -1310.206, -82.39357, 54.39466, -1308.385, -115.6674, 46.46138, 20.f, -1315.043, -162.7263, 68.19829, 150, 0); // parking lot to tennis
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-1813.373, -493.9336, 37.97896, -1802.622, -504.5941, 52.55691, 8.f, -2010.466, -727.9706, -2.617887, -1903.905, -615.4824, 16.9653, 100.f, -1906.799, -535.1393, 55.43876, 150, 0); // reuniting to beach
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-1173.753, -704.4168, 39.93867, -1178.371, -708.7709, 45.88336, 2.5, -1097.246, -813.9249, 28.95643, -1077.078, -839.4893, 33.1883, 12.f, -1103.343, -772.6826, 45.22516, 150, 0); // parking lot to roof
	JUMPS_MISC::ADD_STUNT_JUMP_ANGLED(-470.9938, -792.7442, 44.18004, -477.9633, -792.445, 50.22325, 2.5, -456.3522, -876.0785, 27.28179, -456.1161, -906.9981, 33.19931, 32.f, -438.9023, -831.4876, 60.37182, 150, 0); // parking lot to construction

}