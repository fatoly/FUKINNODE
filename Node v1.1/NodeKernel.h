#pragma once
#include "Structures.h"
#include <algorithm>

class NodeKernel
{
	private:
		GlobalSystem Global;													//���������� ��������� ��� �������� ������
		ThreadSystem Threads;													//��������� ��� �������
		UsuSystem Usu;															//��������� ��� ���������� ���
		/*��������� ������� � ���� ������*/
		void	Calc(int startNode, int endNode);								//�������� ��� �������� ���������
		void	MapGlobalInit(char* fileName);									//������������� ���������� �����
		void	MapLocalInit(char* fileName);									//������������� ��������� �����
		void	CreatePacGlobal(std::vector<int> PacGlobal);					//��������� ����� �� �������� � ���� ����� � ���� �� ������ ����
		void	CreatePacLocal(std::vector<int> PacLocal);						//��������� ����� �� �������� � ��� � ���� �� ������ ����
		int		HowManySheets();												//���������� ���-�� ��� �� ����������� ���� (������� ���� ������� ������� ���)
		int		ShowSheet(int from, int where);									//���������� �� ����� ���� ������ ������ ���� ��������� �����
		void	Events(std::vector<int> OnePac);								//������������ ����� � ������������ � ���������
		void	CreateNewEvent(std::string NewEvent);							//������� ����� �������
		bool	CompareEvents(EventSystem Buffer);											//�������� ��� �������
		void	ExecuteEvent(int key);											//��������� �������
		void	GlobalEvent(std::vector<int> Package);							//���������� ������� �� �������� � ����
		void	LocalEvent(std::vector<int> Package);							//���������� ������� ��������� �� ����������
		int		ShowWays(int from, int where);									//���������� ���-�� ����� �� ������ ���� �� �������, �� ����� ��� ���� �� ����� �����.
		int		CreateMeshThread();												//���������� �� ������ i ����� HANDLE ������ � ������� ������� Threads	
	public:
		/*��������� ������� ����� ������*/
		void	PushPacMesh(char* filename);									//�������� �� ���������� ����� � ��������� ��� ��� (��� ��������)
		void	PushPacUSU(char* filename);										//�������� �� ���������� ����� � ��������� ��� ���� ��� (�������� �������� ���) (��� ��������)
		bool	PauseThreads(int mode);											//1 - ���, 2 - ������ ����. ���������� 1 ���� ��� �����, 0 ���� ��� �� �����
		bool	ContinueThreads(int mode);										//1 - ���, 2 - ������ ����. ���������� 1 ���� ��� �����, 0 ���� ��� ��������
		int		SystemState(int Parametr);										//���������� ��������� ������� (������� ������� �������, ������� ������� � ������� ���)
		
		NodeKernel(char* Local, char* Global, int dream, int now);				//����������� ������. �� ���� ����� ���� ��������� ������, �������� ���, ����� ����
		~NodeKernel();
		//������
		DWORD WINAPI GenHandler(LPVOID t);
		DWORD WINAPI UsuThread(LPVOID t);
		DWORD WINAPI MeshThread(LPVOID t);
};

