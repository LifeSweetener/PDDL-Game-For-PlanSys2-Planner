`rm sfml_game`
`g++ -c game.cpp Include/class.cpp Include/planner.cpp Include/class.h -I /home/timofey/Desktop/C++/SFML/include`
`g++ game.o class.o planner.o -o sfml_game -L /home/timofey/Desktop/C++/SFML_build/lib -lsfml-graphics -lsfml-window -lsfml-system`
`rm game.o`
`rm class.o`
`rm planner.o`
`./sfml_game`
