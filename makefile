hoarding.out: main.o Board.o DiceRoller.o Go.o Gamestate.o Move.o Player.o Property.o Space.o Rules.o Upgrade.o Auction.o
	g++ -g -Wall -Werror -std=c++14 -o hoarding.out main.o Board.o DiceRoller.o Go.o Gamestate.o Move.o Player.o Property.o Space.o Rules.o Upgrade.o Auction.o
main.o: main.cpp Gamestate.h Move.h
	g++ -g -Wall -Werror -std=c++14 -c main.cpp 
Board.o: Board.cpp Board.h Gamestate.h
	g++ -g -Wall -Werror -std=c++14 -c Board.cpp
DiceRoller.o: DiceRoller.cpp DiceRoller.h
	g++ -g -Wall -Werror -std=c++14 -c DiceRoller.cpp
Go.o: Go.cpp Go.h
	g++ -g -Wall -Werror -std=c++14 -c Go.cpp
Gamestate.o: Gamestate.cpp Gamestate.h Rules.h Board.h Player.h
	g++ -g -Wall -Werror -std=c++14 -c Gamestate.cpp
Move.o: Move.cpp Move.h Board.h Player.h Property.h
	g++ -g -Wall -Werror -std=c++14 -c Move.cpp
Player.o: Player.cpp Player.h Gamestate.h
	g++ -g -Wall -Werror -std=c++14 -c Player.cpp
Property.o: Property.cpp Property.h Rules.h Gamestate.h
	g++ -g -Wall -Werror -std=c++14 -c Property.cpp
Space.o: Space.cpp Space.h
	g++ -g -Wall -Werror -std=c++14 -c Space.cpp
Rules.o: Rules.cpp Rules.h
	g++ -g -Wall -Werror -std=c++14 -c Rules.cpp
Upgrade.o: Upgrade.cpp Upgrade.h Property.o Gamestate.o
	g++ -g -Wall -Werror -std=c++14 -c Upgrade.cpp
Auction.o: Auction.cpp Auction.h Gamestate.o
	g++ -g -Wall -Werror -std=c++14 -c Auction.cpp
clean:
	rm -f *.o *.out