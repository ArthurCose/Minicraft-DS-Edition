#include <nds.h>
#include <chrono>
#include <gl2d.h>
#include "game.h"
#include "icons.h"

void initialize_palette()
{
  int pp = 0;
  for (int r = 0; r < 6; r++)
  {
    for (int g = 0; g < 6; g++)
    {
      for (int b = 0; b < 6; b++)
      {
        int rr = (r * 255 / 5);
        int gg = (g * 255 / 5);
        int bb = (b * 255 / 5);
        int mid = (rr * 30 + gg * 59 + bb * 11) / 100;

        int r1 = ((rr + mid * 1) / 2) * 230 / 255 + 10;
        int g1 = ((gg + mid * 1) / 2) * 230 / 255 + 10;
        int b1 = ((bb + mid * 1) / 2) * 230 / 255 + 10;
        Screen::palette[pp++] = RGB15(r1 / 8, g1 / 8, b1 / 8);
      }
    }
  }

  Screen::palette[255] = 0;
}

// in ms
static int playtime = 0;

void incrementTime()
{
  playtime++;
}

int main()
{
  timerStart(0, ClockDivider_1024, (u16)TIMER_FREQ_1024(1000), incrementTime);

  videoSetMode(MODE_5_3D);
  glScreen2D();
  vramSetBankA(VRAM_A_TEXTURE);
  vramSetBankB(VRAM_B_TEXTURE);
  vramSetBankF(VRAM_F_TEX_PALETTE);

  initialize_palette();
  Screen::spriteSheet = std::make_unique<SpriteSheet>(iconsBitmap, 256, 256, 8);

  Game game;

  int refreshRate = 1000 / 59.8261;
  int lostTime = 0;

  while (true)
  {
    auto start = playtime;
    game.tick();

    glBegin2D();
    game.render();
    glEnd2D();
    glFlush(0);
    auto end = playtime;

    auto totalTime = end - start;

    lostTime += std::max(totalTime - refreshRate, 0);

    // skip up to one frame per render
    if (lostTime > refreshRate)
    {
      game.tick();
      lostTime -= refreshRate;
    }
  }

  return 0;
}
