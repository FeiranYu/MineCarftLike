#include"Minecarft.h"

// 旋转摄像机 (有问题需要解决)
void RENDER::MoveCamera(int direction, float step)
{
	if (direction == 0)
	{
		CameraAngleLeftRight += step;
		CameraDirection.z = -1 * cos(CameraAngleLeftRight);
		CameraDirection.x = sin(CameraAngleLeftRight);
	}
}

//位移矩阵
void RENDER::SetWorldMatTranslate(int x, int y, int z)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			worldMat1.m[i][j] = 0;

	for (int i = 0; i < 4; i++)
	{
		worldMat1.m[i][i] = 1;
	}
	worldMat1.m[3][0] = x;
	worldMat1.m[3][1] = y;
	worldMat1.m[3][2] = z;
}



//旋转矩阵
void RENDER::SetWorldMatRotation(int axis, float angle)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			worldMat2.m[i][j] = 0;
	if (axis == 0)	// x轴
	{
		worldMat2.m[0][0] = 1;
		worldMat2.m[1][1] = cos(angle);
		worldMat2.m[1][2] = sin(angle);
		worldMat2.m[2][1] = -sin(angle);
		worldMat2.m[2][2] = cos(angle);
		worldMat2.m[3][3] = 1;
	}
	if (axis == 1)		// y轴
	{
		worldMat2.m[0][0] = cos(angle);
		worldMat2.m[0][2] = -sin(angle);
		worldMat2.m[1][1] = 1;
		worldMat2.m[2][0] = sin(angle);
		worldMat2.m[2][2] = cos(angle);
		worldMat2.m[3][3] = 1;
	}
	if (axis == 2)			// z轴
	{
		worldMat2.m[0][0] = cos(angle);
		worldMat2.m[0][1] = sin(angle);
		worldMat2.m[1][0] = -sin(angle);
		worldMat2.m[1][1] = cos(angle);
		worldMat2.m[2][2] = 1;
		worldMat2.m[3][3] = 1;
	}
}


void RENDER::initCamerMat(vector const up, vector const at, vector const eye)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			camerMat.m[i][j] = 0;

	vector xaxis, yaxis, zaxis;

	vecSub(at, eye, zaxis);
	vecNormal(zaxis);
	vecMul(up, zaxis, xaxis);
	vecNormal(xaxis);
	vecMul(zaxis, xaxis, yaxis);

	camerMat.m[0][0] = xaxis.x;
	camerMat.m[1][0] = xaxis.y;
	camerMat.m[2][0] = xaxis.z;
	camerMat.m[3][0] = -vecDotMul(xaxis, eye);

	camerMat.m[0][1] = yaxis.x;
	camerMat.m[1][1] = yaxis.y;
	camerMat.m[2][1] = yaxis.z;
	camerMat.m[3][1] = -vecDotMul(yaxis, eye);

	camerMat.m[0][2] = zaxis.x;
	camerMat.m[1][2] = zaxis.y;
	camerMat.m[2][2] = zaxis.z;
	camerMat.m[3][2] = -vecDotMul(zaxis, eye);

	camerMat.m[3][3] = 1;

}

void RENDER::initProjectMat(float fov, float aspect, float zn, float zf)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			projectMat.m[i][j] = 0;
	projectMat.m[0][0] = 1 / (tan(fov * 0.5f) * aspect);
	projectMat.m[1][1] = 1 / tan(fov * 0.5f);
	projectMat.m[2][2] = zf / (zf - zn);
	projectMat.m[2][3] = 1.0f;
	projectMat.m[3][2] = (zn * zf) / (zn - zf);

}

void RENDER::SetCameraPos(float posx, float posy, float posz)
{
	CamerPosX = posx;
	CamerPosY = posy;
	CamerPosZ = posz;
}


void RENDER::CalFinalMat()
{
	CameraPos.x = CamerPosX; CameraPos.y = CamerPosY; CameraPos.z = CamerPosZ; CameraPos.w = 1;
	initCamerMat(CameraUp, CameraDirection, CameraPos);
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	initProjectMat(3.1415926 / 2, aspect, 1, 500);


	matMul(worldMat2, worldMat1, tm1);
	matMul(tm1, camerMat, tm2);
	matMul(tm2, projectMat, finalMat);
}


void RENDER::Transform(const vector& rawVec, const matrix& mat, vector& finalVec)
{
	finalVec.x = 0;
	finalVec.y = 0;
	finalVec.z = 0;
	finalVec.w = 0;

	finalVec.x = rawVec.x * mat.m[0][0] + rawVec.y * mat.m[1][0] + rawVec.z * mat.m[2][0] + rawVec.w * mat.m[3][0];
	finalVec.y = rawVec.x * mat.m[0][1] + rawVec.y * mat.m[1][1] + rawVec.z * mat.m[2][1] + rawVec.w * mat.m[3][1];
	finalVec.z = rawVec.x * mat.m[0][2] + rawVec.y * mat.m[1][2] + rawVec.z * mat.m[2][2] + rawVec.w * mat.m[3][2];
	finalVec.w = rawVec.x * mat.m[0][3] + rawVec.y * mat.m[1][3] + rawVec.z * mat.m[2][3] + rawVec.w * mat.m[3][3];


}


// 世界坐标到相机坐标
void RENDER::transform_homogenize(vector& y, vector& x) {
	float rhw = 1.0f / x.w;
	y.x = (x.x * rhw + 1.0f) * SCREEN_WIDTH * 0.5f;
	y.y = (1.0f - x.y * rhw) * SCREEN_HEIGHT * 0.5f;
	y.z = x.z * rhw;
	y.w = x.w;
}




void RENDER::CalTransform()
{

	for (int i = 0; i < pointSum; i++)
	{
		vector finalVV;

		Transform(Point[i], finalMat, finalV[i]);
		transform_homogenize(finalVV, finalV[i]);
		//对顶点进行剪裁
		FinalMesh[i].hide = CVVCheck(finalV[i]);
		FinalMesh[i].Vector = finalVV;
	}

}



bool RENDER::CVVCheck(const vector& vec)
{
	float w = vec.w;
	if (vec.x<-w || vec.x>w)
		return true;
	if (vec.y<-w || vec.y>w)
		return true;
	if (vec.z<0.0f || vec.z>w)
		return true;
	return false;
}

void RENDER::DrawPoint(const point& p)
{
	//cout << "Point z " << p.Vector.z << " depth " << depth[(int)p.Vector.y*SCREEN_WIDTH + (int)p.Vector.x] << endl;
	if (p.Vector.x > 0 && p.Vector.x < SCREEN_WIDTH && p.Vector.y>0 && p.Vector.y < SCREEN_HEIGHT && p.Vector.z < depth[(int)p.Vector.y * SCREEN_WIDTH + (int)p.Vector.x])
	{
		Device.DrawPoint(p.Vector.x, p.Vector.y, p.Color);
		depth[(int)p.Vector.y * SCREEN_WIDTH + (int)p.Vector.x] = p.Vector.z;
		//cout<< " depth now " << depth[(int)p.Vector.y * SCREEN_WIDTH + (int)p.Vector.x] << endl;
	}

}


void RENDER::DrawLine(const point& a, const point& b)
{
	if (a.Vector.x<0 || a.Vector.x>SCREEN_WIDTH || a.Vector.y<0 || a.Vector.y>SCREEN_HEIGHT || b.Vector.x<0 || b.Vector.x>SCREEN_WIDTH || b.Vector.y > 0 || b.Vector.y < SCREEN_HEIGHT)
		return;
	int x1 = a.Vector.x;
	int x2 = b.Vector.x;
	float z1 = a.Vector.z;
	float z2 = a.Vector.z;
	float w1 = a.Vector.w;
	float w2 = b.Vector.w;
	int y1 = a.Vector.y;
	int y2 = b.Vector.y;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;
	int uy = ((dy > 0) << 1) - 1;
	int x = x1, y = y1, eps;



	eps = 0; dx = abs(dx); dy = abs(dy);

	point p;
	p.u = 0;
	p.v = 0;

	p.Color = color(0, 0, 0);
	//cout <<"a z"<< a.Vector.z << endl;
	//cout << "b z" << b.Vector.z << endl;

	if (dx > dy)
	{
		for (x = x1; x != x2; x += ux)
		{
			p.Vector = vector(x, y, 0);
			float tw1 = 1 / w1;
			float tw2 = 1 / w2;
			p.Vector.z = 1 / interp(x, x1, x2, tw1, tw2);
			//p.Vector.z = interp(x, x1, x2, z1 / w1, z2 / w2) * 1 / p.Vector.w;
			//cout << "P z" << p.Vector.z << endl;
			DrawPoint(p);
			eps += dy;
			if ((eps << 1) >= dx)
			{
				y += uy; eps -= dx;
			}
		}

	}
	else
	{
		for (y = y1; y != y2; y += uy)
		{
			p.Vector = vector(x, y, 0);
			float tw1 = 1 / w1;
			float tw2 = 1 / w2;
			p.Vector.z = 1 / interp(x, x1, x2, tw1, tw2);
			//p.Vector.z = interp(x, x1, x2, z1 / w1, z2 / w2) * 1 / p.Vector.w;
			//cout << "P z" << p.Vector.z << endl;
			DrawPoint(p);
			eps += dx;
			if ((eps << 1) >= dy)
			{
				x += ux; eps -= dy;
			}
		}
	}
}



void RENDER::DrawScanLine(const point& leftp, const point& rightp)
{
	point p;
	for (int i =max(leftp.Vector.x,0); i < min(rightp.Vector.x,SCREEN_WIDTH); i++)
	{

		p.Vector = vector(i, leftp.Vector.y, leftp.Vector.z);
		p.Vector.z = 1 / interp(i, leftp.Vector.x, rightp.Vector.x, 1 / leftp.Vector.z, 1 / rightp.Vector.z);
		//cout << "leftz " << leftp.Vector.z << " rightz " << rightp.Vector.z << " p z " << p.Vector.z << endl;
		p.u = interp((float)i, leftp.Vector.x, rightp.Vector.x, leftp.u / leftp.Vector.z, rightp.u / rightp.Vector.z) * p.Vector.z;
		p.v = interp(i, leftp.Vector.x, rightp.Vector.x, leftp.v / leftp.Vector.z, rightp.v / rightp.Vector.z) * p.Vector.z;
		//cout << p.Vector.z << endl;

		//if(i==leftp.Vector.x+20)
			//cout << "u " << p.u << " v " << p.v << endl;
		p.Color = Device.GetTexture(p.u, p.v);
		
		DrawPoint(p);
	}
}

void RENDER::DrawTriangle(const point& p1, const point& p2, const point& p3, color Color)
{
	point P1, P2, P3;
	P1 = p1; P2 = p2; P3 = p3;

	//cout << "P1 u " << P1.u << " v " << P1.v << " P2 u " << P2.u << " v " << P2.v << " P3 u " << P3.u << " v " << P3.v << endl;

	point temp;
	if (P1.hide == 1 && P2.hide == 1 && P3.hide == 1)
	{
		return;
	}
	
	/*
	if (P1.Vector.x<-0 || P1.Vector.x>SCREEN_WIDTH  || P1.Vector.y<-0 || P1.Vector.y>SCREEN_HEIGHT  || 
		P2.Vector.x<-0 || P2.Vector.x>SCREEN_WIDTH  || P2.Vector.y<-0 || P2.Vector.y>SCREEN_HEIGHT  ||
		P3.Vector.x<-0 || P3.Vector.x>SCREEN_WIDTH  || P3.Vector.y<-0 || P3.Vector.y>SCREEN_HEIGHT)
	{
		return;
	}
	*/

	if (P2.Vector.y < P1.Vector.y)
	{
		temp = P2;
		P2 = P1;
		P1 = temp;
	}
	if (P3.Vector.y < P1.Vector.y)
	{
		temp = P3;
		P3 = P1;
		P1 = temp;
	}
	if (P2.Vector.y > P3.Vector.y)
	{
		temp = P2;
		P2 = P3;
		P3 = temp;
	}
	// y: P1<P2<P3

	int left, right;
	point pleft, pright;
	if (P1.Vector.y == P2.Vector.y)
	{
		for (int y = P1.Vector.y; y < min(P3.Vector.y,SCREEN_WIDTH); y++)
		{

			left = interp(y, P1.Vector.y, P3.Vector.y, P1.Vector.x, P3.Vector.x);
			right = interp(y, P2.Vector.y, P3.Vector.y, P2.Vector.x, P3.Vector.x);

			float zleft_t = interp(y, P1.Vector.y, P3.Vector.y, 1 / P1.Vector.w, 1 / P3.Vector.w);
			float zright_t = interp(y, P2.Vector.y, P3.Vector.y, 1 / P2.Vector.w, 1 / P3.Vector.w);

			float uleft = interp(y, P1.Vector.y, P3.Vector.y, P1.u / P1.Vector.w, P3.u / P3.Vector.w) * (1 / zleft_t);
			float vleft = interp(y, P1.Vector.y, P3.Vector.y, P1.v / P1.Vector.w, P3.v / P3.Vector.w) * (1 / zleft_t);
			float uright = interp(y, P2.Vector.y, P3.Vector.y, P2.u / P2.Vector.w, P3.u / P3.Vector.w) * (1 / zright_t);
			float vright = interp(y, P2.Vector.y, P3.Vector.y, P2.v / P2.Vector.w, P3.v / P3.Vector.w) * (1 / zright_t);



			if (left > right)
			{
				int temp = left;
				left = right;
				right = temp;
				float ztemp = zleft_t;
				zleft_t = zright_t;
				zright_t = ztemp;
				float utemp = uleft;
				uleft = uright;
				uright = utemp;
				float vtemp = vleft;
				vleft = vright;
				vright = vtemp;
			}

			pleft.Vector = vector(left, y, 1 / zleft_t);
			pleft.Color = Color;
			pright.Vector = vector(right, y, 1 / zright_t);
			pright.Color = Color;

			pleft.u = uleft;
			pleft.v = vleft;
			pright.u = uright;
			pright.v = vright;
			if(pleft.Vector.y>0&& pleft.Vector.y<SCREEN_HEIGHT)
				DrawScanLine(pleft, pright);

		}
	}
	else if (P2.Vector.y == P3.Vector.y)
	{
		for (int y = P1.Vector.y; y < min(P2.Vector.y,SCREEN_WIDTH); y++)
		{

			left = interp(y, P1.Vector.y, P2.Vector.y, P1.Vector.x, P2.Vector.x);
			right = interp(y, P1.Vector.y, P3.Vector.y, P1.Vector.x, P3.Vector.x);
			float zleft_t = interp(y, P1.Vector.y, P2.Vector.y, 1 / P1.Vector.w, 1 / P2.Vector.w);
			float zright_t = interp(y, P1.Vector.y, P3.Vector.y, 1 / P1.Vector.w, 1 / P3.Vector.w);

			float uleft = interp(y, P1.Vector.y, P2.Vector.y, P1.u / P1.Vector.w, P2.u / P2.Vector.w) * (1 / zleft_t);
			float vleft = interp(y, P1.Vector.y, P2.Vector.y, P1.v / P1.Vector.w, P2.v / P2.Vector.w) * (1 / zleft_t);
			float uright = interp(y, P1.Vector.y, P3.Vector.y, P1.u / P1.Vector.w, P3.u / P3.Vector.w) * (1 / zright_t);
			float vright = interp(y, P1.Vector.y, P3.Vector.y, P1.v / P1.Vector.w, P3.v / P3.Vector.w) * (1 / zright_t);
			if (left > right)
			{
				int temp = left;
				left = right;
				right = temp;
				float ztemp = zleft_t;
				zleft_t = zright_t;
				zright_t = ztemp;
				float utemp = uleft;
				uleft = uright;
				uright = utemp;
				float vtemp = vleft;
				vleft = vright;
				vright = vtemp;
			}

			pleft.Vector = vector(left, y, 1 / zleft_t);
			pleft.Color = Color;
			pright.Vector = vector(right, y, 1 / zright_t);
			pright.Color = Color;
			pleft.u = uleft;
			pleft.v = vleft;
			pright.u = uright;
			pright.v = vright;

			if (pleft.Vector.y > 0 && pleft.Vector.y < SCREEN_HEIGHT)
				DrawScanLine(pleft, pright);
		}
	}
	else
	{
		for (int y = P1.Vector.y + 1; y < min(P2.Vector.y,SCREEN_WIDTH); y++)
		{

			left = interp(y, P1.Vector.y, P2.Vector.y, P1.Vector.x, P2.Vector.x);
			right = interp(y, P1.Vector.y, P3.Vector.y, P1.Vector.x, P3.Vector.x);
			float zleft_t = interp(y, P1.Vector.y, P2.Vector.y, 1 / P1.Vector.w, 1 / P2.Vector.w);
			float zright_t = interp(y, P1.Vector.y, P3.Vector.y, 1 / P1.Vector.w, 1 / P3.Vector.w);


			float uleft = interp(y, P1.Vector.y, P2.Vector.y, P1.u / P1.Vector.w, P2.u / P2.Vector.w) * (1 / zleft_t);
			float vleft = interp(y, P1.Vector.y, P2.Vector.y, P1.v / P1.Vector.w, P2.v / P2.Vector.w) * (1 / zleft_t);
			float uright = interp(y, P1.Vector.y, P3.Vector.y, P1.u / P1.Vector.w, P3.u / P3.Vector.w) * (1 / zright_t);
			float vright = interp(y, P1.Vector.y, P3.Vector.y, P1.v / P1.Vector.w, P3.v / P3.Vector.w) * (1 / zright_t);
			if (left > right)
			{
				int temp = left;
				left = right;
				right = temp;
				float ztemp = zleft_t;
				zleft_t = zright_t;
				zright_t = ztemp;
				float utemp = uleft;
				uleft = uright;
				uright = utemp;
				float vtemp = vleft;
				vleft = vright;
				vright = vtemp;
			}

			pleft.Vector = vector(left, y, 1 / zleft_t);
			pleft.Color = Color;
			pright.Vector = vector(right, y, 1 / zright_t);
			pright.Color = Color;

			pleft.u = uleft;
			pleft.v = vleft;
			pright.u = uright;
			pright.v = vright;

			if (pleft.Vector.y > 0 && pleft.Vector.y < SCREEN_HEIGHT)
				DrawScanLine(pleft, pright);
		}
		for (int y = P2.Vector.y + 1; y < min(P3.Vector.y,SCREEN_WIDTH); y++)
		{

			left = interp(y, P2.Vector.y, P3.Vector.y, P2.Vector.x, P3.Vector.x);
			right = interp(y, P1.Vector.y, P3.Vector.y, P1.Vector.x, P3.Vector.x);
			float zleft_t = interp(y, P2.Vector.y, P3.Vector.y, 1 / P2.Vector.w, 1 / P3.Vector.w);
			float zright_t = interp(y, P1.Vector.y, P3.Vector.y, 1 / P1.Vector.w, 1 / P3.Vector.w);

			float uleft = interp(y, P2.Vector.y, P3.Vector.y, P2.u / P2.Vector.w, P3.u / P3.Vector.w) * (1 / zleft_t);
			float vleft = interp(y, P2.Vector.y, P3.Vector.y, P2.v / P2.Vector.w, P3.v / P3.Vector.w) * (1 / zleft_t);
			float uright = interp(y, P1.Vector.y, P3.Vector.y, P1.u / P1.Vector.w, P3.u / P3.Vector.w) * (1 / zright_t);
			float vright = interp(y, P1.Vector.y, P3.Vector.y, P1.v / P1.Vector.w, P3.v / P3.Vector.w) * (1 / zright_t);
			if (left > right)
			{
				int temp = left;
				left = right;
				right = temp;
				float ztemp = zleft_t;
				zleft_t = zright_t;
				zright_t = ztemp;
				float utemp = uleft;
				uleft = uright;
				uright = utemp;
				float vtemp = vleft;
				vleft = vright;
				vright = vtemp;
			}

			pleft.Vector = vector(left, y, 1 / zleft_t);
			pleft.Color = Color;
			pright.Vector = vector(right, y, 1 / zright_t);
			pright.Color = Color;

			pleft.u = uleft;
			pleft.v = vleft;
			pright.u = uright;
			pright.v = vright;

			if (pleft.Vector.y > 0 && pleft.Vector.y < SCREEN_HEIGHT)
				DrawScanLine(pleft, pright);
		}
	}
}


void RENDER::DrawMesh(const Object *object)
{
	int lastPointSum = pointSum;
	pointSum = object->pointSum;
	Point = object->Point;
	indexSum = object->indexSum;
	Index = object->Index;

	unsigned char* pBmpBuf;//读入图像数据的指针
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD* pColorTable;//颜色表指针
	int biBitCount;//图像类型，每像素位数



	Device.biBitCount = object->texture->biBitCount;
	Device.pColorTable = object->texture->pColorTable;
	Device.bmpWidth = object->texture->bmpWidth;
	Device.bmpHeight = object->texture->bmpHeight;
	Device.pBmpBuf = object->texture->pBmpBuf;

	if (lastPointSum != pointSum)
	{
		delete[] c_mesh;
		delete[] finalV;
		delete[] FinalMesh;
		c_mesh = new color[pointSum];
		finalV = new vector[pointSum];
		FinalMesh = new point[pointSum];
		if (!FinalMesh)
			return;
		if (!c_mesh)
			return;
		if (!finalV)
			return;
	}
	

	CalFinalMat();
	CalTransform();

	for (int i = 0; i < indexSum; i++)
	{

		FinalMesh[i].hide = true;

		if (FinalMesh[i].Vector.x > 0 && FinalMesh[i].Vector.x < SCREEN_WIDTH && FinalMesh[i].Vector.y>0 && FinalMesh[i].Vector.y < SCREEN_HEIGHT && FinalMesh[i].Vector.z < depth[(int)FinalMesh[i].Vector.y * SCREEN_WIDTH + (int)FinalMesh[i].Vector.x])
		{
			FinalMesh[i].hide = false;

		}
		else
		{
			//char strMsg[100];
			//sprintf(strMsg, "FinalMesh[%d].hide=true", i);
			//Debuger.print(strMsg, mRED);
		}

		int a = Index[i].a;
		int b = Index[i].b;
		int c = Index[i].c;

		FinalMesh[a].u = Index[i].ua;
		FinalMesh[a].v = Index[i].va;
		FinalMesh[b].u = Index[i].ub;
		FinalMesh[b].v = Index[i].vb;
		FinalMesh[c].u = Index[i].uc;
		FinalMesh[c].v = Index[i].vc;

		DrawLine(FinalMesh[a], FinalMesh[b]);
		DrawLine(FinalMesh[c], FinalMesh[b]);
		DrawLine(FinalMesh[a], FinalMesh[c]);

		DrawTriangle(FinalMesh[a], FinalMesh[b], FinalMesh[c]);

	}
}

void RENDER::AddDrawObject( Object& object, float px, float py, float pz, int rotationAxis, float angle)
{
	DrawObject* NewDrawObject = new DrawObject;
	NewDrawObject->object = object;
	NewDrawObject->pos.x = px;
	NewDrawObject->pos.y = py;
	NewDrawObject->pos.z = pz;
	NewDrawObject->rotationAxis = rotationAxis;
	NewDrawObject->rotationAngle = angle;

	if (DrawObjectList != NULL)
	{
		NewDrawObject->beforeDrawObject = DrawObjectList;
	}

	DrawObjectList = NewDrawObject;
	DrawObjectSum++;

}

void RENDER::Draw()
{
	int DrawNowIndex = 0;
	DrawObject* NowDrawObject = DrawObjectList;
	while (DrawNowIndex < DrawObjectSum)
	{
		Render.SetWorldMatTranslate(NowDrawObject->pos.x, NowDrawObject->pos.y, NowDrawObject->pos.z);
		Render.SetWorldMatRotation(NowDrawObject->rotationAxis, NowDrawObject->rotationAngle);
		Render.DrawMesh(&NowDrawObject->object);



		NowDrawObject = NowDrawObject->beforeDrawObject;
		//delete TempDrawObject;
		DrawNowIndex++;
	}

}