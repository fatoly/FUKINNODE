#include "stdafx.h"
#include "NodeKernel.h"



//	+	����������� ������� ������ �����
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
//	+	������������� ���������� ����� �� ���������� �����
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
//	+	������������� ��������� ����� �� ���������� �����
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
//		��������� ����� �� �������� � ���� ����� � ���� �� ������ ���� ������� ������ � ���� �� �� ������
	void NodeKernel::CreatePacGlobal(std::vector<int> PacGlobal)
	{
	}
//		��������� ����� �� �������� � ���� ��� � ���� �� ������ ���� ������� ������ � ���� �� �� ������
	void NodeKernel::CreatePacLocal(std::vector<int> PacLocal)
	{
	}
//	+	�������� ������ �� ���������� ����� ��� �������� ������ ���� �����
	void NodeKernel::PushPacMesh(char * filename)
	{
		//PauseThreads(2);								//��� ���� ����� ������ ������ ������� �������, ������ ��� ������, ����� ��������, �� �����.
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
					Threads.MeshIn.at(str.at(0) - 48).push_back(VectorBuf);
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
//		�������� ������ ��� �������� ������ ���� ������
	void NodeKernel::PushPacUSU(char * filename)
	{
	}
//	+	���������� ������� ��� �� ����������� ���� ���� �� �����
	int NodeKernel::HowManySheets()
	{
		if (Global.MapGlobal.size() <= 1)
			return -1;
		return Global.MapGlobal.at(Global.now).size();
	}
//		���������� ������ ��������� � ����������
	void NodeKernel::LocalEvent(std::vector<int> Package)
	{
		//���� ��� � OnePak
	}
//	+	�������� ������� �����, �� ������ �����
	int NodeKernel::ShowWays(int from, int where)
	{
		Global.mapResult.clear();
		Global.visitedNodes.clear();
		Calc(from, where);
		return System::Convert::ToInt64(Global.mapResult.size());
	}
//		������� ����� ����� ��� ���� ������
	int NodeKernel::CreateMeshThread()
	{
		//������� ���� ���������� ����� ������ ������� �������
		return 0;
	}
//		��������� ������ �� �����
	bool NodeKernel::PauseThreads(int mode)
	{
		//������� ���� ��������� �� ���� ��������� ��������� ������ �� �����������, ����� ���� ��������� �� ����� ������ � ���������� ��������.
		return 0;
	}
//		���������� ���������� �������
	bool NodeKernel::ContinueThreads(int mode)
	{
		return 0;
	}
//		��������� ������� ��� ����������� ���������� �������
	int NodeKernel::SystemState(int Parametr)
	{
		return 0;
	}
//	+	�� ����� ���� ���� �������� �����
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
//		������������ ����� �� ��������
	void NodeKernel::Events(std::vector<int> OnePac)
	{
		//�� ���� �������� �����, ������� ������������ (�� ������ � ��������, � ����������� ����������� ���� ���������� ����� � ������� �� ������ ����)
		//������������ ������ ������� � Process
		switch (OnePac.at(4))
		{
		case 1:
		{
			//�������� �� Mesh, �� ���� ���� �������� �������, ���� ����������� �����������
			if (OnePac.at(5) == 0)
			{
				//��� ����������� �����������. ���� ������ ���������� � ��� ������� ��������� GenHandler. 
			}
			else
			{
				//��� ������. �� ���� ������ � ��������������� Template �������.
				
			}
			break; 
		};
		case 2:
		{
			//�������� �� ������, ����� ����������� ������ �� ������ ����
			break; 
		};
		default:
			break;
		}

		Global.Process.push_back(Global.PacEvent);
	}
//		������� ��� �������� ������ ������ �� ���� �������� �� �����
	void NodeKernel::CreateNewEvent(std::string NewEvent)
	{
	}
//	+	������� ��������� ������� � ���������	
	bool NodeKernel::CompareEvents(EventSystem Buffer)
	{
		if (Buffer.Standart.Address == Buffer.Template.Address)											//���������� ������ ������� ������� � �������. ���� ����� �� ��������� ���������/���������� �� ���������
			for (int i = 0; i < Buffer.Standart.Address.size(); i++)
				if (Buffer.Template.Data.at(i).DataType == Buffer.Standart.Event.at(i).DataType)	//�������� ���� ������
					if (Buffer.Standart.Event.at(i).Condition == true)											//���� ��������� � ��������
						if (Buffer.Template.Data.at(i).Value >= Buffer.Standart.Event.at(i).DataRange[0] && Buffer.Template.Data.at(i).Value <= Buffer.Standart.Event.at(i).DataRange[1])
							continue;
						else
							return false;
					else 
						if (Buffer.Template.Data.at(i).Value <= Buffer.Standart.Event.at(i).DataRange[0] || Buffer.Template.Data.at(i).Value >= Buffer.Standart.Event.at(i).DataRange[1])
							continue;
						else 
							return false;
				else
					return false;
		else
			return false;
		return true;
	}
//		������� ����������� ��������������� �����	
	void NodeKernel::ExecuteEvent(EventSystem Buffer)
	{
	}
//		���������� ������ ��������� � ������� �����	
	void NodeKernel::GlobalEvent(std::vector<int> Package)
	{
		//�������� �����, ���� ���������� � �������� ������� � Process, ����� ������� �� ��� ������.
		//�������� �� Mesh, �� ���� ���� �������� �������, ���� ����������� �����������
		if (Package.at(5) == 0)
		{
			//��� ����������� �����������. ���� ������ ���������� � ��� ������� ��������� GenHandler. 
			Package.at(0) = Global.now;
			Package.at(1) = Global.now;
			Package.at(2) = Global.now;
			Package.at(4) = 3;
			Global.Process.insert(Global.Process.begin(), Package);
		}
		else
		{
			//��� ������. �� ���� ������ � ��������������� Template �������.
			for (int i = 0; i < Global.Events.size(); i++)
			{
				//���� ������ ������� �� ������ ����
				for (int j = 0; j < Global.Events.at(i).Standart.Address.size(); j++)
				{
					//������ �� ������� � ������
					if (Global.Events.at(i).Standart.Address.at(j)[0] == Package.at(2))
					{
						//���� ������ ���� �����, �� ��������� ���
						if (Global.Events.at(i).Standart.Address.at(j)[1] == Package.at(3))
						{
							//����� ������ ������, ������ ���� ��������� ��������. ����� ���� ���� � ��� ��������� ��� ������
							if (Global.Events.at(i).Template.Data.at(j).DataType == Package.at(5))
							{
								Global.Events.at(i).Template.Data.at(j).Value = Package.at(6);
								//����� ���� ��� ��������� ������, ���������� � ��������. ���� ��, �� ��������� �������
								if (CompareEvents(Global.Events.at(i)))
								{
									ExecuteEvent(Global.Events.at(i));
								}
							}
						}
					}
				}
			}
		}
	}
//	+	����������� ������
	NodeKernel::NodeKernel(char* Localtxt, char* Globaltxt, int dream, int now)			//����������� ������
	{
		Global.dream = dream;
		Global.now = now;
		Global.hProcess = OpenMutex(MUTEX_ALL_ACCESS, false, "Process");
		MapLocalInit(Localtxt);
		MapGlobalInit(Globaltxt);
	}
//		���������� ������
	NodeKernel::~NodeKernel()
	{
	}
//		������� ����������
	DWORD WINAPI NodeKernel::GenHandler(LPVOID t)
	{
		NodeKernel* _this = (NodeKernel*)t;
		if (!_this) 
			return;
		
		for (;;)
		{
			Sleep(_this->Global.dream); //����� �������� �����������
			WaitForSingleObject(_this->Global.hProcess, INFINITE);	//��������� ����� � ��� ����� Proecss ����������� � ����� ������ ���.
			if (_this->Global.Process.size() > 0)					//���� � ��� ���-�� ����, �� ������, �������� ������.
			{
				if (_this->Global.Process.at(_this->Global.Process.size()).at(0) == _this->Global.now || _this->Global.Process.at(_this->Global.Process.size() - 1).at(1) == _this->Global.now)
				{
					if (_this->Global.Process.at(_this->Global.Process.size()).at(0) == _this->Global.now) //���� �� ��� �������� ����
					{
						switch (_this->Global.Process.at(_this->Global.Process.size()).at(4))
						{
							case 1:
							{
								//������ �� ����
								_this->GlobalEvent(_this->Global.Process.at(_this->Global.Process.size));
								_this->Global.Process.pop_back();
								break;
							}
							case 2:
							{
								_this->LocalEvent(_this->Global.Process.at(_this->Global.Process.size));
								//������ � ���
								break;
							}
							case 3:
							{
								//������ � ������� ���� �� ��� � ��� ��� ������... ���� ����, �� ���� ��������� ����������-�����������, �����
								break;
							}
							default:
							{
								//��������, ���� ���-�� ���������� ���� �� ���.
								break;
							}
						}
					}
					else                        //���� ������ ������������ �����
					{
						int a =	_this->ShowSheet(_this->Global.now,_this->Global.Process.at(_this->Global.Process.size()).at(0));
						WaitForSingleObject(_this->Threads.hMeshOut.at(a), INFINITE);
						std::vector<int> pufpuf;
						pufpuf = _this->Global.Process.at(_this->Global.Process.size());
						pufpuf.at(0)=a;
						_this->Threads.MeshOut.at(a).insert(_this->Threads.MeshOut.at(a).begin(), pufpuf);
						ReleaseMutex(_this->Threads.hMeshOut.at(a));
					}
				}
				else			//���� ���������� �� ���, �� ���������� ���� ����. � ����� ������ ���� �� � ��������� Pass
				{
					WaitForSingleObject(_this->Global.hPass, INFINITE);
					_this->Global.Pass.push_back(_this->Global.Process.at(_this->Global.Process.size() - 1));
					ReleaseMutex(_this->Global.hPass);
					_this->Global.Process.pop_back();
				}
			}
			ReleaseMutex(_this->Global.hProcess);
		}
		return 0;
	}
//		����� ��� ����������
	DWORD WINAPI NodeKernel::UsuThread(LPVOID t)
	{
		return 0;
	}
//		����� ��� ��� ������
	DWORD WINAPI NodeKernel::MeshThread(LPVOID t)
	{
		NodeKernel* _this = (NodeKernel*)t;
		if (!_this) return;
		//��� ������������� ������, �� ���������� ����� ��������� �� ����� ������������
		HANDLE  second[2];
		int Index = _this->Threads.Threads.size() + 1;								//�������� ������ ������, �� ���� ��� ���������� ����� �� ��-��
		second[0] = _this->Threads.hMeshIn.at(Index);
		std::vector<int> VectorBuf;
		for (;;)
		{
			WaitForSingleObject(_this->Threads.hMeshIn.at(Index), INFINITE);
			if (_this->Threads.MeshIn.at(Index).size() > 0 && _this->Threads.PushMeshPac.at(Index) > 0)
			{
				WaitForSingleObject(_this->Global.hProcess, INFINITE);
				_this->Global.Process.push_back(_this->Threads.MeshIn.at(Index).at(_this->Threads.MeshIn.at(Index).size));
				ReleaseMutex(_this->Global.hProcess);
				_this->Threads.MeshIn.at(Index).pop_back();
			}
			//WaitForSingleObject(_this->Threads.hMeshIn.at(Index), INFINITE);		//����������� Mesh
			//if (_this->Threads.MeshIn.at(Index).size > 0 && _this->Threads.PushMeshPac.at(Index) > 0)						//���� �� ������ ��������� �� ���?
			//{
			//	if (System::Convert::ToInt16(_this->Threads.MeshIn.at(Index).at(_this->Threads.MeshIn.at(Index).size - 1).at(0)) == 228)
			//	{
			//		/*System::MessageBox::Show(
			//			Convert::ToString("��� ������ �� ���� 2 �����������. �����: " + Convert::ToString(send2)),
			//			Convert::ToString("��� ���"),
			//			MessageBoxButtons::OK,
			//			MessageBoxIcon::Information);*/
			//		Sleep(1);
			//	}
			//	else												//���� ���� ������, �� �� ������ ��������. ���������� � ��������� ��� GenHandler
			//	{
			//		WaitForSingleObject(_this->Global.hProcess, INFINITE);
			//		_this->Global.Process.push_back(_this->Threads.MeshIn.at(Index).at(_this->Threads.MeshIn.at(Index).size() - 1));
			//		ReleaseMutex(_this->Global.hProcess);
			//		_this->Threads.MeshIn.at(Index).pop_back();
			//	}
			//}
			//ReleaseMutex(_this->Threads.hMeshIn.at(Index));
			//Sleep(_this->Global.dream);
			//WaitForMultipleObjects(2, second, TRUE, INFINITE);	//����������� Mesh1 � MeshProc1
			//if (_this->Threads.MeshOut.at(Index).size()>0)								//���� �� ������ � ������� �� ��������
			//{
			//	VectorBuf = _this->Threads.MeshOut.at(Index)[_this->Threads.MeshOut.at(Index).size() - 1];
			//	VectorBuf.insert(VectorBuf.begin(), 228);
			//	_this->Threads.MeshIn.at(Index).insert(_this->Threads.MeshIn.at().begin(), VectorBuf);
			//	_this->Threads.MeshProcess1.pop_back();
			//}
			//ReleaseMutex(second);
			//Sleep(_this->Global.dream);
		}
		return 0;
	}
