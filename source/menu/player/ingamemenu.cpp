#include "ingamemenu.h"

#include <nds/arm9/input.h>
#include <algorithm>

static const int HOTBAR_Y = 8;
static const int ITEM_W = 8;
static const int ITEM_H = 8;
static const int ITEM_SPACING = 4;
static const int ROTATE_CHAR_MARGIN = 8;
static const int ROTATE_BUTTON_W = ROTATE_CHAR_MARGIN * 2 + 8;
static const int MAP_LEFT = 64;
static const int MAP_TOP = 32;
static const int ACTIVE_ITEM_ICON_OFFSET = -12;
static const int ACTIVE_ITEM_NAME_TOP = SCREEN_HEIGHT - 24;
static const int ACTIVE_ITEM_COUNT_TOP = ACTIVE_ITEM_NAME_TOP + 12;
static const int CLEAR_COLOR = Color::get(5);

static inline int calculateActiveItemNameX(Item& item) {
  return (SCREEN_WIDTH - item.getName().size() * 8) / 2 - ACTIVE_ITEM_ICON_OFFSET / 2;
}

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

  levelChanged = game.currentLevel != lastLevel;
  lastLevel = game.currentLevel;
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

void InGameMenu::resume(Game& game)
{
  justOpened = true;
}

void InGameMenu::render(Screen& screen, Screen& bottomScreen)
{
  clearOld(bottomScreen);

  renderHud(screen);
  renderMap(bottomScreen);
  renderInventory(bottomScreen);

  justOpened = false;
}

void InGameMenu::clearOld(Screen& screen) {
  if (justOpened) {
    screen.clear(CLEAR_COLOR);
    return;
  }

  // erase previous dragged item
  if (previouslyDragging) {
    int x = lastDragX - ITEM_W / 2;
    int y = lastDragY - ITEM_H / 2;
    clearBox(screen, x, y, ITEM_W, ITEM_H);
    lastDragX = dragX;
    lastDragY = dragY;
  }

  // clear active item text
  auto activeItem = player->getActiveItem();

  if (previousActiveItem != nullptr && previousActiveItem != activeItem) {
    int x = calculateActiveItemNameX(*previousActiveItem);
    int y = ACTIVE_ITEM_NAME_TOP;

    // clear text
    clearBox(screen, x, y, previousActiveItem->getName().size() * 8, 8);

    // clear icon
    clearBox(screen, x + ACTIVE_ITEM_ICON_OFFSET, y, 8, 8);
  }

  // clear active item count
  int activeItemCount = -1;

  if (auto resourceItem = std::dynamic_pointer_cast<ResourceItem>(activeItem)) {
    activeItemCount = resourceItem->count;
  }

  if (previousActiveItemCount != -1 && previousActiveItemCount != activeItemCount) {
    int width = (int)std::log10(std::max(previousActiveItemCount, 1)) * 8;
    clearBox(screen, (screen.w - width) / 2, ACTIVE_ITEM_COUNT_TOP, width, 8);
  }
}

void InGameMenu::renderHud(Screen& screen)
{
  const int hudTop = screen.h - 8;
  const int staminaBarLeft = screen.w - 10 * 8;

  for (int i = 0; i < 10; i++) {
    if (i < player->health)
      screen.renderIcon(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 200, 500, 533), 0);
    else
      screen.renderIcon(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 100, 000, 000), 0);

    if (player->staminaRechargeDelay > 0) {
      if (player->staminaRechargeDelay / 4 % 2 == 0)
        screen.renderIcon(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 555, 000, 000), 0);
      else
        screen.renderIcon(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    } else {
      if (i < player->stamina)
        screen.renderIcon(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 220, 550, 553), 0);
      else
        screen.renderIcon(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
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
    int x = calculateItemHotbarX(i);

    bottomScreen.renderBoxFilled(x, HOTBAR_Y, ITEM_W, ITEM_H, CLEAR_COLOR);

    if (index >= itemCount) {
      continue;
    }

    if (index == draggedIndex && hoveredIndex != draggedIndex) {
      continue;
    }

    if (activeItem != NULL && player->getSelectedItemIndex() == index) {
      bottomScreen.renderBox(x, HOTBAR_Y, ITEM_W, ITEM_H, Color::get(444));
    }

    auto& item = *items[index];

    bottomScreen.renderIcon(x, HOTBAR_Y, item.getSprite(), item.getColor(), 0);
  }

  if (activeItem != NULL) {
    if (justOpened || activeItem != previousActiveItem) {
      auto name = activeItem->getName();
      int itemNameLeft = calculateActiveItemNameX(*activeItem);

      bottomScreen.renderIcon(itemNameLeft + ACTIVE_ITEM_ICON_OFFSET, ACTIVE_ITEM_NAME_TOP, activeItem->getSprite(), activeItem->getColor(), 0);
      bottomScreen.renderText(name, itemNameLeft, ACTIVE_ITEM_NAME_TOP, Color::get(-1, 555, 555, 555));
    }

    if (auto resourceItem = std::dynamic_pointer_cast<ResourceItem>(activeItem)) {
      if (previousActiveItemCount != resourceItem->count) {
        bottomScreen.renderTextCentered(std::to_string(resourceItem->count), bottomScreen.w / 2, ACTIVE_ITEM_COUNT_TOP, Color::get(-1, 222, 222, 222));
      }

      previousActiveItemCount = resourceItem->count;
    } else {
      previousActiveItemCount = -1;
    }
  }

  previousActiveItem = activeItem;

  // draw dragged item
  if (hoveredIndex == -1 && draggedIndex != -1 && (uint)draggedIndex < items.size()) {
    auto& item = *items[draggedIndex];
    int x = dragX - ITEM_W / 2;
    int y = dragY - ITEM_H / 2;
    bottomScreen.renderIcon(x, y, item.getSprite(), item.getColor(), 0);
  }

  previouslyDragging = draggedIndex != -1;
  lastDragX = dragX;
  lastDragY = dragY;
}

void InGameMenu::renderMap(Screen& screen)
{
  int levelSize = 128;

  auto& map = *this->map;

  int playerMapX = player->x >> 4;
  int playerMapY = player->y >> 4;

  if (levelChanged || justOpened) {
    if (auto sscreen = dynamic_cast<SoftwareScreen*>(&screen)) {
      DC_FlushRange(&map[0], map.size());

      for (int y = 0; y < levelSize; y++) {
        int channel = y % 4;

        while (dmaBusy(channel)) {
          // wait for channel
        }

        dmaCopyWordsAsynch(
          channel,
          &map[y * levelSize],
          &sscreen->pixels[(y + MAP_TOP) * screen.w + MAP_LEFT],
          levelSize);
      }
    } else {
      for (int y = 0; y < levelSize; y++) {
        for (int x = 0; x < levelSize; x++) {
          screen.renderPixel(MAP_LEFT + x, MAP_TOP + y, map[y * levelSize + x]);
        }
      }
    }
  } else {
    int width = 24;
    int left = playerMapX - width / 2;
    int top = playerMapY - 19;
    int bottom = playerMapY + 9;

    int height = bottom - top;

    clearBox(screen, MAP_LEFT + left, MAP_TOP + top, width, height);
  }

  screen.setOffset(
    player->x - MAP_LEFT - playerMapX,
    player->y - MAP_TOP - playerMapY
  );
  player->render(screen);
  screen.setOffset(0, 0);
}

void InGameMenu::clearBox(Screen& screen, int left, int top, int w, int h)
{
  auto& map = *this->map;

  int right = std::clamp(left + w, 0, screen.w);
  int bottom = std::clamp(top + h, 0, screen.h);
  left = std::clamp(left, 0, screen.w);
  top = std::clamp(top, 0, screen.h);

  int levelSize = 128;
  int mapX0 = std::clamp(left - MAP_LEFT, 0, levelSize);
  int mapX1 = std::clamp(right - MAP_LEFT, 0, levelSize);
  int mapY0 = std::clamp(top - MAP_TOP, 0, levelSize);
  int mapY1 = std::clamp(bottom - MAP_TOP, 0, levelSize);

  if (left < mapX0 || right > mapX1 || top < mapY0 || bottom > mapY1) {
    // no need to erase with the background color if this range is entirely within the map
    // the next loop will redraw the map

    for (int y = top; y < bottom; y++) {
      for (int x = left; x < right; x++) {
        screen.renderPixel(x, y, CLEAR_COLOR);
      }
    }
  }

  if (!levelChanged) {
    // no need to render the map if the level changes, as the entire map will be redrawn in that case

    for (int y = mapY0; y < mapY1; y++) {
      for (int x = mapX0; x < mapX1; x++) {
        screen.renderPixel(MAP_LEFT + x, MAP_TOP + y, map[y * levelSize + x]);
      }
    }
  }
}