#pragma once

#include "../menu.h"
#include "../../entity/player.h"

class InGameMenu : public Menu
{
public:
  InGameMenu(std::shared_ptr<Player> player, std::shared_ptr<std::vector<unsigned char>> map);

  void tick(Game& game) override;
  void resume(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;

private:
  std::shared_ptr<Player> player;
  std::shared_ptr<std::vector<unsigned char>> map;
  int startIndex = 0;
  int hoveredIndex = -1;
  int draggedIndex = -1;
  int dragX, dragY;
  int lastSelectedItemIndex;
  bool previouslyHoldingItem;
  bool settingHeld;

  // used for rendering
  int lastDragX;
  int lastDragY;
  bool previouslyDragging = -1;
  int lastLevel = -1;
  bool levelChanged = false;
  bool justOpened = true;

  void handleItemDragging(Game& game);
  void handleTouchButtons(Game& game);
  void clampHotbar(Game& game);
  void clearOld(Screen& bottomScreen);
  void renderHud(Screen& screen);
  void renderInventory(Screen& bottomScreen);
  void renderMap(Screen& bottomScreen);
  void clearBox(Screen& screen, int x, int y, int w, int h);
};
