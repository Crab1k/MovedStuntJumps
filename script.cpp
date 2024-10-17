/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "script.h"
#include "utils.h"
#include "keyboard.h"
#include "jumps.h"
#include "graphics.h"
#include <vector>

void disableOriginalStunts()
{
	// disable set to avoid counter issues
	GAMEPLAY::DISABLE_STUNT_JUMP_SET(0);

	// clear every possible stunt jump ID
	for (int i = 0; i < 16384; i++) {
		GAMEPLAY::DELETE_STUNT_JUMP(i);
	}

}

void addMovedStunts()
{
	// 10
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(153.6544, 1661.692, 227.3657, 165.3773, 1674.75, 235.2305, 6.f, 205.5174, 2114.674, 54, 383.3433, 2330.24, 123, 100.f, 247.59, 1919.89, 254.25, 150, 0); // lamar down
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-764.2935, 945.8024, 228.5571, -723.0186, 930.0048, 240, 6.f, -857.0611, 441.225, 82, -852.3926, 315.6501, 90, 26.f, -815.92, 860.92, 268.11, 150, 0); // big score
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-955.576, -181.6195, 38.56944, -962.9437, -180.3027, 48.76974, 6, -1052.138, 10.01165, 50, -991.4943, -123.7049, 35.35284, 50, -976.843, -178.2526, 48.65004, 150, 0); // golf
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-236.6489, 336.4734, 106.5948, -217.772, 341.277, 111.2053, 6.f, -202.9962, 530.1286, 162.5095, -204.7086, 562.7252, 188.1333, 70.f, -164.0033, 468.7388, 161.6555, 150, 0); // vinewood bounce *
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(123.1099, 602.2518, 206.8295, 129.4429, 604.0329, 203.0355, 6.f, 130.4396, 246.0203, 118.1499, 153.364, 306.8955, 130, 80.f, 178.3878, 265.1066, 143.2553, 150, 0); // vinewood ramp *
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(2001.78, 3895.777, 33.85062, 1997.11, 3906.627, 40.49184, 6.f, 2341.978, 4102.864, 26.0314, 2462.083, 4171.103, 40.5618, 70.f, 2115.721, 4150.492, 70.10548, 150, 0); // sandy shores lake
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-1265.194, -380.3223, 58.71485, -1263.002, -373.3664, 65.81985, 2.5, -1451.292, -381.7545, 48.42518, -1402.532, -342.3726, 33.79811, 50.f, -1418.419, -387.0577, 50.1657, 150, 0); // parking lot to road
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-1306.279, -188.8446, 60.90955, -1300.866, -185.6255, 65.49306, 2.5, -1310.206, -82.39357, 54.39466, -1308.385, -115.6674, 46.46138, 20.f, -1315.043, -162.7263, 68.19829, 150, 0); // parking lot to tennis
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-1813.373, -493.9336, 37.97896, -1802.622, -504.5941, 52.55691, 8.f, -2010.466, -727.9706, -2.617887, -1903.905, -615.4824, 16.9653, 100.f, -1906.799, -535.1393, 55.43876, 150, 0); // reuniting to beach
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-1173.753, -704.4168, 39.93867, -1178.371, -708.7709, 45.88336, 2.5, -1097.246, -813.9249, 28.95643, -1077.078, -839.4893, 33.1883, 12.f, -1103.343, -772.6826, 45.22516, 150, 0); // parking lot to roof

	// 20
	GAMEPLAY::ADD_STUNT_JUMP_ANGLED(-470.9938, -792.7442, 44.18004, -477.9633, -792.445, 50.22325, 2.5, -456.3522, -876.0785, 27.28179, -456.1161, -906.9981, 33.19931, 32.f, -438.9023, -831.4876, 60.37182, 150, 0); // parking lot to construction


	GAMEPLAY::ENABLE_STUNT_JUMP_SET(0);
}

bool isInRotatableCuboid(Vector3 p, RotatableCuboid region)
{
	// We can check the Z axis trivially.
	if (p.z < min(region.start.z, region.end.z) || p.z > max(region.start.z, region.end.z))
		return false;

	// https://math.stackexchange.com/a/2193733

	// line = A + t * B with A = region.start and B = delta
	// f(t) = (1 - t)A + tB - P

	auto vX = region.end.x - region.start.x;
	auto vY = region.end.y - region.start.y;

	auto uX = region.start.x - p.x;
	auto uY = region.start.y - p.y;

	auto vdotu = vX * uX + vY * uY;
	auto vdotv = vX * vX + vY * vY;

	auto t = -vdotu / vdotv;
	if (t < 0 || t > 1)
		return false;

	auto interpX = region.start.x + vX * t;
	auto interpY = region.start.y + vY * t;

	return (interpX - p.x) * (interpX - p.x) + (interpY - p.y) * (interpY - p.y) < (region.width * region.width) / 4.f;
}

void drawJumps()
{
	// For some reason on patch v1.27, drawing a regular box messes up all line drawing, so we draw all the wireframe boxes first, then the solid ones.

	GRAPHICS::CLEAR_DRAW_ORIGIN();

	auto playerPed = PLAYER::PLAYER_PED_ID();
	auto vehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	auto vehicleCoords = vehicle ? ENTITY::GET_ENTITY_COORDS(vehicle, true) : v3(0, 0, 0);

	for (StuntJump stunt : stunts)
	{
		drawBoxWireframe(stunt.jump, 48, 127, 127, 255);

		if (vehicle && isInRotatableCuboid(vehicleCoords, stunt.landing))
		{
			drawBoxWireframe(stunt.landing, 96, 255, 96, 255);
		}
		else
		{
			drawBoxWireframe(stunt.landing, 255, 96, 255, 255);
		}
	}

	static int opacity = 48;
	if (IsKeyJustUp(VK_F3))
		opacity -= 8 * (IsKeyDown(VK_SHIFT) ? -1 : 1);
	opacity = min(max(opacity, 0), 255);

	for (StuntJump stunt : stunts)
	{
		drawBox(stunt.landing, 255, 255, 255, opacity);
	}
}

void main()
{
	disableOriginalStunts();
	addMovedStunts();
	initializeJumps();

	while (true)
	{
		static bool enabled = true;
		if (IsKeyJustUp(VK_F4))
			enabled = !enabled;

		if (enabled)
			drawJumps();

		WAIT(0);
	}
}

void ScriptMain()
{
	main();
}

