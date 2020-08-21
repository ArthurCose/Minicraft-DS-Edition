#include "player.h"

#include <memory>
#include <functional>
#include <nds.h>
#include "../level/level.h"
#include "../item/furnitureitem.h"
#include "../item/powergloveitem.h"
#include "workbenches/workbench.h"
#include "particle/textparticle.h"
#include "../menu/player/inventorymenu.h"
#include "../menu/player/pausemenu.h"

Player::Player()
{
  x = 24;
  y = 24;
  stamina = maxStamina;

  inventory.add(std::make_shared<FurnitureItem>(std::make_shared<Workbench>()));
  inventory.add(std::make_shared<PowerGloveItem>());
}

void Player::tick(Game &game, Level &level, std::shared_ptr<Entity> self)
{
  Mob::tick(game, level, self);

  swimming = isSwimming(level);

  if (invulnerableTime > 0)
    invulnerableTime--;

  Tile *onTile = level.getTile(x >> 4, y >> 4);

  if (onTile == Tile::stairsDown || onTile == Tile::stairsUp)
  {
    if (onStairDelay == 0)
    {
      game.scheduleLevelChange((onTile == Tile::stairsUp) ? 1 : -1);
      onStairDelay = 10;
      return;
    }
    onStairDelay = 10;
  }
  else
  {
    if (onStairDelay > 0)
      onStairDelay--;
  }

  if (stamina <= 0 && staminaRechargeDelay == 0 && staminaRecharge == 0)
  {
    staminaRechargeDelay = 40;
  }

  if (staminaRechargeDelay > 0)
  {
    staminaRechargeDelay--;
  }

  if (staminaRechargeDelay == 0)
  {
    staminaRecharge++;

    if (swimming)
    {
      staminaRecharge = 0;
    }
    while (staminaRecharge > 10)
    {
      staminaRecharge -= 10;
      if (stamina < maxStamina)
        stamina++;
    }
  }

  int xa = 0;
  int ya = 0;
  if (game.isHeld(KEY_UP))
    ya--;
  if (game.isHeld(KEY_DOWN))
    ya++;
  if (game.isHeld(KEY_LEFT))
    xa--;
  if (game.isHeld(KEY_RIGHT))
    xa++;
  if (swimming && tickTime % 60 == 0)
  {
    if (stamina > 0)
    {
      stamina--;
    }
    else
    {
      Mob::hurt(level, *this, 1, dir ^ 1);
    }
  }

  if (staminaRechargeDelay % 2 == 0)
  {
    move(level, xa, ya);
  }

  if (game.justTapped(KEY_B) && stamina > 0)
  {
    stamina--;
    staminaRecharge = 0;
    attack(level);
  }

  if (game.justTapped(KEY_X))
    game.setMenu(std::make_unique<InventoryMenu>(std::static_pointer_cast<Player>(self)));

  if (game.justTapped(KEY_A))
    use(game, level);

  if (game.justTapped(KEY_START))
    game.enterMenu(std::make_unique<PauseMenu>());

  if (attackTime > 0)
    attackTime--;
}

void Player::attack(Level &level)
{
  walkDist += 8;
  attackDir = dir;
  attackItem = activeItem;
  bool done = false;

  if (activeItem != NULL)
  {
    attackTime = 10;
    int yo = -2;
    int range = 12;
    if (dir == 0 && interact(level, x - 8, y + 4 + yo, x + 8, y + range + yo))
      done = true;
    if (dir == 1 && interact(level, x - 8, y - range + yo, x + 8, y - 4 + yo))
      done = true;
    if (dir == 3 && interact(level, x + 4, y - 8 + yo, x + range, y + 8 + yo))
      done = true;
    if (dir == 2 && interact(level, x - range, y - 8 + yo, x - 4, y + 8 + yo))
      done = true;
    if (done)
      return;

    int xt = x >> 4;
    int yt = (y + yo) >> 4;
    int r = 12;
    if (attackDir == 0)
      yt = (y + r + yo) >> 4;
    if (attackDir == 1)
      yt = (y - r + yo) >> 4;
    if (attackDir == 2)
      xt = (x - r) >> 4;
    if (attackDir == 3)
      xt = (x + r) >> 4;

    if (xt >= 0 && yt >= 0 && xt < level.w && yt < level.h)
    {
      if (activeItem->interactOn(*level.getTile(xt, yt), level, xt, yt, *this, attackDir))
      {
        done = true;
      }
      else
      {
        if (level.getTile(xt, yt)->interact(level, xt, yt, *this, *activeItem, attackDir))
        {
          done = true;
        }
      }
      if (activeItem->isDepleted())
      {
        activeItem = NULL;
      }
    }
  }

  if (done)
    return;

  if (activeItem == NULL || activeItem->canAttack())
  {
    attackTime = 5;
    int yo = -2;
    int range = 20;
    if (dir == 0)
      hurt(level, x - 8, y + 4 + yo, x + 8, y + range + yo);
    if (dir == 1)
      hurt(level, x - 8, y - range + yo, x + 8, y - 4 + yo);
    if (dir == 3)
      hurt(level, x + 4, y - 8 + yo, x + range, y + 8 + yo);
    if (dir == 2)
      hurt(level, x - range, y - 8 + yo, x - 4, y + 8 + yo);

    int xt = x >> 4;
    int yt = (y + yo) >> 4;
    int r = 12;
    if (attackDir == 0)
      yt = (y + r + yo) >> 4;
    if (attackDir == 1)
      yt = (y - r + yo) >> 4;
    if (attackDir == 2)
      xt = (x - r) >> 4;
    if (attackDir == 3)
      xt = (x + r) >> 4;

    if (xt >= 0 && yt >= 0 && xt < level.w && yt < level.h)
    {
      level.getTile(xt, yt)->hurt(level, xt, yt, *this, random.nextInt(3) + 1, attackDir);
    }
  }
}

bool Player::use(Game &game, Level &level)
{
  int yo = -2;
  if (dir == 0 && use(game, level, x - 8, y + 4 + yo, x + 8, y + 12 + yo))
    return true;
  if (dir == 1 && use(game, level, x - 8, y - 12 + yo, x + 8, y - 4 + yo))
    return true;
  if (dir == 3 && use(game, level, x + 4, y - 8 + yo, x + 12, y + 8 + yo))
    return true;
  if (dir == 2 && use(game, level, x - 12, y - 8 + yo, x - 4, y + 8 + yo))
    return true;

  int xt = x >> 4;
  int yt = (y + yo) >> 4;
  int r = 12;
  if (attackDir == 0)
    yt = (y + r + yo) >> 4;
  if (attackDir == 1)
    yt = (y - r + yo) >> 4;
  if (attackDir == 2)
    xt = (x - r) >> 4;
  if (attackDir == 3)
    xt = (x + r) >> 4;

  if (xt >= 0 && yt >= 0 && xt < level.w && yt < level.h)
  {
    if (level.getTile(xt, yt)->use(level, xt, yt, *this, attackDir))
      return true;
  }

  return false;
}

bool Player::use(Game &game, Level &level, int x0, int y0, int x1, int y1)
{
  auto entities = level.getEntities(x0, y0, x1, y1);

  for (auto e : entities)
  {
    if (e.get() != this)
      if (e->use(game, level, *this, attackDir))
        return true;
  }

  return false;
}

bool Player::interact(Level &level, int x0, int y0, int x1, int y1)
{
  auto entities = level.getEntities(x0, y0, x1, y1);

  for (auto e : entities)
  {
    if (e.get() != this)
      if (e->interact(*this, *activeItem, attackDir))
        return true;
  }

  return false;
}

void Player::hurt(Level &level, int x0, int y0, int x1, int y1)
{
  auto entities = level.getEntities(x0, y0, x1, y1);

  for (auto e : entities)
  {
    if (e.get() != this)
      e->hurt(level, *this, getAttackDamage(*e), attackDir);
  }
}

int Player::getAttackDamage(Entity &e)
{
  int dmg = random.nextInt(3) + 1;
  if (attackItem != NULL)
  {
    dmg += attackItem->getAttackDamageBonus(e);
  }
  return dmg;
}

void Player::render(Screen &screen)
{
  int xt = 0;
  int yt = 14;

  int flip1 = (walkDist >> 3) & 1;
  int flip2 = (walkDist >> 3) & 1;

  if (dir == 1)
  {
    xt += 2;
  }
  if (dir > 1)
  {
    flip1 = 0;
    flip2 = ((walkDist >> 4) & 1);
    if (dir == 2)
    {
      flip1 = 1;
    }
    xt += 4 + ((walkDist >> 3) & 1) * 2;
  }

  int xo = x - 8;
  int yo = y - 11;

  if (swimming)
  {
    yo += 4;
    int waterColor = Color::get(-1, -1, 115, 335);
    if (tickTime / 8 % 2 == 0)
    {
      waterColor = Color::get(-1, 335, 5, 115);
    }
    screen.renderTile(xo + 0, yo + 3, 5 + 13 * 32, waterColor, 0);
    screen.renderTile(xo + 8, yo + 3, 5 + 13 * 32, waterColor, 1);
  }

  if (attackTime > 0 && attackDir == 1)
  {
    screen.renderTile(xo + 0, yo - 4, 6 + 13 * 32, Color::get(-1, 555, 555, 555), 0);
    screen.renderTile(xo + 8, yo - 4, 6 + 13 * 32, Color::get(-1, 555, 555, 555), 1);
    if (attackItem != NULL)
    {
      attackItem->renderIcon(screen, xo + 4, yo - 4);
    }
  }
  int col = Color::get(-1, 100, 220, 532);
  if (hurtTime > 0)
  {
    col = Color::get(-1, 555, 555, 555);
  }

  auto furnitureItem = std::dynamic_pointer_cast<FurnitureItem>(activeItem);

  if (furnitureItem)
  {
    yt += 2;
  }

  screen.renderTile(xo + 8 * flip1, yo + 0, xt + yt * 32, col, flip1);
  screen.renderTile(xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, col, flip1);
  if (!swimming)
  {
    screen.renderTile(xo + 8 * flip2, yo + 8, xt + (yt + 1) * 32, col, flip2);
    screen.renderTile(xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, col, flip2);
  }

  if (attackTime > 0 && attackDir == 2)
  {
    screen.renderTile(xo - 4, yo, 7 + 13 * 32, Color::get(-1, 555, 555, 555), 1);
    screen.renderTile(xo - 4, yo + 8, 7 + 13 * 32, Color::get(-1, 555, 555, 555), 3);
    if (attackItem != NULL)
    {
      attackItem->renderIcon(screen, xo - 4, yo + 4);
    }
  }
  if (attackTime > 0 && attackDir == 3)
  {
    screen.renderTile(xo + 8 + 4, yo, 7 + 13 * 32, Color::get(-1, 555, 555, 555), 0);
    screen.renderTile(xo + 8 + 4, yo + 8, 7 + 13 * 32, Color::get(-1, 555, 555, 555), 2);
    if (attackItem != NULL)
    {
      attackItem->renderIcon(screen, xo + 8 + 4, yo + 4);
    }
  }
  if (attackTime > 0 && attackDir == 0)
  {
    screen.renderTile(xo + 0, yo + 8 + 4, 6 + 13 * 32, Color::get(-1, 555, 555, 555), 2);
    screen.renderTile(xo + 8, yo + 8 + 4, 6 + 13 * 32, Color::get(-1, 555, 555, 555), 3);
    if (attackItem != NULL)
    {
      attackItem->renderIcon(screen, xo + 4, yo + 8 + 4);
    }
  }

  if (furnitureItem)
  {
    auto furniture = furnitureItem->furniture;
    furniture->x = x;
    furniture->y = yo;
    furniture->render(screen);
  }
}

void Player::touchItem(ItemEntity &itemEntity)
{
  itemEntity.take(*this);
  inventory.add(itemEntity.item);
}

bool Player::canSwim()
{
  return true;
}

bool Player::findStartPos(Level &level)
{
  struct Position
  {
    int x, y;
  };

  Position spawn;

  auto findAcceptablePos = [&](std::function<bool(Tile *)> acceptableStart) {
    std::vector<Position> spawnablePositions;
    spawnablePositions.reserve(100);

    for (int y = 0; y < level.h; y++)
    {
      for (int x = 0; x < level.w; x++)
      {
        if (acceptableStart(level.getTile(x, y)))
        {
          spawnablePositions.push_back({x, y});
        }
      }
    }

    if (spawnablePositions.size() > 0)
    {
      int index = random.nextInt(spawnablePositions.size());
      spawn = spawnablePositions[index];
      return true;
    }

    return false;
  };

  // attempt to start on a grass tile
  bool foundAcceptableStart = findAcceptablePos([](Tile *tile) { return tile == Tile::grass; });

  if (!foundAcceptableStart)
  {
    // accept any walkable space
    foundAcceptableStart = findAcceptablePos([&](Tile *tile) {
      return tile != Tile::water &&
             tile != Tile::lava &&
             tile->mayPass(level, x, y, *this);
    });
  }

  if (!foundAcceptableStart)
  {
    // accept anything
    spawn.x = random.nextInt(level.w);
    spawn.y = random.nextInt(level.h);
  }

  this->x = spawn.x * 16 + 8;
  this->y = spawn.y * 16 + 8;

  return true;
}

bool Player::payStamina(int cost)
{
  if (cost > stamina)
    return false;
  stamina -= cost;
  return true;
}

int Player::getLightRadius()
{
  int r = 2;
  if (activeItem != NULL)
  {
    if (auto furnitureItem = std::dynamic_pointer_cast<FurnitureItem>(activeItem))
    {
      int rr = furnitureItem->furniture->getLightRadius();
      if (rr > r)
        r = rr;
    }
  }
  return r;
}

void Player::touchedBy(Level &level, Entity &entity)
{
  if (!dynamic_cast<Player *>(&entity))
  {
    entity.touchedBy(level, *this);
  }
}

void Player::doHurt(Level &level, int damage, int attackDir)
{
  if (hurtTime > 0 || invulnerableTime > 0)
    return;

  Sound::playerHurt.play();
  level.add(std::make_shared<TextParticle>(std::to_string(damage), x, y, Color::get(-1, 504, 504, 504)));
  health -= damage;
  if (attackDir == 0)
    yKnockback = +6;
  if (attackDir == 1)
    yKnockback = -6;
  if (attackDir == 2)
    xKnockback = -6;
  if (attackDir == 3)
    xKnockback = +6;
  hurtTime = 10;
  invulnerableTime = 30;
}

void Player::die(Game &game, Level &level)
{
  Mob::die(game, level);
  Sound::playerDeath.play();
}