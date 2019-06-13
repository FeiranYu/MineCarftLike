#include"Minecarft.h"

void COMMANDLINE::Input()
{
	BringWindowToTop(Device.hwndCmd);
	cout << ">>";
	cin.getline(cmd, MAX_LINESUM);
	parser();
}

char* FindStr(char* msg, int& index)
{
	bool FindStringFlag = false;
	int strLeft = -1;
	int strRight = -1;
	for (int i = index; i < strlen(msg); i++)
	{
		if (msg[i] >= 'a' && msg[i] <= 'z')
		{
			if (!FindStringFlag)
			{
				FindStringFlag = true;
			}
			if (strLeft == -1)
			{
				strLeft = i;
			}
			strRight = i;
			index = i + 1;

		}
		else {
			if (FindStringFlag)
			{
				break;
			}
		}
	}
	if (strRight != -1)
	{
		int len = strRight - strLeft+1;
		char *NewStr = new char[100];
		memset(NewStr, 0, 100);
		int i = 0;
		for (; i <len; i++)
		{
			NewStr[i] = msg[i + strLeft];
		}

		return NewStr;
	}
	else {
		return NULL;
	}
}

float FindFloat(char* msg, int& index)
{
	float num = 0;
	int indexDot = -1;
	bool FindFloatFlag = false;
	bool negFlag = false;
	for (int i = index; i < strlen(msg); i++)
	{
		
		if (msg[i] >= '0' && msg[i] <= '9')
		{
			if (!FindFloatFlag)
			{
				if (i - 1 > 0 && msg[i - 1] == '-')
				{
					negFlag = true;
				}
				FindFloatFlag = true;
			}
			if (indexDot == -1)
			{
				num *= 10;
				num += msg[i] - '0';
				index = i + 1;
			}
			else
			{
				num += float(msg[i] - '0') / (float)pow(10, i - indexDot);
				index = i + 1;
			}
		}
		else if (msg[i] == '.')
		{
			indexDot = i;
		}
		else
		{	
			if (FindFloatFlag)
			{
				break;
				Debuger.print("no find float in str", mRED);
			}
		}
	}
	if (negFlag)
	{
		return -num;
	}
	else
	{
		return num;
	}
}

void COMMANDLINE::parser()
{
	if (strcmp(cmd, "exit") == 0)
	{
		Debuger.print("exit",mWHITE);
		gameoverflag = true;
	}
	char cmd1[] = "setpos";
	bool cmd1Flag = true;
	for (int i = 0; i < strlen(cmd1); i++)
	{
		if (cmd[i] != cmd1[i])
		{
			cmd1Flag = false;
		}
	}
	int scanIndex = strlen(cmd1);
	if (cmd1Flag)
	{
		float px, py, pz;
		px = FindFloat(cmd, scanIndex);
		py = FindFloat(cmd, scanIndex);
		pz = FindFloat(cmd, scanIndex);

		char msgStr[100];
		sprintf(msgStr, "x%.2f y%.2f z%.2f", px, py, pz);
		Debuger.print(msgStr, mGREEN);
		Render.SetCameraPos(px, py, pz);
	}
	char cmd2[] = "add";
	bool cmd2Flag = true;
	for (int i = 0; i < strlen(cmd2); i++)
	{
		if (cmd[i] != cmd2[i])
		{
			cmd2Flag = false;
		}
	}
	scanIndex = strlen(cmd2);
	if (cmd2Flag)
	{
		Debuger.print("ADD CALL!", mGREEN);
		float px, py, pz;
		char* objectname;
		objectname = FindStr(cmd, scanIndex);

		Debuger.print(objectname, mBLUE);
		px = FindFloat(cmd, scanIndex);
		py = FindFloat(cmd, scanIndex);
		pz = FindFloat(cmd, scanIndex);
		if (strcmp(objectname, "cube") == 0)
		{
			Render.AddDrawObject(*cube, px, py, pz);
			Debuger.print("Cube Create!", mGREEN);
		}

		
	}

	
	BringWindowToTop(Device.hwndShowWindow);
}