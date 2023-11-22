#include "sound.h"

#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

bool Sound::initialized = false;

Sound Sound::playerHurt(SFX_PLAYERHURT);
Sound Sound::playerDeath(SFX_DEATH);
Sound Sound::monsterHurt(SFX_MONSTERHURT);
Sound Sound::test(SFX_TEST);
Sound Sound::pickup(SFX_PICKUP);
Sound Sound::bossdeath(SFX_BOSSDEATH);
Sound Sound::craft(SFX_CRAFT);

Sound::Sound(mm_word id)
{
  sound_effect = {
      {id},                    // id
      (int)(1.0f * (1 << 10)), // rate
      0,                       // handle
      255,                     // volume
      255,                     // panning
  };
}

void Sound::play()
{
  if (!initialized) {
    mmInitDefaultMem((mm_addr)soundbank_bin);
    mmLoadEffect(SFX_PLAYERHURT);
    mmLoadEffect(SFX_DEATH);
    mmLoadEffect(SFX_MONSTERHURT);
    mmLoadEffect(SFX_TEST);
    mmLoadEffect(SFX_PICKUP);
    mmLoadEffect(SFX_BOSSDEATH);
    mmLoadEffect(SFX_CRAFT);

    initialized = true;
  }

  mmEffectEx(&sound_effect);
}
