#pragma once
#include <map>
#include <vector>
#include <windows.h>
#include <iostream>
#include <algorithm>





class NodeKernel
{
	private:
		/*шаблонные события*/
		struct EventSystem
		{
			std::vector<int[2]> Address;	//Адреса, с которых требуются данные для исполнения сценария (Узел и УСУ)		
											//vector<> - массив, где каждый элемент соответствует адресу с которого ожидаются данные для исполнения сценария
											//multimap<> - ключ это тип данных "Влажность воздуха", "Температура почвы" итд
											//multimap<> - ключ bool определяет логику вхождения в интервал, double определяет интервал
			std::vector<std::multimap<std::string, std::multimap <bool, double[2]>>> Values;
			//Что требуется сделать при истине сценария
			std::vector<std::map<int[2], std::vector<int[2]>>> Action; //
																	   //
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
			std::vector< EventSystem > Events;										//Массив структур Event
			std::vector<int> PacEvent;												//Буфер для функции Event
		} Global;
		/*Это все что связанно с сетью между узлами*/
		struct ThreadSystem
		{
			std::vector<HANDLE> Threads;											//Массив для хранения хэндлов потоков шин данных.
			std::vector<std::vector<std::vector<int>>> MeshOut;						//Массив структур для дальнейшей отправки пакетов по шинам.
			std::vector<HANDLE> hMeshOut;											//Массив мьютексов для доступа в структуру MeshOut
			std::vector<std::vector<std::vector<int>>> MeshIn;						//Структура для эмуляции входных пакетов на шины данных
			std::vector<HANDLE> hMeshIn;											//Массив мьютексов для доступа в структуру MeshIn
		} Thread;
		/*Это все что связанно с сетью УСУ данного узла*/
		struct USUSystem
		{
			std::vector<std::vector<int>> USUOut;									//Структура для отправки пакетов в сеть УСУ
			HANDLE hUSUOut;															//Мьютекс для этой структуры
			std::vector<std::vector<int>> USUIn;									//Структура входных данных с сети УСУ
			HANDLE hUSUIn;															//Мьютекс для этой структуры
			HANDLE hUSUThread;														//Хэндл потока работающего с сетью УСУ
		} Usu;
		/*Приватные функции в этом классе*/
		void Calc(int startNode, int endNode);									//Рекурсия для просчета маршрутов
		void MapGlobalInit(char* fileName);										//Инициализация глобальной карты
		void MapLocalInit(char* fileName);										//Инициализация локальной карты
		void CreatePacGlobal(std::vector<int> PacGlobal);						//Формирует пакет на отправку в сеть узлов и шлет на нужную шину
		void CreatePacLocal(std::vector<int> PacLocal);							//Формирует пакет на отправку в УСУ и шлет на нужную шину
		int HowManySheets();													//Возвращает кол-во шин на эмулируемом узле (сколько надо создать потоков итд)
		int ShowSheet(int from, int where);										//Возвращает на какую шину данных сейчас надо отправить пакет
		void Events(std::vector<int> OnePac);									//обрабатывает пакет в соответствии с событиями
	public:
		/*Публичные функции этого класса*/
		void PushPacMesh(char* filename);										//Запушить из текстового файла в структуры для шин
		void PushPacUSU(char* filename);										//Запушить из текстового файла в структуры для шины УСУ (значения датчиков итд)
		int ShowWays(int from, int where);										//Возвращает кол-во путей от одного узла до другого, хз зачем это надо но пусть будет.
		int CreateMeshThread();													//Возвращает по какому i найти HANDLE потока в массиве хэндлов Threads
		bool PauseThreads(int mode);											//1 - все, 2 - только шины. Возвращает 1 если все ровно, 0 если уже на паузе
		bool ContinueThreads(int mode);											//1 - все, 2 - только шины. Возвращает 1 если все ровно, 0 если уже работают
		NodeKernel(char* Local, char* Global, int dream, int now);					//Конструктор класса. На вход имена двух текстовых файлов, интервал сна, номер узла
		~NodeKernel();
};

