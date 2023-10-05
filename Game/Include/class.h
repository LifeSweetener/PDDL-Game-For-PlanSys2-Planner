#include <utility>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

/* Модель игрового мира. Она также описана в .pddl-файле с доменом планирования PlanSys2.
 *
 * Количество клеток поля (англ. floor) игрового мира задаётся случайно генератором псевдослучайных чисел.
 * Расположение уборщика (англ. cleaner), свалки (англ. trash) и булыжников, которые уборщик должен убрать в помойку,
 * аналогично определяется случайным образом.
 *
 * Текстуры, размеры клеток, форма человека и изображение помойки взяты либо из Интернета, либо построены из
 * примитивов SFML-библиотеки.
 * 
 * Мир имеет форму таблицы с целыми положительными номерами ячеек (клеток). Таблица состоит из
 * строк (англ. rows) и столбцов (англ. columns), которые и составляют обозначения клеток мира.
*/
class Game {
	public:
		Game();
		const float cellSize = 120.f;														// размер клеток мира
		const float outlineSize = 5.f;													// толщина границы клеток
		void gameDraw(RenderWindow& window);								// зарисовать весь мир в SFML-окне
		
		int getRows() {return rows;} int getCols() {return cols;}
		int getCleanerRow() {return rowCleaner;} int getCleanerCol() {return colCleaner;}
		int getTrashRow() {return trashRow;} int getTrashCol() {return trashCol;}
		int getStonesAmount() {return stonesAmount;}															// вернуть общее кол-во камней в игре (мире)
		pair<int, int>** getStonesPositionPublic() {																	// получить координаты всех клеток с камнями
			pair<int, int> **stonesPositionLocal = new pair<int, int>* [stonesAmount];
			for (int i = 0; i < stonesAmount; ++i)
				stonesPositionLocal[i] = new pair<int, int>(stonesPosition[i]->first, stonesPosition[i]->second);
			
			return stonesPositionLocal;
		}
		
		void moveCleaner(string where);
		void pickStone(string whatStone);
		void setMsg(string msg);
		string getMsg() {return gameMsg->getString();}
	
	private:
		// вспомогательные методы:
		pair<int, int> getFloorSize();
		pair<int, int> getCleanerPosition();
		pair<int, int>** getStonesPosition();
		pair<int, int> getTrashPosition();
		pair<RectangleShape**, Texture*> makeFloor();
		VertexArray* getCleanerBody();
		CircleShape** getStones();
		RectangleShape* getTrash();
		
		int rows, cols;									// размеры мира
		int maxRowOrCol;							// максимально возможное расположение уборщика и мусорки на поле (не вдавайтесь в детали :) )
		int rowCleaner, colCleaner;				// клетка с уборщиком
		int stonesAmount;							// общее кол-во камней
		int trashRow, trashCol;					// клетка со свалкой для камней
		bool change;									// создана ли уже игра и идёт ли её изменение (расположений объектов и т.д.)
		
		VertexArray *cleanerBody;				// фигура уборщика (из пяти вершин - verteces)
		RectangleShape **rects;					// множество клеток игры
		Texture *texture_floor;						// текстуры...
		Texture *texture_stone;
		Texture *trash_texture;
		CircleShape **stones;						// булыжники (камни)
		pair<int, int> **stonesPosition;			// номера клеток с камнями
		RectangleShape *trash;					// свалка (помойка, мусорка)
		
		Text *gameMsg=NULL;
		Font *gameFont=NULL;
};

/* Элементарное действие плана.
 * Структура, содержащая поля массива символов (действия),
 * порядкового номера действия (id) и указателя на следующее действие (next).
*/
struct planElement {
	char action[20];
	int id;
	planElement *next;
};

/* Пользовательский планировщик:
*/
class Planner {
	public:
		Planner();											// конструктор пользовательского планировщика, использующего планировщик "PlanSys2"
		void getPDDLPlan();						// вычислить план и заполнить односвязный список элементами плана
		string getPlan() {return plan;}			// получить необработанный план от планировщика PlanSys2
		bool realizePlan();
		bool getEnd() {return end;}
		
		Game *game;									// игровой мир
		
	private:
		string getPDDLProblem();				// сгенерировать в соответствии с классом Game строку с файлом .pddl проблемы
		
		struct planElement *header=NULL;	// односвязный список элементов плана (элементарных действий)
		struct  planElement *current=NULL;
		int planSize;										// количество элементов плана
		bool end;
		
		string plan;										// необработанный план (row plan)
};