#include "Engine.h"
#include <chrono>
#include <thread>
int main()
{
	Engine engine;

	while (engine.gamestate == GameState::GameRuns)
	{
		engine.render();
		engine.run();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return EXIT_SUCCESS;
}