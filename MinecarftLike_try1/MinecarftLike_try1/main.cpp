#include"Minecarft.h"

float angle = 0;

COMMANDLINE Commandline;
DEBUGER Debuger;
DEVICE Device;
RENDER Render;
bool gameoverflag;
Object* cube;


int main()
{
	Device.init();
	gameoverflag = false;

	Object* grass = Device.LoadObject("grass.txt");
	if (!grass)
		return -1;

	cube= Device.LoadObject("cube.txt");
	if (!cube)
		return -1;

	Render.AddDrawObject(*grass, 0, 0, 0, 2, angle);
	Render.AddDrawObject(*grass, 2, 2, 0, 2, angle);

	for (int z = -10; z < 0; z += 2)
	{
		for (int x = -10; x < 0; x += 2)
		{
			Render.AddDrawObject(*grass, x, 0, z, 0, angle);
		}
	}

	time_t start, stop;
	start = time(NULL);
	int frame = 0;
	while (!gameoverflag)
	{
		Device.clean();



		Render.Draw();
		
		Device.update();
		stop = time(NULL);
		frame++;
		if (stop - start >= 1)
		{
			start = stop;
			char title[200];
			sprintf(title, "FPS %d POS (%.1f %.1f %.1f) ", frame,Render.CamerPosX,Render.CamerPosY,Render.CamerPosZ);
			Device.SetTitle(title);
			frame = 0;
		}
	}
	Debuger.print("Exit Normal", mGREEN);
	return 0;
}