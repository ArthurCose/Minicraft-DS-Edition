#include "generatemenu.h"

GenerateMenu::GenerateMenu()
{
  currentStep = 0;
  currentStepName = "Generating Sky";
}

void GenerateMenu::tick(Game &game)
{
  int levelSize = 128;

  switch (currentStep)
  {
  case 0:
    game.frameSkipEnabled = false;
    game.levels.clear();

    game.levels.emplace(game.levels.begin(), levelSize, levelSize, 1);

    currentStepName = "Generating Overworld";
    break;
  case 1:
    game.levels.emplace(game.levels.begin(), levelSize, levelSize, 0, game.levels[0]);

    currentStepName = "Generating Underground 1";
    break;
  case 2:
    game.levels.emplace(game.levels.begin(), levelSize, levelSize, -1, game.levels[0]);

    currentStepName = "Generating Underground 2";
    break;
  case 3:
    game.levels.emplace(game.levels.begin(), levelSize, levelSize, -2, game.levels[0]);

    currentStepName = "Generating Underground 3";
    break;
  case 4:
    game.levels.emplace(game.levels.begin(), levelSize, levelSize, -3, game.levels[0]);

    currentStepName = "Choosing spawn location";
    break;
  case 5:
    game.currentLevel = game.levels.size() - 2;

    game.player = std::make_shared<Player>();
    game.player->findStartPos(game.levels[game.currentLevel]);
    game.levels[game.currentLevel].add(game.player);

    currentStepName = "Spawning mobs";
    break;
  case 6:
    for (size_t i = 0; i < game.levels.size(); i++)
    {
      game.levels[i].trySpawn(5000);
    }

    currentStepName = "Done!";
    break;
  case 7:
    game.frameSkipEnabled = true;
    close(game);
    break;
  }

  currentStep++;
}

void GenerateMenu::render(Screen &screen)
{
  screen.clear(0);

  screen.renderTextCentered(currentStepName, screen.w / 2, screen.h / 2 - 16, Color::get(-1, 555, 555, 555));

  const int barColor = 151;
  const int barColors = Color::get(barColor, barColor, barColor, barColor);
  const int barBackgroundColor = 555;
  const int barBackgroundColors = Color::get(barBackgroundColor, barBackgroundColor, barBackgroundColor, barBackgroundColor);

  const int totalSteps = 8;

  for (int i = 0; i < totalSteps; i++)
    screen.renderTile(
        screen.w / 2 - totalSteps * 4 + i * 8,
        screen.h / 2 + 16,
        0,
        i < currentStep ? barColors : barBackgroundColors,
        0);
}
