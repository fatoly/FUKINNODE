#pragma once
#include "Structures.h"
#include <algorithm>

class NodeKernel
{
	private:
		GlobalSystem Global;													//Глобальная структура для главного потока
		ThreadSystem Threads;													//Структура для потоков
		UsuSystem Usu;															//Структура для подсистемы УСУ
		/*Приватные функции в этом классе*/
		void	Calc(int startNode, int endNode);								//Рекурсия для просчета маршрутов
		void	MapGlobalInit(char* fileName);									//Инициализация глобальной карты
		void	MapLocalInit(char* fileName);									//Инициализация локальной карты
		void	CreatePacGlobal(std::vector<int> PacGlobal);					//Формирует пакет на отправку в сеть узлов и шлет на нужную шину
		void	CreatePacLocal(std::vector<int> PacLocal);						//Формирует пакет на отправку в УСУ и шлет на нужную шину
		int		HowManySheets();												//Возвращает кол-во шин на эмулируемом узле (сколько надо создать потоков итд)
		int		ShowSheet(int from, int where);									//Возвращает на какую шину данных сейчас надо отправить пакет
		void	Events(std::vector<int> OnePac);								//обрабатывает пакет в соответствии с событиями
		void	CreateNewEvent(std::string NewEvent);							//Создать новое событие
		bool	CompareEvents(int key);											//Сравнить два события
		void	ExecuteEvent(int key);											//Выполнить событие
		void	GlobalEvent(std::vector<int> Package);							//Обработчик пакетов на отправку в сеть
		void	LocalEvent(std::vector<int> Package);							//Обработчик пакетов пришедших из подсистемы
		int		ShowWays(int from, int where);									//Возвращает кол-во путей от одного узла до другого, хз зачем это надо но пусть будет.
		int		CreateMeshThread();												//Возвращает по какому i найти HANDLE потока в массиве хэндлов Threads	
	public:
		/*Публичные функции этого класса*/
		void	PushPacMesh(char* filename);									//Запушить из текстового файла в структуры для шин (для эмуляции)
		void	PushPacUSU(char* filename);										//Запушить из текстового файла в структуры для шины УСУ (значения датчиков итд) (для эмуляции)
		bool	PauseThreads(int mode);											//1 - все, 2 - только шины. Возвращает 1 если все ровно, 0 если уже на паузе
		bool	ContinueThreads(int mode);										//1 - все, 2 - только шины. Возвращает 1 если все ровно, 0 если уже работают
		int		SystemState(int Parametr);										//Возвращает состояние системы (сколько потоков открыто, сколько пакетов в буферах итд)
		
		NodeKernel(char* Local, char* Global, int dream, int now);				//Конструктор класса. На вход имена двух текстовых файлов, интервал сна, номер узла
		~NodeKernel();
		//Потоки
private:
		static DWORD WINAPI GenHandler(LPVOID t);

		//RWorker* _this = (RWorker*)ptr;
		//if (!_this) return 1; //забыли нам дать объект, не будем работать
		//for (;;)
		//{
		//	//сделаем важную работу
		//	_this->setData(_this->getData() + 1);
		//	//поспим, глядя на флаг конца работы
		//	if (WaitForSingleObject(_this->hStopEvt, 1000) != WAIT_TIMEOUT)
		//		break;
		//}
		//return 0;

		static DWORD WINAPI UsuThread(LPVOID t);
		static DWORD WINAPI MeshThread(LPVOID t);
};

