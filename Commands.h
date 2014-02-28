/*

"wait" - does nothing that frame
"fire1" - 5 params : x, y, speed, angle, "name of file to clone"
"fire2" - 8 params : x, y, speed, angle, accel, accelAngle, speedEnd, "name of file to clone"
*/

#include <string>

struct variant
{
	variant()
	{
		for(int i = 0; i < 8; i++)
		{
			vString[i] = "";
			vFloat[i] = -1;
		}
	}

	std::string vString[8];
	float vFloat[8];
};