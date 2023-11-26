#include "ingamemenu.h"

#include <nds/arm9/input.h>
#include <algorithm>

static const int HOTBAR_Y = 8;
static const int ITEM_W = 8;
static const int ITEM_H = 8;
static const int ITEM_SPACING = 4;
static const int ROTATE_CHAR_MARGIN = 8;
static const int ROTATE_BUTTON_W = ROTATE_CHAR_MARGIN * 2 + 8;

static inline int calculateItemHotbarX(int i) {
  return ROTATE_BUTTON_W + i * (ITEM_W + ITEM_SPACING * 2) + ITEM_SPACING;
}

static inline int calculateHotbarLen(Screen& bottomScreen) {
  return (bottomScreen.w - 48) / 16;
}

static inline int resolveHoveredIndex(Screen& bottomScreen, Player& player, int startIndex, int touchX, int touchY) {
  if (touchY < HOTBAR_Y - ITEM_SPACING || touchY > HOTBAR_Y + ITEM_H + ITEM_SPACING) {
    return -1;
  }

  auto& items = player.inventory.items;
  int itemCount = items.size();
  int hotbarLen = calculateHotbarLen(bottomScreen);

  for (int i = 0; i < hotbarLen; i++) {
    int index = startIndex + i;

    if (index >= itemCount) {
      break;
    }

    int x = calculateItemHotbarX(i);

    if (touchX >= x - ITEM_SPACING && touchX < x + ITEM_W + ITEM_SPACING) {
      return index;
    }
  }

  return -1;
}

InGameMenu::InGameMenu(std::shared_ptr<Player> player, std::shared_ptr<std::vector<unsigned char>> map)
{
  this->player = player;
  this->map = map;
  lastSelectedItemIndex = player->getSelectedItemIndex();
  previouslyHoldingItem = player->holdingItem();
  blocksGameTick = false;
}

void InGameMenu::tick(Game& game)
{
  game.frameSkipEnabled = true;
  handleItemDragging(game);
  handleTouchButtons(game);
  clampHotbar(game);
}

void InGameMenu::handleTouchButtons(Game& game) {
  if (!game.justTappedOrRepeat(KEY_TOUCH)) {
    return;
  }

  int x = game.touchX();
  int y = game.touchY();

  if (y > HOTBAR_Y + ITEM_H + ITEM_SPACING) {
    return;
  }

  if (x < ROTATE_BUTTON_W) {
    startIndex--;
  }

  if (x > game.bottomScreen.w - ROTATE_BUTTON_W) {
    startIndex++;
  }
}

void InGameMenu::handleItemDragging(Game& game) {
  auto& player = *game.player;

  if (game.justReleased(KEY_TOUCH)) {
    if (draggedIndex != -1) {
      draggedIndex = -1;
      player.setItemHeld(settingHeld);
    }
  }

  if (!game.isHeld(KEY_TOUCH)) {
    return;
  }

  if (draggedIndex == -1) {
    // initializing settingHeld, default to toggle
    settingHeld = !player.holdingItem();
  } else if (hoveredIndex == -1) {
    // prevent toggle if an item is dragged out of the hotbar
    settingHeld = true;
  }

  dragX = game.touchX();
  dragY = game.touchY();
  hoveredIndex = resolveHoveredIndex(game.bottomScreen, player, startIndex, dragX, dragY);

  if (hoveredIndex != -1) {
    auto& items = player.inventory.items;

    if (draggedIndex != -1 && (uint)draggedIndex < items.size()) {
      // dragging an item, and hovering an item
      // rotating items to swap
      player.inventory.swapSlots(draggedIndex, hoveredIndex);
    }

    if (player.getSelectedItemIndex() != hoveredIndex) {
      player.setSelectedItemIndex(hoveredIndex);

      // force holding if we weren't selecting this slot before
      settingHeld = true;
    }

    player.setItemHeld(true);

    draggedIndex = hoveredIndex;
  }
}

void InGameMenu::clampHotbar(Game& game) {
  auto& player = *game.player;
  int selected = player.getSelectedItemIndex();
  int hotbarLen = calculateHotbarLen(game.bottomScreen);
  bool selectionModified =
    selected != lastSelectedItemIndex || player.holdingItem() != previouslyHoldingItem;

  if (selectionModified && draggedIndex == -1) {
    // clamp to keep selection on screen
    startIndex = std::clamp(startIndex, selected - hotbarLen + 1, selected);
  }

  // clamp to keep items on screen
  int totalItems = player.inventory.items.size();
  startIndex = std::clamp(startIndex, 0, std::max(totalItems - hotbarLen, 0));

  // update tracking for clamping
  lastSelectedItemIndex = player.getSelectedItemIndex();
  previouslyHoldingItem = player.holdingItem();
}

void InGameMenu::render(Screen& screen, Screen& bottomScreen)
{
  bottomScreen.clear(Color::get(5));

  renderHud(screen);
  renderMap(bottomScreen);
  renderInventory(bottomScreen);
}

void InGameMenu::renderHud(Screen& screen)
{
  const int hudTop = screen.h - 8;
  const int staminaBarLeft = screen.w - 10 * 8;

  for (int i = 0; i < 10; i++) {
    if (i < player->health)
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 200, 500, 533), 0);
    else
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 100, 000, 000), 0);

    if (player->staminaRechargeDelay > 0) {
      if (player->staminaRechargeDelay / 4 % 2 == 0)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 555, 000, 000), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    } else {
      if (i < player->stamina)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 220, 550, 553), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    }
  }
}

void InGameMenu::renderInventory(Screen& bottomScreen)
{
  auto& items = player->inventory.items;
  auto activeItem = player->getActiveItem();

  bottomScreen.renderText("<", ROTATE_CHAR_MARGIN, HOTBAR_Y, Color::get(-1, 555, 555, 555));
  bottomScreen.renderText(">", bottomScreen.w - ROTATE_CHAR_MARGIN * 2, HOTBAR_Y, Color::get(-1, 555, 555, 555));

  int itemCount = items.size();
  int hotbarLen = calculateHotbarLen(bottomScreen);

  for (int i = 0; i < hotbarLen; i++) {
    int index = startIndex + i;

    if (index >= itemCount) {
      break;
    }

    if (index == draggedIndex && hoveredIndex != draggedIndex) {
      continue;
    }

    int x = calculateItemHotbarX(i);

    if (activeItem != NULL && player->getSelectedItemIndex() == index) {
      bottomScreen.renderBox(x, HOTBAR_Y, ITEM_W, ITEM_H, Color::get(444));
    }

    auto& item = *items[index];

    bottomScreen.renderTile(x, HOTBAR_Y, item.getSprite(), item.getColor(), 0);
  }

  if (activeItem != NULL) {
    auto name = activeItem->getName();

    int iconOffset = -12;
    int itemNameLeft = (bottomScreen.w - name.size() * 8) / 2 - iconOffset / 2;
    int itemNameTop = bottomScreen.h - 24;

    bottomScreen.renderTile(itemNameLeft + iconOffset, itemNameTop, activeItem->getSprite(), activeItem->getColor(), 0);
    bottomScreen.renderText(name, itemNameLeft, itemNameTop, Color::get(-1, 555, 555, 555));

    if (auto resourceItem = std::dynamic_pointer_cast<ResourceItem>(activeItem)) {
      bottomScreen.renderTextCentered(std::to_string(resourceItem->count), bottomScreen.w / 2, itemNameTop + 12, Color::get(-1, 222, 222, 222));
    }
  }

  if (hoveredIndex == -1 && draggedIndex != -1 && (uint)draggedIndex < items.size()) {
    auto& item = *items[draggedIndex];
    int x = dragX - ITEM_W / 2;
    int y = dragY - ITEM_H / 2;
    bottomScreen.renderTile(x, y, item.getSprite(), item.getColor(), 0);
  }
}

void InGameMenu::renderMap(Screen& screen)
{
  int mapLeft = 64;
  int mapTop = 32;
  int levelSize = 128;

  if (auto sscreen = dynamic_cast<SoftwareScreen*>(&screen)) {
    for (int y = 0; y < levelSize; y++) {
      int channel = y % 4;

      while (dmaBusy(channel)) {
        // wait for channel
      }

      dmaCopyWordsAsynch(
        channel,
        &map->at(y * levelSize),
        &sscreen->pixels[(y + mapTop) * screen.w + mapLeft],
        levelSize);
    }
  } else {
    for (int y = 0; y < levelSize; y++) {
      for (int x = 0; x < levelSize; x++) {
        screen.renderPixel(mapLeft + x, mapTop + y, map->at(y * levelSize + x));
      }
    }
  }

  screen.setOffset(
    player->x - mapLeft - (player->x >> 4),
    player->y - mapTop - (player->y >> 4));
  player->render(screen);
  screen.setOffset(0, 0);
}
