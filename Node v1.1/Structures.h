#pragma once
#include <map>
#include <vector>
#include <windows.h>

//�������
struct DataPair
{
	int DataType;
	int Value;
};
struct EventPair
{
	int DataType;
	int DataRange[2];
	bool Condition;
};
struct TemplateRules
{
	std::vector<int[2]> Address;
	std::vector <DataPair> Data;
};
struct Rules
{
	std::vector<int[2]> Address;											//������, � ������� ��������� ������ ��� ���������� �������� (���� � ���)		
	std::vector<EventPair> Event;											//��������� ��� ���������� �������� ��� ������ ��������. ������� ������������� � Adress
};
/*�������*/
struct EventSystem
{
	std::string Name;														//��� ������� �������. ����� ������ ��� �������, ��� ��� ������������ ����������������
	TemplateRules Template;													//������ ��� ����������
	Rules Standart;															//������
	std::vector																//��������
		<std::map															//�������
		<int[2]																//�����
		,std::vector														//����� ������������ ��������
		<int>>> Action;														//��� ��������� ������� ��� ������ ��������
};
/*����� ������� ��������� ��� ����*/
struct GlobalSystem
{
	int dream;																//�������� ���
	int now;																//����������� ���� (���������� �����)
	std::string name;														//��� ����
	std::vector<std::vector<int>> mapResult;								//��������� ��� ���������
	std::vector<int> visitedNodes;											//��������� ��� ������� Calc
	std::map																//����
		<int																//����
		,std::map															//������
		<int																//����� ���
		, int																//��� ���
		>> MapLocal;														//��������� ��� ���� ���
	std::map<int, std::map<int, std::vector<int>>> MapGlobal;				//C�������� ��� ���� �����
	std::vector<std::vector<int>> Process;									//��������� ��� �������� ����������� GenHandler
	HANDLE hProcess;														//������ ��� ������� ��������� GenHandler
	std::vector<std::vector<int>> Pass;										//��������� Pass ��� ����������� ����������� �������
	HANDLE hPass;															//������� ��� ��������� Pass
	std::vector<EventSystem> Events;										//������ �������� Event
	std::vector<int> PacEvent;												//����� ��� ������� Event
};
/*��� ��� ��� �������� � ����� ����� ������*/
struct ThreadSystem
{
	std::vector<HANDLE> Threads;											//������ ��� �������� ������� ������� ��� ������.
	std::vector																//����
		<std::vector														//������
		<std::vector														//�����
		<int>>> MeshOut;													//������ �������� ��� ���������� �������� ������� �� �����.
	std::vector<HANDLE> hMeshOut;											//������ ��������� ��� ������� � ��������� MeshOut
	std::vector																//����
		<std::vector														//������
		<std::vector														//�����
		<int>>> MeshIn;														//��������� ��� �������� ������� ������� �� ���� ������
	std::vector<HANDLE> hMeshIn;											//������ ��������� ��� ������� � ��������� MeshIn
	std::vector<int> PushMeshPac;											//������� ������ ������� (������� ������� �� ��������� MeshIn)
};
/*��� ��� ��� �������� � ����� ��� ������� ����*/
struct UsuSystem
{
	std::vector<std::vector<int>> UsuOut;									//��������� ��� �������� ������� � ���� ���
	HANDLE hUsuOut;															//������� ��� ���� ���������
	std::vector<std::vector<int>> UsuIn;									//��������� ������� ������ � ���� ���
	HANDLE hUsuIn;															//������� ��� ���� ���������
	HANDLE hUsuThread;														//����� ������ ����������� � ����� ���
};