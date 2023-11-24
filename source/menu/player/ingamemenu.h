#pragma once

#include "../menu.h"
#include "../../entity/player.h"

class InGameMenu : public Menu
{
public:
  InGameMenu(std::shared_ptr<Player> player, std::shared_ptr<std::vector<unsigned char>> map);

  void tick(Game& game) override;
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

  void handleItemDragging(Game& game);
  void handleTouchButtons(Game& game);
  void clampHotbar(Game& game);
  void renderHud(Screen& screen);
  void renderInventory(Screen& bottomScreen);
  void renderMap(Screen& bottomScreen);
};
