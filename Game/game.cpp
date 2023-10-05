#include <utility>
#include <iostream>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include "Include/class.h"
using namespace sf;
using namespace std;

int main()
{
	RenderWindow window(VideoMode::getDesktopMode(), "A game");
	Planner myPlanner;
	
	while (window.isOpen()) {
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) window.close();
			if ((ev.type == Event::KeyPressed)&&(myPlanner.getEnd() == false)) myPlanner.realizePlan();
			if ((ev.type == Event::KeyPressed)&&(myPlanner.getEnd())) window.close();
		}
		
		window.clear(Color(250, 250, 250));
		myPlanner.game->gameDraw(window);
		window.display();
	}
	
    return 0;
}