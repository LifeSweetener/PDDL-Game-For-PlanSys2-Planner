#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <cstdlib>
#include "class.h"
#include <SFML/Graphics.hpp>

string Planner::getPDDLProblem() {
	// Генерация блока PDDL-объектов и их типов:
	string codeToProblemFile = "(define (problem GameNextTask)\n\
\t(:domain GameDomain)\n\
\t(:objects\n\
\t\tcl1 - cleaner\n\
\t\tt1 - trash\n\
\t\t";
	
	int rows = game->getRows();
	int cols = game->getCols();
	int rowCleaner = game->getCleanerRow();
	int colCleaner = game->getCleanerCol();
	int trashRow = game->getTrashRow();
	int trashCol = game->getTrashCol();
	int stonesAmount = game->getStonesAmount();
	pair<int, int>** stonesPosition = game->getStonesPositionPublic();
	for (int i = 0; i < stonesAmount; ++i) codeToProblemFile += "s" + to_string(i+1) + " ";
	codeToProblemFile += "- stone\n\t";
	
	for (int i = 0; i < rows; ++i) {
		codeToProblemFile += "\t";
		for (int j= 0; j < cols; ++j) codeToProblemFile += "c" + to_string(i+1) + to_string(j+1) + " ";
		codeToProblemFile += "- cell\n\t";
	}
	codeToProblemFile += ")\n\t(:init\n";
	
	// Блок init:
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			codeToProblemFile += "\t\t(= (row_cell c"  + to_string(i+1) + to_string(j+1) + ") " + to_string(i+1) + ") (= (col_cell c"  + to_string(i+1) + to_string(j+1) + ") " + to_string(j+1) + ")\n";
	
	for (int i = 0; i < stonesAmount; ++i) codeToProblemFile += "\t\t(stoneExist s" + to_string(i+1) + ")\n";
	
	bool occupied = false;
	for (int i = 0; i < rows; ++i) {
		codeToProblemFile += "\t\t";
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < stonesAmount; ++k) {
					if ((stonesPosition[k]->first == (i+1))&&(stonesPosition[k]->second == (j+1))) {
						codeToProblemFile += "(occupied c"  + to_string(i+1) + to_string(j+1) + " s" + to_string(k+1) + ")";
						occupied = true;
						break;
					}
			}
			if (!occupied) codeToProblemFile += "(free c"  + to_string(i+1) + to_string(j+1) + ")\t";
			occupied = false;
		}
		codeToProblemFile += "\n";
	}
	
	codeToProblemFile += "\t\t(ready cl1)\n";
	codeToProblemFile += "\t\t(= (row_cleaner cl1) " + to_string(rowCleaner) + ")\n";
	codeToProblemFile += "\t\t(= (col_cleaner cl1) " + to_string(colCleaner) + ")\n";
	codeToProblemFile += "\t\t(= (row_trash t1) " + to_string(trashRow) + ")\n";
	codeToProblemFile += "\t\t(= (col_trash t1) " + to_string(trashCol) + ")\n\n";
	
	codeToProblemFile += "\t\t(= (totalEnergyCost) 0)\n";
	codeToProblemFile += "\t)\n\
\t(:goal (and\n";
	
	// Конец файла с проблемой:
	for (int i = 0; i < stonesAmount; ++i)
		codeToProblemFile += "\t\t(stoneRemoved s" + to_string(i+1) + ")\n";
	
	codeToProblemFile += "\t))\n\
\t(:metric minimize (totalEnergyCost))\n\
)";
	
	return codeToProblemFile;
}

void Planner::getPDDLPlan() {
	// создание проблемы и её запись в файл .pddl:
	string pddlProblem = getPDDLProblem();
	ofstream pddlProblemFile("PDDL/game_next_task.pddl");
	pddlProblemFile << pddlProblem;
	
	// запуск планировщика PlanSys2:
	system("ros2 run popf popf PDDL/game_domain.pddl PDDL/game_next_task.pddl > PDDL/plan.tmp");
	
	// копирование созданного планировщиком плана в программу:
	ifstream pddlPlan("PDDL/plan.tmp");
	while (!pddlPlan) pddlPlan = ifstream("PDDL/plan.tmp");
	string temp;
	while(getline(pddlPlan, temp)) plan += temp;
	system("rm PDDL/plan.tmp");
	
	int pos = plan.find("Solution Found");
	plan.replace(0, pos, "");
	pos = plan.find("000");
	plan.replace(0, pos, "");
	
	// создание односвязного списка с элементарными действиями:
	int i = 0;
	pos = 0;														// начальная позиция в строке плана, ограничивающая очередное действие с начала
	int posEnd;													// позиция в строке плана, ограничивающая очередное действие плана с конца
	planElement *elem= new planElement;		// выделить место для элемента односвязного списка
	header = elem;											// головной (начальный) элемент динамического односвязного списка
	while (true) {
		pos = plan.find("(", pos+1);
		if (pos >= plan.length()) {
			elem->action[0] = '\0';
			elem->id = i;
			elem->next = NULL;
			break;
		}
		
		posEnd = plan.find(" ", pos+1);
		int len = posEnd-pos-1;
		int start = pos+1;
		plan.copy(elem->action, len, start);			// вставить строку с очередным действием в элемент списка
		elem->action[len] = '\0';							// обозначить конец строки действия
		elem->id = i;												// определить номер очередного действия
		elem->next = new planElement;				// создать (в куче) следующий элемент односвязного списка
		elem = elem->next;									// сослаться на вновь созданный элемент списка
		++i;
	}
	planSize = i;											// общее кол-во действий в списке
}

bool Planner::realizePlan() {
	if (!header) return false;
	if (!current) current = header;
	
	if (string(current->action).find("move") != -1) game->moveCleaner(current->action);
	else if (string(current->action).find("pick") != -1) game->pickStone(current->action);
	
	if (string(current->action).find("throw") != -1) game->setMsg(string(current->action));
	
	if (current->next) current = current->next;
	else { game->setMsg("THE END! ALL STONES HAVE BEEN REMOVED."); end = true; }
	
	return true;
}

// конструктор пользовательского планировщика:
Planner::Planner() {
	end = false;
	game = new Game();
	getPDDLPlan();
}