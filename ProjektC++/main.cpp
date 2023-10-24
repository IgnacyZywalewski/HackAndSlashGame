#include "game.h"
#include "player.h"
#include <ctime>


int main(int argc, char* argv[]) {
	srand(time(nullptr));

	Game game;
	game.run();

	return 0;
}