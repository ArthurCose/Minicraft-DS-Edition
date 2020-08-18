#pragma once

#include "../menu.h"

class GenerateMenu : public Menu
{
private:
  std::string currentStepName;
  int currentStep;

public:
  GenerateMenu();

  void tick(Game &game) override;
  void render(Screen &game) override;
};
