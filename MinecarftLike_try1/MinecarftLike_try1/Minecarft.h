//--------------------------------------------------------------------------------------
//		MinecarftLike Try0
//
//
//
//
//
//--------------------------------------------------------------------------------------


#pragma once
#define _CRT_SECURE_NO_WARNINGS

using namespace std;


#include<Windows.h>
#include<time.h>
#include<tchar.h>
#include<iostream>
#include<fstream>
#include<stdint.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int bits = 24;
const int MAX_DEPTH = -9999;

extern float angle;

extern bool gameoverflag;



class color
{
public:
	color(int R, int G, int B) :r(R), g(G), b(B) {}
	color() :r(255), g(255), b(255) {}
	int r, g, b;
};

class vector
{
public:
	vector(float X, float Y, float Z, float W = 1) :x(X), y(Y), z(Z), w(W) {}
	vector() :x(0), y(0), z(0), w(1) {}
	float x, y, z, w;
};

class point
{
public:
	color Color;
	vector Vector;
	float u=0, v=0;
	bool hide = 0;
};

class matrix
{
public:
	float m[4][4];
};

struct TriangleIndex
{
	int a, b, c;
	float ua, va, ub, vb, uc, vc;
};

struct Texture
{
	unsigned char* pBmpBuf;//读入图像数据的指针
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD* pColorTable;//颜色表指针
	int biBitCount;//图像类型，每像素位数
};


enum ObjectType{tTEXTURE,tCOLOR};
struct Object
{
	int pointSum;
	vector* Point;

	int indexSum;
	TriangleIndex* Index;

	ObjectType objecttype;
	Texture *texture;
	color Color;
};

struct DrawObject
{
	Object object;
	vector pos;
	int rotationAxis;
	float rotationAngle;
	DrawObject* beforeDrawObject;
};


extern BYTE buffer[SCREEN_WIDTH * SCREEN_HEIGHT * bits / 8];
extern float depth[SCREEN_WIDTH * SCREEN_HEIGHT];


extern Object* cube;

class DEVICE
{
public:
	bool init();
	void clean();
	void update();
	void DrawPoint(int x, int y,color Color=color(0,0,0));
	void SetTitle(const char*);
	color GetTexture(float u, float v);
	Texture* LoadBmp(char* bmpName);
	Object* LoadObject(const char*);
	
	HDC screen_hdc;

	HWND hwndShowWindow;
	HWND hwndCmd;
private:
	friend class RENDER;
	//windows data
	HINSTANCE hInstance;
	WNDCLASS Draw;

	MSG msg;
	char title[100];

	//draw data

	HDC hCompatibleDC;
	HBITMAP hCompatibleBitmap;
	HBITMAP hOldBitmap;
	BITMAPINFO binfo;


	//texture data
	unsigned char* pBmpBuf;//读入图像数据的指针
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD* pColorTable;//颜色表指针
	int biBitCount;//图像类型，每像素位数

};




enum DEBUG_MSG_TYPE{ mRED,mGREEN,mBLUE,mWHITE };
class DEBUGER
{
public:
	void print(const char* msg,DEBUG_MSG_TYPE mTYPE =mWHITE);
};

class RENDER
{
public:
	RENDER()
	{
		CameraUp.x = 0; CameraUp.y = 1; CameraUp.z = 0; CameraUp.w = 1;
		CameraDirection.x = 0; CameraDirection.y = 0; CameraDirection.z = -1; CameraDirection.w = 1;
		CameraAngleUpDown = 0;
		CameraAngleLeftRight = 0;
		DrawObjectSum = 0;

	}

	void MoveCamera(int direction, float step);
	void SetWorldMatTranslate(int x, int y, int z);
	void SetWorldMatRotation(int axis, float angle);
	void SetCameraPos(float posx, float posy, float posz);
	void initCamerMat(vector const up, vector const at, vector const eye);
	void initProjectMat(float fov, float aspect, float zn, float zf);
	void CalFinalMat();
	void Transform(const vector& rawVec, const matrix& mat, vector& finalVec);
	void transform_homogenize(vector& y, vector& x);
	void CalTransform();
	bool CVVCheck(const vector& vec);
	void DrawPoint(const point& p);
	void DrawLine(const point& a, const point& b);
	void DrawScanLine(const point& leftp, const point& rightp);
	void DrawTriangle(const point& p1, const point& p2, const point& p3, color Color = color(255, 255, 255));
	void DrawMesh(const Object *);
	void AddDrawObject(Object& object, float px=0, float py=0, float pz=0, int rotationAxis=0, float angle=0);
	void Draw();
	
	color *c_mesh;
	point *FinalMesh;
	vector *finalV;

	int pointSum;
	vector *Point;

	int indexSum;
	TriangleIndex* Index;

	float CamerPosX = 0;
	float CamerPosY = 0;
	float CamerPosZ = 6;

	vector CameraUp;
	vector CameraDirection;
	vector CameraPos;
	float CameraAngleUpDown;
	float CameraAngleLeftRight;

	DrawObject* DrawObjectList;
	int DrawObjectSum;

private:
	
	
	matrix worldMat1;
	matrix worldMat2;
	matrix camerMat;
	matrix projectMat;
	matrix finalMat;
	matrix tm1, tm2;	//矩阵相乘时的临时矩阵
};

class COMMANDLINE
{
public:
	void Input();
	void parser();
private:
	const int MAX_LINESUM = 100;
	char cmd[100];
};

void KeyControl(WPARAM wParam);

float interp(const float& y, const float& y0, const  float& y1, const  float& x0, const  float& x1);
float vecLength(const vector& v);
void vecNormal(vector& v);
void vecMul(const vector& a, const vector& b, vector& c);
void vecSub(const vector& a, const vector& b, vector& c);
float vecDotMul(const vector& a, const vector& b);
void matMul(const matrix& a, const matrix& b, matrix& c);
void showMat(const matrix& mat);

extern COMMANDLINE Commandline;
extern DEBUGER Debuger;
extern DEVICE Device;
extern RENDER Render;
