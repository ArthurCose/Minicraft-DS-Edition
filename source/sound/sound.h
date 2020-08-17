#pragma once

#include <maxmod9.h>

class Sound
{
public:
  static Sound playerHurt;
  static Sound playerDeath;
  static Sound monsterHurt;
  static Sound test;
  static Sound pickup;
  static Sound bossdeath;
  static Sound craft;

  void play();

private:
  Sound(mm_word id);
  mm_sound_effect sound_effect;

  static bool initialized;
};
