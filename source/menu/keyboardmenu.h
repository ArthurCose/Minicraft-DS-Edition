#pragma once

#include "menu.h"

class KeyboardMenu : public Menu
{
public:
  KeyboardMenu(std::function<void(std::string)> callback);

  void setCharacterLimit(size_t limit) { characterLimit = limit; }
  void setPlaceholder(std::string text) { placeholder = text; }
  void submit(Game& game);
  void backspace();
  void insertChar(char c);
  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;

private:
  int bgColor;
  size_t characterLimit = 24;
  std::string text;
  std::string placeholder;
  size_t caret = 0;
  int caretTimer = 0;
  int touchX = 0;
  int touchY = 0;
  std::function<void(std::string)> callback;
};
