#include "stdafx.h"
#include "NodeKernel.h"



//����������� ������� ������ �����
void NodeKernel::Calc(int startNode, int endNode)
{
	Global.visitedNodes.push_back(startNode);
	std::map<int, std::vector<int>> sheets = Global.MapGlobal[startNode];
	for (int sheetN = 0; sheetN < sheets.size(); sheetN++)				//���� �� ����� �� ����
	{
		std::vector<int> nodes = sheets[sheetN];
		for (int nodeN = 0; nodeN < nodes.size(); nodeN++)				//���� �� ����� �� ����
		{
			if (nodes[nodeN] == endNode)								//���� ����� �������� �����, �� ���������� � �������������� ���������
			{
				Global.mapResult.push_back(Global.visitedNodes);
				return;
			}
			else
			{
				if (std::find(Global.visitedNodes.begin(), Global.visitedNodes.end(), nodes[nodeN]) != Global.visitedNodes.end())
					continue;
				else
				{
					Calc(nodes[nodeN], endNode);
					Global.visitedNodes.pop_back();
				}
			}
		}
	}
}
//������������� ���������� �����
void NodeKernel::MapGlobalInit(char * fileName)
{
	std::map<int, std::vector<int>> Buffer2;
	std::vector<int> VectorBuf;
	std::map<int, int> Buffer;
	int LevelBuf = 0, NUM = 0, level1 = 0, level2 = 0;
	Buffer.clear();
	int flag = 0;
	char file2[256] = { 0 }, buf[256] = { 0 };
	FILE* file = fopen(fileName, "rt");
	Buffer2.clear();
	for (;;)
	{
		if (fgets(buf, 256, file) == NULL)		// ���� ���� ����������
			break;
		LevelBuf = 0;
		NUM = 0;
		std::string str = buf;
		for (int i = 0; i < str.length() - 1; i++)		//������� ����� � ��� ����� � ������� ������������
		{
			if (buf[i] != '>')
				NUM = NUM * 10 + (buf[i] - '0');
			else
				LevelBuf++;
		}
		switch (LevelBuf)
		{
		case 1:
		{
			if (flag == 1)
			{
				if (VectorBuf.empty() != 1)
					Buffer2.insert(std::make_pair(level2, VectorBuf));
				level2 = NUM;
				VectorBuf.clear();
				flag = 0;
			}
			if (Buffer2.empty() != 1)
				Global.MapGlobal.insert(std::make_pair(level1, Buffer2));
			Buffer2.clear();
			VectorBuf.clear();
			level1 = NUM;
			break;
		}
		case 2:
		{
			if (VectorBuf.empty() != 1)
				Buffer2.insert(std::make_pair(level2, VectorBuf));
			level2 = NUM;
			VectorBuf.clear();
			flag = 0;
			break;
		}
		case 3:
		{
			flag = 1;
			VectorBuf.push_back(NUM);
			break;
		}
		default:
			break;
		}
	}

	if (flag == 1)
	{
		if (VectorBuf.empty() != 1)
			Buffer2.insert(std::make_pair(level2, VectorBuf));
		level2 = NUM;
		VectorBuf.clear();
		flag = 0;
	}
	if (Buffer2.empty() != 1)
		Global.MapGlobal.insert(std::make_pair(level1, Buffer2));
	Buffer2.clear();
	VectorBuf.clear();
	level1 = NUM;
	fclose(file);
}
//������������� ��������� �����
void NodeKernel::MapLocalInit(char * fileName)
{
	std::map<int, int> Buffer;
	FILE*	file = fopen(fileName, "rt");
	char buf[256] = { 0 };
	int flag = 0;
	int LevelBuf = 0, NUM = 0, level1 = 0, level2 = 0;
	Buffer.clear();
	for (;;)											//���������� ���������� ��� ���������
	{
		LevelBuf = 0;
		NUM = 0;
		if (fgets(buf, 256, file) == NULL)		// ���� ���� �� ��������
			break;
		std::string str = buf;


		for (int i = 0; i < str.length() - 1; i++)		//������� ����� � ��� ����� � ������� ������������
		{
			if (buf[i] != '>')
				NUM = NUM * 10 + (buf[i] - '0');
			else
				LevelBuf++;
		}

		switch (LevelBuf)
		{
		case 1:
		{
			if (Buffer.empty() != 1)
				Global.MapLocal.insert(std::make_pair(level1, Buffer));
			Buffer.clear();
			level1 = NUM;
			break;
		}
		case 2:
		{
			level2 = NUM;
			flag = 0;
			break;
		}
		case 3:
		{
			Buffer.insert(std::make_pair(level2, NUM));
			flag = 1;
			break;
		}
		default:
			break;
		}
	}
	Global.MapLocal.insert(std::make_pair(level1, Buffer));
	Buffer.clear();
	fclose(file);
}
//��������� ����� �� �������� � ���� ����� � ���� �� ������ ����
void NodeKernel::CreatePacGlobal(std::vector<int> PacGlobal)
{
}
//��������� ����� �� �������� � ���� ��� � ���� �� ������ ����
void NodeKernel::CreatePacLocal(std::vector<int> PacLocal)
{
}
//�������� ������ �� ���������� ����� ��� �������� ������ ���� �����
void NodeKernel::PushPacMesh(char * filename)
{
	PauseThreads(2);								//��� ���� ����� ������ ������ ������� �������, ������ ��� ������, ����� ��������, �� �����.
	FILE*	file = fopen(filename, "rt");
	char buf[256] = { 0 };
	std::string str;
	std::vector<int> VectorBuf;						//��� ���� ����� ������ � ��������� ������<������>
	int choise;
	for (int Where = 0;; Where++)					//���� �� �������� � �����
	{
		if (fgets(buf, 256, file) == NULL)			//���� ������ �� �������� ������ ���� ��������, �� � ������ ������� ������
			break;
		str = buf;									//����������� � ��� ������ ������� ����� ���������� ���� ������ �� ������� ��� ������� ������� ������
		if (str.at(1) != 1)							//????�������� �� ������, � ������ �� ������ ���������. ����� ������ � ����������, ���������� �������� ��������� ������ ��� � ���������� ��������� ���������.
		{
			for (int z = 1; z < str.length(); z++)	//����������� �� ������
			{
				if (System::Convert::ToChar(str.at(z)) <= '9' && System::Convert::ToChar(str.at(z)) >= '0')			//��� ��� ���������� ������ �����
				{
					int Buf = 0;
					for (; System::Convert::ToChar(str.at(z)) <= '9' && System::Convert::ToChar(str.at(z)) >= '0';)	//�� ������ � �����
					{
						Buf = Buf * 10 + (System::Convert::ToChar(str.at(z)) - 48);
						z++;
					}
					z--;
					VectorBuf.push_back(Buf);
				}
				else
				{
					if (System::Convert::ToChar(str.at(z)) == ' ' || System::Convert::ToChar(str.at(z)) == '\t' || System::Convert::ToChar(str.at(z)) == '\n')
						continue;
					else
						Sleep(1); //����� ������ ������ � ������, ������ ��� ���������� ���� ��������� ��� ������������ ����� ������
				}
			}
			if (System::Convert::ToChar(str.at(0)) > 48 && System::Convert::ToChar(str.at(0)) < 58)
				choise = 1;
			else
				choise = 2;
			switch (choise)
			{
			case 1:
			{
				Thread.MeshIn.at(str.at(0) - 48).push_back(VectorBuf);
				break;
			}
			default:
			{
				Sleep(1); //����� ���� ����, ����� ������ ������.
				break;
			}
			}
			VectorBuf.clear();
		}
		else                                        //����� � ������. ������� ����� ����� ����� ���� - ��.
		{
			Sleep(1);
		}
	}
	fclose(file);
}
//�������� ������ ��� �������� ������ ���� ������
void NodeKernel::PushPacUSU(char * filename)
{
}
//���������� ������� ��� �� ����������� ����
int NodeKernel::HowManySheets()
{
	if (Global.MapGlobal.size() <= 1)
		return -1;
	return Global.MapGlobal.at(Global.now).size();
}
//�������� ������� �����
int NodeKernel::ShowWays(int from, int where)
{
	Global.mapResult.clear();
	Global.visitedNodes.clear();
	Calc(from, where);
	return System::Convert::ToInt64(Global.mapResult.size());
}
//������� ����� ����� ��� ���� ������
int NodeKernel::CreateMeshThread()
{
	//������� ���� ���������� ����� ������ ������� �������
	return 0;
}
//��������� ������ �� �����
bool NodeKernel::PauseThreads(int mode)
{
	//������� ���� ��������� �� ���� ��������� ��������� ������ �� �����������, ����� ���� ��������� �� ����� ������ � ���������� ��������.
	return 0;
}
//���������� ���������� �������
bool NodeKernel::ContinueThreads(int mode)
{
	return 0;
}
//�� ����� ���� ���� �������� �����
int NodeKernel::ShowSheet(int from, int where)
{
	Global.mapResult.clear();
	Global.visitedNodes.clear();
	Calc(from, where);
	for (int i = 0; i < 999999; i++)																//���� �� ������ ���������
		for (int j = 0; j < Global.mapResult.size(); j++)											//���� �� �������������� ���� � ������ ����� � ��� ���������� ��������� ����
			if (Global.mapResult.at(j).size() == i)													//���� ����� ���������� ��������� ����, �� ���� ����� �� ����� ���� �� �������
				for (int y = 1; y < Global.MapGlobal.at(from).size() + 1; y++)						//���� �� ����� �� ����������� ����
					for (int z = 1; z < Global.MapGlobal.at(from).at(y).size() + 1; z++)			//���� �� ����� �� ���� ������
						if (Global.MapGlobal.at(from).at(y).at(z) == Global.mapResult.at(j).at(0))	//���� ����� ������ ���� �� ����, �� ���������� �� ����� ���� �����.
							return y;																//���������� ����� ����
}
//������������ ����� �� ��������
void NodeKernel::Events(std::vector<int> OnePac)
{
	//�� ���� �������� �����, ������� ������������ (�� ������ � ��������, � ����������� �����������)
	//������������ ������ ������� � OnePac
	Global.PacEvent.clear();
	Global.PacEvent = OnePac;
}
//����������� ������
NodeKernel::NodeKernel(char* Localtxt, char* Globaltxt, int dream, int now)			//����������� ������
{
	Global.dream = dream;
	Global.now = now;
	Global.hProcess = OpenMutex(MUTEX_ALL_ACCESS, false, "Process");
	MapLocalInit(Localtxt);
	MapGlobalInit(Globaltxt);
}
//���������� ������
NodeKernel::~NodeKernel()
{
}