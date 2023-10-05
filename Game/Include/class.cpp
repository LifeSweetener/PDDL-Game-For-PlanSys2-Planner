#include <utility>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "class.h"
using namespace sf;
using namespace std;

/* Вычислить размер игрового поля.
 * Return: два случайных целых числа в Паре.
*/
pair<int, int> Game::getFloorSize() {
    random_device rd;										// объект для генерации сида
    mt19937 gen(rd());										// генератор псевдослучайных чисел "Вихрь Мерсенна", инициализированный rd()
    uniform_int_distribution<> distrib(4, 8);		// дискретное равномерное распределение
	int rows = distrib(gen);
	int cols = distrib(gen);
	
	return pair<int, int>(rows, cols);
}

/* Вычислить позицию уборщика.
 * Return: позиция случайной клетки поля.
*/
pair<int, int> Game::getCleanerPosition() {
	int max = maxRowOrCol;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, max);
	int row = distrib(gen);
	int col = distrib(gen);
	
	return pair<int, int>(row, col);
}

/* Сгенерировать позиции камней на клетках игры.
 * Return: Пара номера (координат) клетки поля.
*/
pair<int, int>** Game::getStonesPosition() {
	int max = maxRowOrCol;
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, max);
	stonesAmount = distrib(gen);
	
	pair<int, int> **stonesPosition = new pair<int, int> *[stonesAmount];
	for (int i = 0; i < stonesAmount; ++i) {
		int row_ = distrib(gen);
		int col_ = distrib(gen);
		bool thereAreOthers = true;
		while (((row_ == rowCleaner)&&(col_ == colCleaner))||((row_ == trashRow)&&(col_ == trashCol))||(thereAreOthers)) {
			thereAreOthers = false;
			row_ = distrib(gen);
			col_ = distrib(gen);
			for (int j = 0; j < i; ++j)
				if ((row_ == stonesPosition[j]->first)&&(col_ == stonesPosition[j]->second)) thereAreOthers = true;
		}
		
		stonesPosition[i] = new pair<int, int>(row_, col_);
	}
	
	return stonesPosition;
}

/* Создать позицию склада с отбросами (помойки).
 * Return: Пара координат.
*/
pair<int, int> Game::getTrashPosition() {
	int max = maxRowOrCol;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, max);
	int row = distrib(gen);
	int col = distrib(gen);
	while ((row == rowCleaner)&&(col == colCleaner)) {
			row = distrib(gen);
			col = distrib(gen);
	}
	
	return pair<int, int>(row, col);
}

/* Сгенерировать квадратные клетки игрового мира:
 * rows - количество строк, cols - количество столбцов
 * (в сравнении с двумерной таблицей).
 *
 * Return: Пара из двумерного массива квадратов и их текстуры.
*/
pair<RectangleShape**, Texture*> Game::makeFloor() {
	Texture *texture_floor = new Texture();				// единственный объект текстуры для всех клеток поля игры (так эффективнее)
	if (!texture_floor->loadFromFile("Textures/texture-whilte-floor.png")) return pair<RectangleShape**, Texture*>();	// ошибка в импорте текстуры
	
	RectangleShape **rects = new RectangleShape *[rows];
	for (int i = 0; i < rows; ++i)
		rects[i] = new RectangleShape[cols];
	
	int pos_col = 0;
	for (int j = 0; j < cols; ++j) {
		int pos_row = 0;
		for (int i = 0; i < rows; ++i) {
			rects[i][j] = RectangleShape();
			rects[i][j].setSize(Vector2f(cellSize, cellSize));
			rects[i][j].setTexture(texture_floor);
			rects[i][j].setPosition(pos_col, pos_row);
			rects[i][j].setOutlineThickness(outlineSize);
			rects[i][j].setOutlineColor(Color(200, 200, 200));
			pos_row += cellSize + outlineSize;
		}
		pos_col += cellSize + outlineSize;
	}
	
	return pair<RectangleShape**, Texture*>(rects, texture_floor);
}

/* Сгенерировать самого уборщика.
 * Return: SFML-массив вершин (VertexArray).
*/
VertexArray* Game::getCleanerBody() {
	cleanerBody = new VertexArray(LineStrip, 5);
	pair<int, int> cell = (!change) ? getCleanerPosition() : pair<int, int>(rowCleaner, colCleaner);
	int row = cell.first;
	int col = cell.second;
	rowCleaner = row;
	colCleaner = col;
	(*cleanerBody)[0].position = Vector2f(cellSize*(col-1)+cellSize/2.f, cellSize*(row-1)+cellSize/2.f);
	(*cleanerBody)[1].position = Vector2f((*cleanerBody)[0].position.x, (*cleanerBody)[0].position.y+30.f);
	(*cleanerBody)[2].position = Vector2f((*cleanerBody)[1].position.x-30.f, (*cleanerBody)[1].position.y+20.f);
	(*cleanerBody)[3].position = Vector2f((*cleanerBody)[1].position.x+30.f, (*cleanerBody)[2].position.y);
	(*cleanerBody)[4].position = Vector2f((*cleanerBody)[3].position.x-30.f, (*cleanerBody)[1].position.y);
	
	(*cleanerBody)[0].color = Color::Black;
	(*cleanerBody)[1].color = Color::Black;
	(*cleanerBody)[2].color = Color(0,0,0,0);
	(*cleanerBody)[3].color = Color(0,0,0,0);
	(*cleanerBody)[4].color = Color::Black;
	
	return cleanerBody;
}

/* Создать случайные булыжники на игровом поле.
 * Return: указатель на массив указателей на камни.
*/
CircleShape** Game::getStones() {
	texture_stone = new Texture();
	if (!texture_stone->loadFromFile("Textures/texture-stone-gray.png")) return NULL;
	
	stonesPosition = getStonesPosition();						// координаты клеток поля с будущими камнями (1,2,...)
	CircleShape **stones = new CircleShape *[stonesAmount];
	for (int i = 0; i < stonesAmount; ++i) {
		stones[i] = new CircleShape();
		float vy = (cellSize+outlineSize) * ((stonesPosition[i]->first) - 1.f);
		float vx = (cellSize+outlineSize) * ((stonesPosition[i]->second) - 1.f);
		stones[i]->setPosition(Vector2f(vx, vy));
		stones[i]->setRadius(cellSize/2.f);
		stones[i]->setTexture(texture_stone);
	}
	return stones;
}

/* Создать помойку.
 * Return: указатель на форму прямоугольника с текстурой помойки внутри.
*/
RectangleShape* Game::getTrash() {
	pair<int, int> position = getTrashPosition();
	trashRow = position.first;
	trashCol = position.second;
	
	trash_texture =  new Texture();
	if (!trash_texture->loadFromFile("Images/trash.png")) return NULL;
	RectangleShape *trash = new RectangleShape();
	trash->setTexture(trash_texture);
	trash->setSize(Vector2f(cellSize, cellSize));
	float trashX = (cellSize+outlineSize)*(trashCol-1.f);
	float trashY = (cellSize+outlineSize)*(trashRow-1.f);
	trash->setPosition(trashX, trashY);
	
	return trash;
}

/* Конструктор класса Game.
 * Инициализирует все члены-данные.
*/
Game::Game() {
	change = false;
	pair<int, int> rands = getFloorSize();
	rows = rands.first;																// кол-во строк мира
	cols = rands.second;															// кол-во столбцов мира
	maxRowOrCol = (rows < cols) ? rows : cols;						// максимальное значение размера мира
	
	pair <RectangleShape**, Texture*> justAPair = makeFloor();
	if ((justAPair.first == NULL)||(justAPair.second== NULL)) {delete this;}
	rects = justAPair.first;															// сгенерированные клетки игрового поля
	texture_floor = justAPair.second;											// текстура этих клеток
	
	cleanerBody = getCleanerBody();										// Сначала генерируется уборщик (без головы)),
	trash = getTrash();																// затем помойка,
	stones = getStones();															// а только потом - камни (булыжники).
	change = true;
}

/*
 * Отрисовать все объекты игры.
*/
void Game::gameDraw(RenderWindow& window) {
	for (int j = 0; j < cols; ++j)
		for (int i = 0; i < rows; ++i)
			window.draw(rects[i][j]);
	// Отрисовать в той же последовательности:
	window.draw(*cleanerBody);
	window.draw(*trash);
	for (int i = 0; i < stonesAmount; ++i)
		if (stones[i])
			window.draw(*(stones[i]));
	
	if (gameMsg)
		window.draw(*gameMsg);
}

/* Написать подсказывающий текст.
*/
void Game::setMsg(string msg) {
	if (!gameFont) {
		gameFont = new Font;
		if (!gameFont->loadFromFile("Fonts/Mathematical.ttf")) return;
	}
	if (!gameMsg) {
		Text *text = new Text;
		text->setFont(*gameFont);
		text->setCharacterSize(50);
		text->setFillColor(Color::Red);
		text->setPosition(Vector2f((float)cols*(cellSize+outlineSize), (cellSize+outlineSize)));
		text->setRotation(-10.f);
		
		gameMsg = text;
	}
	
	gameMsg->setString(msg);
}

/* Передвинуть уборщика на соседнюю клетку в соответствии
 * с текущим действием сгенерированного плана.
*/
void Game::moveCleaner(string where) {
	if ((where == "movedown")||(where == "movedownloaded")) {
		delete cleanerBody;
		rowCleaner += 1;
		cleanerBody = getCleanerBody();
	}
	if ((where == "moveup")||(where == "moveuploaded")) {
		delete cleanerBody;
		rowCleaner -= 1;
		cleanerBody = getCleanerBody();
	}
	if ((where == "moveright")||(where == "moverightloaded")) {
		delete cleanerBody;
		colCleaner += 1;
		cleanerBody = getCleanerBody();
	}
	if ((where == "moveleft")||(where == "moveleftloaded")) {
		delete cleanerBody;
		colCleaner -= 1;
		cleanerBody = getCleanerBody();
	}
	
	setMsg("cleaner " + where);
}

/* Удалить камень и передвинуть на его место
 * уборщика.
*/
void Game::pickStone(string whatStone) {
	if (whatStone == "pickleftstone") {
		for (int i = 0; i < stonesAmount; ++i)
			if ((stonesPosition[i])&&(stonesPosition[i]->second == colCleaner-1)&&(stonesPosition[i]->first == rowCleaner)) {
				delete stonesPosition[i];
				delete stones[i];
				stonesPosition[i] = NULL;
				stones[i] = NULL;
				setMsg(whatStone + " s" + to_string(i+1));
				break;
			}
		delete cleanerBody;
		colCleaner -= 1;
		cleanerBody = getCleanerBody();
	}
	if (whatStone == "pickrightstone") {
		for (int i = 0; i < stonesAmount; ++i)
			if ((stonesPosition[i])&&(stonesPosition[i]->second == colCleaner+1)&&(stonesPosition[i]->first == rowCleaner)) {
				delete stonesPosition[i];
				delete stones[i];
				stonesPosition[i] = NULL;
				stones[i] = NULL;
				setMsg(whatStone + " s" + to_string(i+1));
				break;
			}
		delete cleanerBody;
		colCleaner += 1;
		cleanerBody = getCleanerBody();
	}
}