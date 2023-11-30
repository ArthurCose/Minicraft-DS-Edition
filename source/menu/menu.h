#pragma once

#include "../gfx/screen.h"

#include <vector>
#include <memory>
#include "listitem.h"
#include "../game.h"
#include "../gfx/color.h"

class Game;

class Menu
{
public:
  bool blocksGameTick = true;

  virtual void tick(Game& game) {};
  virtual void resume(Game& game) {};
  virtual void render(Screen& screen, Screen& bottomScreen) {};

  template <class T,
    class SharedPtr = typename T::value_type,
    class Element = typename SharedPtr::element_type,
    class IsDerived = typename std::enable_if<std::is_base_of<ListItem, Element>::value, void*>::type>
  void renderItemList(
    Screen& screen,
    int xo, int yo, int x1, int y1,
    const T& listItems,
    int selected,
    bool renderCursor)
  {
    const int itemCount = listItems.size();
    renderCursor = renderCursor && itemCount > 0;

    const int w = x1 - xo;
    const int h = y1 - yo - 1;

    int visibleCount = itemCount;
    if (visibleCount > h)
      visibleCount = h;

    int listStart = selected - h / 2;

    if (listStart > itemCount - h)
      listStart = itemCount - h;
    if (listStart < 0)
      listStart = 0;

    for (int i = 0; i < visibleCount; i++) {
      listItems[i + listStart]->renderInventory(screen, (1 + xo) * 8, (i + 1 + yo) * 8);
    }

    if (renderCursor) {
      int yy = selected + 1 - listStart + yo;
      screen.renderText(">", (xo + 0) * 8, yy * 8, Color::get(5, 555, 555, 555));
      screen.renderText("<", (xo + w) * 8, yy * 8, Color::get(5, 555, 555, 555));
    }
  }

  void setParent(std::unique_ptr<Menu> menu);

  void close(Game& game);

protected:
  std::unique_ptr<Menu> parent;
};
