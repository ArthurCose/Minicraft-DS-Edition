#include <nds.h>
#include "game.h"

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
}

int main()
{
  initialize_palette();

  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);

  // videoSetModeSub(3);
  // consoleDemoInit();
  // keyboardDemoInit();
  // keyboardShow();

  Game game;

  while (true)
  {
    game.tick();
    // game.tick();
    // game.tick();
    // game.tick();
    game.render();

    // int key = keyboardUpdate();

    // if (key > 0)
    //   iprintf("%c", key);
  }

  return 0;
}
