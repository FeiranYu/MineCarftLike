#include"Minecarft.h"

float angle = 0;



DEBUGER Debuger;
DEVICE Device;
RENDER Render;


int main()
{
	Device.init();
	char filename[] = "1.bmp";
	if (!Device.LoadBmp(filename))
		return -1;
	MeshAndIndex* cube = Device.LoadMeshAndIndexFromFile("grass.txt");
	if (!cube)
		return -1;
	
	time_t start, stop;
	start = time(NULL);
	int frame = 0;
	while (1)
	{
		Device.clean();

		Render.SetWorldMatTranslate(0, 1, 0);
		Render.SetWorldMatRotation(2, 90);
		Render.DrawMesh(cube);

		Device.update();
		//char strBuffer[20];
		//sprintf(strBuffer, "Hello World");
		//TextOut(Device.screen_hdc, 0, 0, strBuffer, 6);
		stop = time(NULL);
		frame++;
		if (stop - start >= 1)
		{
			start = stop;
			char title[20];
			sprintf(title, "FPS %d POS (%.1f %.1f %.1f) ", frame,Render.CamerPosX,Render.CamerPosY,Render.CamerPosZ);
			Device.SetTitle(title);
			frame = 0;
		}

	}
	return 0;
}