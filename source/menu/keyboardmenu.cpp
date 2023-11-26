#include "keyboardmenu.h"

static const std::vector<std::string_view> rows = {
  "1234567890-",
  "QWERTYUIOP",
  "ASDFGHJKL",
  "ZXCVBNM()",
};

const int BACKGROUND_COLOR = Color::get(0);
const int TEXTBOX_FRAME_COLOR = Color::get(555);
const int PLACEHOLDER_TEXT_COLOR = Color::get(-1, 333, 333, 333);
const int TEXT_COLOR = Color::get(-1, 555, 555, 555);
const int CARET_COLOR = Color::get(554);
const int KEY_HIGHLIGHT_COLOR = Color::get(444);
const int DEFAULT_KEY_COLOR = Color::get(555);
const int KEY_TEXT_COLOR = Color::get(-1, 0, 0, 0);

const int CARET_BLINK_DELAY = 0;
const int CARET_BLINK_RATE = 30;

const int TEXTBOX_X = 16;
const int TEXTBOX_Y = 16;
const int TEXTBOX_H_PADDING = 8;
const int TEXTBOX_V_PADDING = 6;
const int TEXTBOX_HEIGHT = 8 + TEXTBOX_V_PADDING * 2;
const int TEXT_X = TEXTBOX_X + TEXTBOX_H_PADDING;
const int TEXT_Y = TEXTBOX_Y + TEXTBOX_V_PADDING;

const int KEY_WIDTH = 16;
const int KEY_HEIGHT = 14;
const int KEY_SPACING = 1;
const int KEY_H_STRIDE = KEY_WIDTH + KEY_SPACING * 2;
const int KEY_V_STRIDE = KEY_HEIGHT + KEY_SPACING * 2;
const int KEY_H_PADDING = (KEY_WIDTH - 8) / 2;
const int KEY_V_PADDING = (KEY_HEIGHT - 8) / 2;
const int KEYBOARD_X = 14;
const int KEYBOARD_Y = 70;


struct SpecialKey {
  bool requiresInput{};
  int x, y, defaultColor, highlightColor;
  int definedWidth{};
  std::string_view text;
  std::function<void(Game&, KeyboardMenu&)> action;

  int width() const {
    if (definedWidth > 0) {
      return definedWidth;
    }

    return (int)text.size() * 8 + KEY_V_PADDING * 2;
  }
};

const int SUBMIT_X = KEYBOARD_X + KEY_H_STRIDE * (int)rows[0].size();

static const std::vector<SpecialKey> specialKeys = {
  // submit
  SpecialKey {
    .requiresInput = true,
    .x = SUBMIT_X,
    .y = KEYBOARD_Y + KEY_V_STRIDE * 2,
    .defaultColor = Color::get(252),
    .highlightColor = Color::get(141),
    .text = "- >",
    .action = [](Game& game, KeyboardMenu& menu) {
      menu.submit(game);
    },
  },
  // backspace
  SpecialKey {
    .x = SUBMIT_X,
    .y = KEYBOARD_Y,
    .defaultColor = Color::get(512),
    .highlightColor = Color::get(401),
    .text = "< X",
    .action = [](Game& game, KeyboardMenu& menu) {
      menu.backspace();
    },
  },
  // spacebar
  SpecialKey {
    .x = KEYBOARD_X + KEY_H_STRIDE,
    .y = KEYBOARD_Y + KEY_V_STRIDE * 4,
    .defaultColor = DEFAULT_KEY_COLOR,
    .highlightColor = KEY_HIGHLIGHT_COLOR,
    .definedWidth = KEY_H_STRIDE * ((int)rows[2].size()) + KEY_WIDTH,
    .text = "",
    .action = [](Game& game, KeyboardMenu& menu) {
      menu.insertChar(' ');
    },
  },
};

KeyboardMenu::KeyboardMenu(std::function<void(std::string)> callback) :
  callback(callback)
{
}

void KeyboardMenu::insertChar(char c) {
  if (text.size() < characterLimit) {
    text.insert(caret, 1, c);
    caret++;
    caretTimer = 0;
  }
}

void KeyboardMenu::setText(std::string text)
{
  this->text = text;
  caret = text.size();
}

void KeyboardMenu::submit(Game& game) {
  callback(text);
  close(game);
}

void KeyboardMenu::backspace() {
  if (caret > 0) {
    text = text.erase(caret - 1, 1);
    caret--;
    caretTimer = 0;
  }
}

static inline char touchedChar(int touchX, int touchY) {
  for (int i = 0; i < (int)rows.size(); i++) {
    std::string_view row = rows[i];

    for (int j = 0; j < (int)row.length(); j++) {
      int x = KEYBOARD_X + j * KEY_H_STRIDE + i * KEY_H_STRIDE / 2;
      int y = KEYBOARD_Y + i * KEY_V_STRIDE;

      if (touchX >= x && touchX < x + KEY_WIDTH && touchY >= y && touchY < y + KEY_HEIGHT) {
        return row[j];
      }
    }
  }

  return 0;
}

void KeyboardMenu::tick(Game& game)
{
  if (game.justTappedOrRepeat(KEY_B)) {
    close(game);
    return;
  }

  touchX = game.touchX();
  touchY = game.touchY();

  caretTimer++;

  if (game.justTappedOrRepeat(KEY_L) && !game.justTappedOrRepeat(KEY_R) && caret > 0) {
    caret--;
    caretTimer = 0;
  }

  if (game.justTappedOrRepeat(KEY_R) && !game.justTappedOrRepeat(KEY_L) && caret < text.size()) {
    caret++;
    caretTimer = 0;
  }

  if (game.justTappedOrRepeat(KEY_X) || game.justTappedOrRepeat(KEY_Y)) {
    backspace();
  }

  if (game.justTappedOrRepeat(KEY_TOUCH)) {
    if (char c = touchedChar(touchX, touchY)) {
      insertChar(c);
    }

    for (auto& key : specialKeys) {
      if (touchX >= key.x && touchX < key.x + key.width() && touchY >= key.y && touchY < key.y + KEY_HEIGHT) {
        if (!key.requiresInput || !text.empty()) {
          key.action(game, *this);
        }
        caretTimer = 0;
      }
    }
  }

  const int TEXTBOX_WIDTH = game.bottomScreen.w - TEXTBOX_X * 2;

  if (game.isHeld(KEY_TOUCH) && touchY > TEXTBOX_Y && touchY < TEXTBOX_Y + TEXTBOX_HEIGHT && touchX > TEXTBOX_X && touchX < TEXTBOX_X + TEXTBOX_WIDTH) {
    caret = std::clamp((touchX - TEXT_X) / 8, 0, (int)text.size());
    caretTimer = 0;
  }
}

void KeyboardMenu::render(Screen& screen, Screen& bottomScreen)
{
  bottomScreen.clear(Color::get(0));

  // draw textbox
  const int TEXTBOX_WIDTH = bottomScreen.w - TEXTBOX_X * 2;
  bottomScreen.renderBox(TEXTBOX_X, TEXTBOX_Y, TEXTBOX_WIDTH, TEXTBOX_HEIGHT, TEXTBOX_FRAME_COLOR);

  if (text.empty()) {
    bottomScreen.renderText(placeholder, TEXT_X, TEXT_Y, PLACEHOLDER_TEXT_COLOR);
  } else {
    bottomScreen.renderText(text, TEXT_X, TEXT_Y, TEXT_COLOR);
  }

  // draw caret
  if (caretTimer < CARET_BLINK_DELAY || (caretTimer - CARET_BLINK_DELAY) / CARET_BLINK_RATE % 2 == 0) {
    bottomScreen.renderBoxFilled((int)caret * 8 + TEXTBOX_X + TEXTBOX_H_PADDING, TEXTBOX_Y + TEXTBOX_V_PADDING - 1, 2, 10, CARET_COLOR);
  }

  // draw character keys
  for (int i = 0; i < (int)rows.size(); i++) {
    std::string_view row = rows[i];

    for (int j = 0; j < (int)row.length(); j++) {
      int x = KEYBOARD_X + j * KEY_H_STRIDE + i * KEY_H_STRIDE / 2;
      int y = KEYBOARD_Y + i * KEY_V_STRIDE;

      int keyColor = DEFAULT_KEY_COLOR;

      if (touchX >= x && touchX < x + KEY_WIDTH && touchY >= y && touchY < y + KEY_HEIGHT) {
        keyColor = KEY_HIGHLIGHT_COLOR;
      }

      std::string_view substring = row.substr(j, 1);
      bottomScreen.renderBoxFilled(x, y, KEY_WIDTH, KEY_HEIGHT, keyColor);
      bottomScreen.renderText(substring, x + KEY_H_PADDING, y + KEY_V_PADDING, KEY_TEXT_COLOR);
    }
  }

  // draw special keys
  for (auto& key : specialKeys) {
    int width = key.width();
    int keyColor = key.defaultColor;

    if (key.requiresInput && text.empty()) {
      keyColor = Color::get(333);
    } else if (touchX >= key.x && touchX < key.x + width && touchY >= key.y && touchY < key.y + KEY_HEIGHT) {
      keyColor = key.highlightColor;
    }

    bottomScreen.renderBoxFilled(key.x, key.y, width, KEY_HEIGHT, keyColor);
    bottomScreen.renderText(key.text, key.x + KEY_H_PADDING, key.y + KEY_V_PADDING, KEY_TEXT_COLOR);
  }
}
