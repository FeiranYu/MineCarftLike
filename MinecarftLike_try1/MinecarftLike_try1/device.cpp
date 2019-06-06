#include"Minecarft.h"


BYTE buffer[SCREEN_WIDTH * SCREEN_HEIGHT * bits / 8];
float depth[SCREEN_WIDTH * SCREEN_HEIGHT];

MeshAndIndex* DEVICE::LoadMeshAndIndexFromFile(const char* filepath)
{
	MeshAndIndex* NewMeshAndIndex = new MeshAndIndex;
	ifstream infile;
	infile.open(filepath, ios::in);
	if (!infile.is_open())
	{
		char msg[100];
		sprintf(msg,"LoadMeshAndIndexFromFile %s Fail!", filepath);
		Debuger.print(msg, mRED);
		return 0;
	}
	
	infile >> NewMeshAndIndex->pointSum;
	NewMeshAndIndex->Point = new vector [NewMeshAndIndex->pointSum];
	for (int i = 0; i < NewMeshAndIndex->pointSum; i++)
	{
		infile >> NewMeshAndIndex->Point[i].x >> NewMeshAndIndex->Point[i].y >> NewMeshAndIndex->Point[i].z;
	}
	infile >> NewMeshAndIndex->indexSum;
	NewMeshAndIndex->Index = new TriangleIndex[NewMeshAndIndex->indexSum];
	for (int i = 0; i < NewMeshAndIndex->indexSum; i++)
	{
		infile >> NewMeshAndIndex->Index[i].a >> NewMeshAndIndex->Index[i].b >> NewMeshAndIndex->Index[i].c >> NewMeshAndIndex->Index[i].ua >> NewMeshAndIndex->Index[i].va >> NewMeshAndIndex->Index[i].ub >> NewMeshAndIndex->Index[i].vb >> NewMeshAndIndex->Index[i].uc >> NewMeshAndIndex->Index[i].vc;
	}

	infile.close();
	char msg[100];
	sprintf(msg, "LoadMeshAndIndexFromFile %s Success", filepath);
	Debuger.print(msg, mGREEN);
	return NewMeshAndIndex;
}




void KeyControl(WPARAM wParam)
{
	char str[30];
	sprintf(str,"KeyControl: press %c[%d] ", wParam,wParam);
	Debuger.print(str,mBLUE);

	if (wParam == VK_ESCAPE)
	{
		exit(0);
	}
	if (wParam == VK_LEFT)
		angle += 0.05;
	if (wParam == VK_RIGHT)
		angle -= 0.05;
	if (wParam == VK_UP);

	if (wParam == VK_DOWN);

	if (wParam == 'W')
		Render.CamerPosZ -= 0.1;
	if (wParam == 'S')
		Render.CamerPosZ += 0.1;
	if (wParam == 'A')
		Render.CamerPosX += 0.1;
	if (wParam == 'D')
		Render.CamerPosX -= 0.1;
	if (wParam == 'Q')
		Render.CamerPosY -= 0.1;
	if (wParam == 'E')
		Render.CamerPosY += 0.1;
}


LRESULT CALLBACK WindowProc(
	_In_	HWND hwnd,
	_In_	UINT uMsg,
	_In_	WPARAM wParam,
	_In_	LPARAM lParam
)
{

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		KeyControl(wParam);
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



bool DEVICE::init()
{

	//init windows
	sprintf_s(title, "HI");
	hInstance = GetModuleHandle(NULL);

	Draw.cbClsExtra = 0;
	Draw.cbWndExtra = 0;
	Draw.hCursor = LoadCursor(hInstance, IDC_ARROW);
	Draw.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	Draw.lpszMenuName = NULL;
	Draw.style = WS_MINIMIZEBOX | CS_HREDRAW | CS_VREDRAW;
	Draw.hbrBackground = (HBRUSH)COLOR_WINDOW;
	Draw.lpfnWndProc = WindowProc;
	Draw.lpszClassName = title;
	Draw.hInstance = hInstance;

	RegisterClass(&Draw);

	hwnd = CreateWindow(
		title,
		title,
		WS_OVERLAPPEDWINDOW,
		38,
		20,
		SCREEN_WIDTH + 15,
		SCREEN_HEIGHT + 38,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	//init bitbuffer
	ZeroMemory(&binfo, sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount = bits;
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biSizeImage = 0;
	binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth = SCREEN_WIDTH;

	screen_hdc = GetDC(hwnd);
	hCompatibleDC = CreateCompatibleDC(screen_hdc);
	hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);


	return true;
}

void DEVICE::clean()
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 1] = 255 - 255 * (float)y / (float)SCREEN_HEIGHT;
			buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 2] = 255 - 255 * (float)y / (float)SCREEN_HEIGHT;
			buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 3] = 255 - 255 * (float)y / (float)SCREEN_HEIGHT;
			depth[y * SCREEN_WIDTH + x] = 20;
		}
	}
}


//更新屏幕buffer
void DEVICE::update()
{

	// put buffer To Screen
	SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, buffer, (BITMAPINFO*)& binfo, DIB_RGB_COLORS);
	BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
	// deal message
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void DEVICE::SetTitle(const char* title)
{
	SetWindowText(hwnd, title);
}

void DEVICE::DrawPoint(int x, int y,color Color)
{
	buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 1] = Color.r;
	buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 2] = Color.g;
	buffer[y * SCREEN_WIDTH * 3 + (x + 1) * 3 - 3] = Color.b;
}

color DEVICE::GetTexture(float u, float v)
{
	int y = v * bmpHeight;
	int x = u * bmpWidth;
	return color(pBmpBuf[y * bmpWidth * 3 + x * 3 + 2], pBmpBuf[y * bmpWidth * 3 + x * 3 + 1], pBmpBuf[y * bmpWidth * 3 + x * 3]);

}

bool DEVICE::LoadBmp(char* bmpName)
{
	FILE* fp = fopen(bmpName, "rb");//二进制读方式打开指定的图像文件
	if (fp == 0)
		return 0;

	//跳过位图文件头结构BITMAPFILEHEADER
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER infohead;
	fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fp); //获取图像宽、高、每像素所占位数等信息
	bmpWidth = infohead.biWidth;
	bmpHeight = infohead.biHeight;
	biBitCount = infohead.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	//showBmpInforHead(infohead);//显示信息头 

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256
	if (biBitCount == 8)
	{
		//申请颜色表所需要的空间，读颜色表进内存
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	//申请位图数据所需要的空间，读位图数据进内存
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	fclose(fp);//关闭文件
	Debuger.print("DEVIDE::readBmp read OK",mGREEN);
	return 1;//读取文件成功
}