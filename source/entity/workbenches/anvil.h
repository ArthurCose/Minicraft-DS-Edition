#pragma once

#include "../furniture.h"

class Anvil : public Furniture
{
public:
  Anvil();

  bool use(Game& game, Level& level, Player& player, int attackDir) override;
  std::shared_ptr<Furniture> clone() override;
  std::string_view serializedClassName() override { return "Anvil"; }
};
