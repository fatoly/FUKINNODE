#pragma once
#include <map>
#include <vector>
#include <windows.h>
#include <iostream>
#include <algorithm>





class NodeKernel
{
	private:
		/*��������� �������*/
		struct EventSystem
		{
			std::vector<int[2]> Address;	//������, � ������� ��������� ������ ��� ���������� �������� (���� � ���)		
											//vector<> - ������, ��� ������ ������� ������������� ������ � �������� ��������� ������ ��� ���������� ��������
											//multimap<> - ���� ��� ��� ������ "��������� �������", "����������� �����" ���
											//multimap<> - ���� bool ���������� ������ ��������� � ��������, double ���������� ��������
			std::vector<std::multimap<std::string, std::multimap <bool, double[2]>>> Values;
			//��� ��������� ������� ��� ������ ��������
			std::vector<std::map<int[2], std::vector<int[2]>>> Action; //
																	   //
		};
		/*����� ������� ��������� ��� ����*/
		struct GlobalSystem
		{
			std::vector<std::vector<int>> mapResult;								//��������� ��� ���������
			std::vector<int> visitedNodes;											//��������� ��� ������� Calc
			std::map<int, std::map<int, int>> MapLocal;								//��������� ��� ���� ���
			std::map<int, std::map<int, std::vector<int>>> MapGlobal;				//C�������� ��� ���� �����
			std::vector<std::vector<int>> Process;									//��������� ��� �������� ����������� GenHandler
			HANDLE hProcess;														//������ ��� ������� ��������� GenHandler
			std::vector<std::vector<int>> Pass;										//��������� Pass ��� ����������� ����������� �������
			HANDLE hPass;															//������� ��� ��������� Pass
			int dream;																//�������� ���
			int now;																//����������� ����
			std::vector< EventSystem > Events;										//������ �������� Event
			std::vector<int> PacEvent;												//����� ��� ������� Event
		} Global;
		/*��� ��� ��� �������� � ����� ����� ������*/
		struct ThreadSystem
		{
			std::vector<HANDLE> Threads;											//������ ��� �������� ������� ������� ��� ������.
			std::vector<std::vector<std::vector<int>>> MeshOut;						//������ �������� ��� ���������� �������� ������� �� �����.
			std::vector<HANDLE> hMeshOut;											//������ ��������� ��� ������� � ��������� MeshOut
			std::vector<std::vector<std::vector<int>>> MeshIn;						//��������� ��� �������� ������� ������� �� ���� ������
			std::vector<HANDLE> hMeshIn;											//������ ��������� ��� ������� � ��������� MeshIn
		} Thread;
		/*��� ��� ��� �������� � ����� ��� ������� ����*/
		struct USUSystem
		{
			std::vector<std::vector<int>> USUOut;									//��������� ��� �������� ������� � ���� ���
			HANDLE hUSUOut;															//������� ��� ���� ���������
			std::vector<std::vector<int>> USUIn;									//��������� ������� ������ � ���� ���
			HANDLE hUSUIn;															//������� ��� ���� ���������
			HANDLE hUSUThread;														//����� ������ ����������� � ����� ���
		} Usu;
		/*��������� ������� � ���� ������*/
		void Calc(int startNode, int endNode);									//�������� ��� �������� ���������
		void MapGlobalInit(char* fileName);										//������������� ���������� �����
		void MapLocalInit(char* fileName);										//������������� ��������� �����
		void CreatePacGlobal(std::vector<int> PacGlobal);						//��������� ����� �� �������� � ���� ����� � ���� �� ������ ����
		void CreatePacLocal(std::vector<int> PacLocal);							//��������� ����� �� �������� � ��� � ���� �� ������ ����
		int HowManySheets();													//���������� ���-�� ��� �� ����������� ���� (������� ���� ������� ������� ���)
		int ShowSheet(int from, int where);										//���������� �� ����� ���� ������ ������ ���� ��������� �����
		void Events(std::vector<int> OnePac);									//������������ ����� � ������������ � ���������
	public:
		/*��������� ������� ����� ������*/
		void PushPacMesh(char* filename);										//�������� �� ���������� ����� � ��������� ��� ���
		void PushPacUSU(char* filename);										//�������� �� ���������� ����� � ��������� ��� ���� ��� (�������� �������� ���)
		int ShowWays(int from, int where);										//���������� ���-�� ����� �� ������ ���� �� �������, �� ����� ��� ���� �� ����� �����.
		int CreateMeshThread();													//���������� �� ������ i ����� HANDLE ������ � ������� ������� Threads
		bool PauseThreads(int mode);											//1 - ���, 2 - ������ ����. ���������� 1 ���� ��� �����, 0 ���� ��� �� �����
		bool ContinueThreads(int mode);											//1 - ���, 2 - ������ ����. ���������� 1 ���� ��� �����, 0 ���� ��� ��������
		NodeKernel(char* Local, char* Global, int dream, int now);					//����������� ������. �� ���� ����� ���� ��������� ������, �������� ���, ����� ����
		~NodeKernel();
};

