#include "Engine.h"

//std::vector<std::string> GameEngine::m_level_rendering(50, std::string(190, ' '));

int main()
{
	//GameEngine engine("Bugslife.csv");
	GameEngine engine("C:/dev/internship/task_6.1/task_6/src/Bugslife.csv");

	while (engine.game_result() == GameEngine::result_t::game_still_on)
	{
		engine.on_year_pass();

		engine.render();
	}


	return EXIT_SUCCESS;
}