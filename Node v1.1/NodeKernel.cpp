#include "stdafx.h"
#include "NodeKernel.h"



//	+	Рекурсивная функция поиска путей
	void NodeKernel::Calc(int startNode, int endNode)
	{
		Global.visitedNodes.push_back(startNode);
		std::map<int, std::vector<int>> sheets = Global.MapGlobal[startNode];
		for (int sheetN = 0; sheetN < sheets.size(); sheetN++)				//Идем по шинам на узле
		{
			std::vector<int> nodes = sheets[sheetN];
			for (int nodeN = 0; nodeN < nodes.size(); nodeN++)				//Идем по узлам на шине
			{
				if (nodes[nodeN] == endNode)								//Если нашли конечную точку, то записываем в результирующую структуру
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
//	+	инициализация глобальной карты из текстового файла
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
			if (fgets(buf, 256, file) == NULL)		// Если файл закончился
				break;
			LevelBuf = 0;
			NUM = 0;
			std::string str = buf;
			for (int i = 0; i < str.length() - 1; i++)		//смотрим какое у нас число и уровень инкапсуляции
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
//	+	инициализация локальной карты из текстового файла
	void NodeKernel::MapLocalInit(char * fileName)
	{
		std::map<int, int> Buffer;
		FILE*	file = fopen(fileName, "rt");
		char buf[256] = { 0 };
		int flag = 0;
		int LevelBuf = 0, NUM = 0, level1 = 0, level2 = 0;
		Buffer.clear();
		for (;;)											//Наполнение контейнера для подсистем
		{
			LevelBuf = 0;
			NUM = 0;
			if (fgets(buf, 256, file) == NULL)		// Если файл не открылся
				break;
			std::string str = buf;


			for (int i = 0; i < str.length() - 1; i++)		//смотрим какое у нас число и уровень инкапсуляции
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
//		Формирует пакет на отправку в сеть узлов и шлет на нужную шину помоему лишняя и надо бы ее убрать
	void NodeKernel::CreatePacGlobal(std::vector<int> PacGlobal)
	{
	}
//		Формирует пакет на отправку в сеть УСУ и шлет на нужную шину помоему лишняя и надо бы ее убрать
	void NodeKernel::CreatePacLocal(std::vector<int> PacLocal)
	{
	}
//	+	Запушить пакеты из текстового файла для эмуляции работы сети узлов
	void NodeKernel::PushPacMesh(char * filename)
	{
		//PauseThreads(2);								//Для того чтобы небыло вообще никаких проблем, ставим все потоки, кроме главного, на паузу.
		FILE*	file = fopen(filename, "rt");
		char buf[256] = { 0 };
		std::string str;
		std::vector<int> VectorBuf;						//Для того чтобы пушить в структуру вектор<вектор>
		int choise;
		for (int Where = 0;; Where++)					//цикл по строчкам в файле
		{
			if (fgets(buf, 256, file) == NULL)			//Если строку не получили значит файл кончился, ну и заодно считали строку
				break;
			str = buf;									//захреначили в тип данных который умеет возвращать свою длинну тк считать все символы слишком лениво
			if (str.at(1) != 1)							//????проверка на дурака, в работе ли сейчас структура. Отлов пробем с мьютексами, изначально заданным текстовым файлом или с отсутсвием затирания структуры.
			{
				for (int z = 1; z < str.length(); z++)	//Пробегаемся по строке
				{
					if (System::Convert::ToChar(str.at(z)) <= '9' && System::Convert::ToChar(str.at(z)) >= '0')			//Тут нас интересуют только цифры
					{
						int Buf = 0;
						for (; System::Convert::ToChar(str.at(z)) <= '9' && System::Convert::ToChar(str.at(z)) >= '0';)	//Из строки в число
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
							Sleep(1); //Нашли другой символ в строке, ошибка тут происходит надо придумать как обрабатывать такую ошибку
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
					Sleep(1); //Такой шины нету, соотв пушить некуда.
					break;
				}
				}
				VectorBuf.clear();
			}
			else                                        //пакет в работе. Нахрена такая шляпа нужна была - хз.
			{
				Sleep(1);
			}
		}
		fclose(file);
	}
//		Запушить пакеты для эмуляции работы сети УСУшек
	void NodeKernel::PushPacUSU(char * filename)
	{
	}
//	+	Возвращает сколько шин на эмулируемом узле тоже хз зачем
	int NodeKernel::HowManySheets()
	{
		if (Global.MapGlobal.size() <= 1)
			return -1;
		return Global.MapGlobal.at(Global.now).size();
	}
//		Агрегирует данные пришедшие с подсистемы
	void NodeKernel::LocalEvent(std::vector<int> Package)
	{
		//срет ими в OnePak
	}
//	+	Показать сколько путей, хз правда зачем
	int NodeKernel::ShowWays(int from, int where)
	{
		Global.mapResult.clear();
		Global.visitedNodes.clear();
		Calc(from, where);
		return System::Convert::ToInt64(Global.mapResult.size());
	}
//		Создать новый поток для шины данных
	int NodeKernel::CreateMeshThread()
	{
		//вернуть надо порядковый номер потока который создали
		return 0;
	}
//		Поставить потоки на паузу
	bool NodeKernel::PauseThreads(int mode)
	{
		//Сначала надо подождать по всем мьютексам окончания работы со структурами, после чего поставить на паузу потоки и освободить мьютексы.
		return 0;
	}
//		Продолжить выполнение потоков
	bool NodeKernel::ContinueThreads(int mode)
	{
		return 0;
	}
//		Публичная функция для отображения параметров системы
	int NodeKernel::SystemState(int Parametr)
	{
		return 0;
	}
//	+	На какую шину надо посылать пакет
	int NodeKernel::ShowSheet(int from, int where)
	{
		Global.mapResult.clear();
		Global.visitedNodes.clear();
		Calc(from, where);
		for (int i = 0; i < 999999; i++)																//Идем по длинам маршрутов
			for (int j = 0; j < Global.mapResult.size(); j++)											//Идем по результирующей мапе и чекаем какой у нас минимально возможный путь
				if (Global.mapResult.at(j).size() == i)													//Если нашли минимально возможный путь, то надо найти от какой шины он исходит
					for (int y = 1; y < Global.MapGlobal.at(from).size() + 1; y++)						//Идем по шинам на эмулируемом узле
						for (int z = 1; z < Global.MapGlobal.at(from).at(y).size() + 1; z++)			//Идем по узлам на шине данных
							if (Global.MapGlobal.at(from).at(y).at(z) == Global.mapResult.at(j).at(0))	//Если нашли нужный узел на шине, то возвращаем на какой шине нашли.
								return y;																//возвращаем номер шины
	}
//		Обрабатываем пакет по событиям
	void NodeKernel::Events(std::vector<int> OnePac)
	{
		//На вход получаем пакет, который обрабатываем (Из данных с датчиков, в управляющее воздействие либо отправляем пакет с данными на другой узел)
		//Обработанный пихаем обратно в Process
		switch (OnePac.at(4))
		{
		case 1:
		{
			//Получили из Mesh, то бишь либо значения дачиков, либо управляющее воздействие
			if (OnePac.at(5) == 0)
			{
				//Это управляющее воздействие. Надо просто обработать в вид который принимает GenHandler. 
			}
			else
			{
				//Это данные. Их надо внести в соответствующий Template события.
				
			}
			break; 
		};
		case 2:
		{
			//Получили от УСУшек, соотв переправить данные на нужный узел
			break; 
		};
		default:
			break;
		}

		Global.Process.push_back(Global.PacEvent);
	}
//		Функция для создания нового ивента то есть запушить из файла
	void NodeKernel::CreateNewEvent(std::string NewEvent)
	{
	}
//	+	Функция сравнения ивентов с эталонным	
	bool NodeKernel::CompareEvents(EventSystem Buffer)
	{
		if (Buffer.Standart.Address == Buffer.Template.Address)											//сравниваем массив адресов шаблона и эталона. Если равны то проверяем вхождение/выхождение из диапазона
			for (int i = 0; i < Buffer.Standart.Address.size(); i++)
				if (Buffer.Template.Data.at(i).DataType == Buffer.Standart.Event.at(i).DataType)	//проверка типа данных
					if (Buffer.Standart.Event.at(i).Condition == true)											//Если вхождение в диапазон
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
//		Функция выполняющая соответствующий ивент	
	void NodeKernel::ExecuteEvent(EventSystem Buffer)
	{
	}
//		Агрегирует данные пришетшие с системы узлов	
	void NodeKernel::GlobalEvent(std::vector<int> Package)
	{
		//получили пакет, надо обработать и запихать обратно в Process, благо мьютекс мы уже заняли.
		//Получили из Mesh, то бишь либо значения дачиков, либо управляющее воздействие
		if (Package.at(5) == 0)
		{
			//Это управляющее воздействие. Надо просто обработать в вид который принимает GenHandler. 
			Package.at(0) = Global.now;
			Package.at(1) = Global.now;
			Package.at(2) = Global.now;
			Package.at(4) = 3;
			Global.Process.insert(Global.Process.begin(), Package);
		}
		else
		{
			//Это данные. Их надо внести в соответствующий Template события.
			for (int i = 0; i < Global.Events.size(); i++)
			{
				//Ищем нужное событие по адресу узла
				for (int j = 0; j < Global.Events.at(i).Standart.Address.size(); j++)
				{
					//Шагаем по адресам в ивенте
					if (Global.Events.at(i).Standart.Address.at(j)[0] == Package.at(2))
					{
						//Если нужный узел нашли, то проверяем УСУ
						if (Global.Events.at(i).Standart.Address.at(j)[1] == Package.at(3))
						{
							//нашли нужный шаблон, значит надо заполнить темплейт. Ясное дело если у нас совпадает тип данных
							if (Global.Events.at(i).Template.Data.at(j).DataType == Package.at(5))
							{
								Global.Events.at(i).Template.Data.at(j).Value = Package.at(6);
								//После того как заполнили шаблон, сравниваем с эталоном. Если да, то выполняем событие
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
//	+	Конструктор класса
	NodeKernel::NodeKernel(char* Localtxt, char* Globaltxt, int dream, int now)			//Конструктор класса
	{
		Global.dream = dream;
		Global.now = now;
		Global.hProcess = OpenMutex(MUTEX_ALL_ACCESS, false, "Process");
		MapLocalInit(Localtxt);
		MapGlobalInit(Globaltxt);
	}
//		Деструктор класса
	NodeKernel::~NodeKernel()
	{
	}
//		Главный обработчик
	DWORD WINAPI NodeKernel::GenHandler(LPVOID t)
	{
		NodeKernel* _this = (NodeKernel*)t;
		if (!_this) 
			return;
		
		for (;;)
		{
			Sleep(_this->Global.dream); //чтобы работало помедленнее
			WaitForSingleObject(_this->Global.hProcess, INFINITE);	//подождать когда у нас буфер Proecss освободится а потом занять его.
			if (_this->Global.Process.size() > 0)					//Если в нем что-то есть, то ништяк, работаем пасаны.
			{
				if (_this->Global.Process.at(_this->Global.Process.size()).at(0) == _this->Global.now || _this->Global.Process.at(_this->Global.Process.size() - 1).at(1) == _this->Global.now)
				{
					if (_this->Global.Process.at(_this->Global.Process.size()).at(0) == _this->Global.now) //Если мы это конечный узел
					{
						switch (_this->Global.Process.at(_this->Global.Process.size()).at(4))
						{
							case 1:
							{
								//Пришло из меша
								_this->GlobalEvent(_this->Global.Process.at(_this->Global.Process.size));
								_this->Global.Process.pop_back();
								break;
							}
							case 2:
							{
								_this->LocalEvent(_this->Global.Process.at(_this->Global.Process.size));
								//пришло с УСУ
								break;
							}
							case 3:
							{
								//пришло с ивентов пока хз что и как тут мутить... Хотя знаю, но блэт агрегация собственно-придуманная, ухххх
								break;
							}
							default:
							{
								//Ошибочка, надо как-то обработать пока хз как.
								break;
							}
						}
					}
					else                        //Если просто перевалочный пункт
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
				else			//если назначался не нам, то пропускаем мимо ушей. В нашем случае срем им в структуру Pass
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
//		Поток для подсистемы
	DWORD WINAPI NodeKernel::UsuThread(LPVOID t)
	{
		return 0;
	}
//		Поток для меш систем
	DWORD WINAPI NodeKernel::MeshThread(LPVOID t)
	{
		NodeKernel* _this = (NodeKernel*)t;
		if (!_this) return;
		//При инициализации потока, мы определяем какие структуры он будет использовать
		HANDLE  second[2];
		int Index = _this->Threads.Threads.size() + 1;								//Называем индекс потока, то бишь его порядковый номер на ус-ве
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
			//WaitForSingleObject(_this->Threads.hMeshIn.at(Index), INFINITE);		//Резервируем Mesh
			//if (_this->Threads.MeshIn.at(Index).size > 0 && _this->Threads.PushMeshPac.at(Index) > 0)						//Есть ли пакеты пришедшие из вне?
			//{
			//	if (System::Convert::ToInt16(_this->Threads.MeshIn.at(Index).at(_this->Threads.MeshIn.at(Index).size - 1).at(0)) == 228)
			//	{
			//		/*System::MessageBox::Show(
			//			Convert::ToString("Все пакеты по шине 2 отправленны. Итого: " + Convert::ToString(send2)),
			//			Convert::ToString("Все гуд"),
			//			MessageBoxButtons::OK,
			//			MessageBoxIcon::Information);*/
			//		Sleep(1);
			//	}
			//	else												//Если норм пакеты, то шо делать остается. Записываем в структуру для GenHandler
			//	{
			//		WaitForSingleObject(_this->Global.hProcess, INFINITE);
			//		_this->Global.Process.push_back(_this->Threads.MeshIn.at(Index).at(_this->Threads.MeshIn.at(Index).size() - 1));
			//		ReleaseMutex(_this->Global.hProcess);
			//		_this->Threads.MeshIn.at(Index).pop_back();
			//	}
			//}
			//ReleaseMutex(_this->Threads.hMeshIn.at(Index));
			//Sleep(_this->Global.dream);
			//WaitForMultipleObjects(2, second, TRUE, INFINITE);	//резервируем Mesh1 и MeshProc1
			//if (_this->Threads.MeshOut.at(Index).size()>0)								//Есть ли пакеты в очереди на отправку
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
