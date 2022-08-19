#include "Engine.h"

int main()
{
	GameEngine engine("Bugslife.csv");
	//GameEngine engine("C:/dev/internship/task_6.1/task_6/src/Bugslife.csv");

	while (engine.game_result() == GameEngine::result_t::game_still_on)
	{
		engine.render();
		engine.on_year_pass();
	}

	if (engine.game_result() == GameEngine::result_t::ants_died_out)
		std::cout << "Ants lost";
	else
		std::cout << "Ants won!";

	return EXIT_SUCCESS;
}
