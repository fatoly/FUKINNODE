#pragma once
#include <map>
#include <vector>
#include <windows.h>

//Правила
struct Rules
{
	std::vector<int[2]> Address;											//Адреса, с которых требуются данные для исполнения сценария (Узел и УСУ)		
																			//vector<> - массив, где каждый элемент соответствует адресу с которого ожидаются данные для исполнения сценария
	std::vector<std::multimap<std::string, std::multimap <bool, double[2]>>> Values;//Структура для объявления значений для самого сценария. Индексы соответствуют с Adress
};
/*Событие*/
struct EventSystem
{
	std::string Name;														//Имя данного события. Нужно скорее для отладки, чем для полноценного функционирования
	Rules Template;															//Шаблон для заполнения
	Rules Standart;															//Эталон
	std::vector<std::map<int[2], std::vector<int[2]>>> Action;				//Что требуется сделать при истине сценария
};
/*Самая главная структура для узла*/
struct GlobalSystem
{
	std::vector<std::vector<int>> mapResult;								//Структура для маршрутов
	std::vector<int> visitedNodes;											//Структура для функции Calc
	std::map<int, std::map<int, int>> MapLocal;								//Структура для мапы БСУ
	std::map<int, std::map<int, std::vector<int>>> MapGlobal;				//Cтруктура для мапы узлов
	std::vector<std::vector<int>> Process;									//Структура для главного обработчика GenHandler
	HANDLE hProcess;														//Мютекс для главной структуры GenHandler
	std::vector<std::vector<int>> Pass;										//Структура Pass для ослеживания пропущенных пакетов
	HANDLE hPass;															//Мьютекс для структуры Pass
	int dream;																//Интервал сна
	int now;																//эмулируемый узел
	std::vector<EventSystem> Events;										//Массив структур Event
	std::vector<int> PacEvent;												//Буфер для функции Event
};
/*Это все что связанно с сетью между узлами*/
struct ThreadSystem
{
	std::vector<HANDLE> Threads;											//Массив для хранения хэндлов потоков шин данных.
	std::vector<std::vector<std::vector<int>>> MeshOut;						//Массив структур для дальнейшей отправки пакетов по шинам.
	std::vector<HANDLE> hMeshOut;											//Массив мьютексов для доступа в структуру MeshOut
	std::vector<std::vector<std::vector<int>>> MeshIn;						//Структура для эмуляции входных пакетов на шины данных
	std::vector<HANDLE> hMeshIn;											//Массив мьютексов для доступа в структуру MeshIn
};
/*Это все что связанно с сетью УСУ данного узла*/
struct USUSystem
{
	std::vector<std::vector<int>> USUOut;									//Структура для отправки пакетов в сеть УСУ
	HANDLE hUSUOut;															//Мьютекс для этой структуры
	std::vector<std::vector<int>> USUIn;									//Структура входных данных с сети УСУ
	HANDLE hUSUIn;															//Мьютекс для этой структуры
	HANDLE hUSUThread;														//Хэндл потока работающего с сетью УСУ
};